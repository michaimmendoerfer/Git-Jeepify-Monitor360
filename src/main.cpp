#include <Arduino.h>

#include "scr_st77916.h"

#define NODE_NAME "Monitor 360"
#define NODE_TYPE MONITOR_ROUND
//#define KILL_NVS 1

const char *_Version = "V 3.61";
const char *_Name = "Monitor 360";
const char _Protokoll_Version[] = "1.10";

#pragma region Includes
#include <Arduino.h>
#include "Jeepify.h"
#include "main.h"
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "pref_manager.h"
#include "PeerClass.h"
#include "LinkedList.h"
#include <lvgl.h>
#include "Ui\ui.h"
#include "Ui\ui_events.h" 
#include <nvs_flash.h>
#pragma endregion Includes

#pragma region Globals

int PeerCount;
Preferences preferences;
uint8_t broadcastAddressAll[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

PeerClass Self;

String jsondataBuf;

uint32_t TSPing          = 0;

volatile uint32_t TSMsgRcv  = 0;
volatile uint32_t TSMsgSnd  = 0;
volatile uint32_t TSMsgPDC  = 0;
volatile uint32_t TSMsgBat  = 0;
volatile uint32_t TSMsgVolt = 0;
volatile uint32_t TSMsgEich = 0;
volatile uint32_t TSMsgPair = 0;
volatile uint32_t TSPair    = 0;

lv_timer_t *WDButtonVars;

int ActiveMultiScreen;
bool WebServerActive = true;

#pragma endregion Globals
#pragma region Main
void OnDataRecv(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
    PeerClass *P;
    
    char* buff = (char*) incomingData;   
    JsonDocument doc; 
    String jsondata = String(buff); 
    
    //String BufS; 
    //char Buf[50] = {};
    char Tx[10] = {};
    char Nx[10] = {};
    char Bx[10] = {};
    
    bool SaveNeeded = false;
    bool NewPeer    = false;
    
    jsondataBuf = jsondata;
    PrepareJSON();
    
    DeserializationError error = deserializeJson(doc, jsondata);

    if (!error) // erfolgreich JSON
    {
        P = FindPeerByMAC(info->src_addr);
        TSMsgRcv = millis();
        int Order = (int)doc["Order"];

        if ((!P) and (Order == SEND_CMD_PAIR_ME) and (Self.GetPairMode())) // neuen Peer registrieren
        { 
            P = new PeerClass();
            PeerList.add(P);
            SaveNeeded = true;
            NewPeer    = true;
            Self.SetPairMode(false); TSPair = 0;
        }

        if (P)      // Peer bekannt
        { 
            P->SetTSLastSeen(millis());
            if (Self.GetDebugMode()) Serial.printf("bekannter Node: %s - LastSeen at %d", P->GetName(), P->GetTSLastSeen());
            
            if (Order == SEND_CMD_PAIR_ME) 
            // check or init names
            { 
                int    Status       = doc["Status"];
                String PeerName     = doc["Node"];
                String PeerVersion  = doc["Version"];

                if ((strcmp(PeerName.c_str(), P->GetName()) != 0) or (strcmp(PeerVersion.c_str(), P->GetVersion()) != 0))
                {
                    SaveNeeded = true;
                    P->Setup(PeerName.c_str(), (int)doc["Type"], PeerVersion.c_str(), info->src_addr, 
                        (bool) bitRead(Status, 1), (bool) bitRead(Status, 0), (bool) bitRead(Status, 2), (bool) bitRead(Status, 3));
                } 
                
                // Message-Bsp: "Node":"ESP32-1"; "T0":"1"; "N0":"Switch1"
                for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
                    snprintf(Tx, sizeof(Tx), "T%d", Si);                          // Check for T0 (Type of Periph 0)
                    if (Self.GetDebugMode()) Serial.printf("Check Pairing for: %s", Tx);
                    
                    if (doc.containsKey(Tx)) 
                    {
                        if (Self.GetDebugMode()) Serial.printf("Pairing found: %s", Tx);       
                        int  Type = doc[Tx];                                       // Set Periph[0].Type

                        snprintf(Nx, sizeof(Nx), "N%d", Si);                      // get N0 (Name of Periph 0)
                        String PName = doc[Nx];

                        if ((strcmp(PName.c_str(), P->GetPeriphName(Si)) != 0) or (Type != P->GetPeriphType(Si)))
                        {
                                P->PeriphSetup(Si, PName.c_str(), Type, false, false, 0, 0, 0, P->GetId());
                                if (NewPeer) PeriphList.add(P->GetPeriphPtr(Si));
                                SaveNeeded = true;
                                if (Self.GetDebugMode()) Serial.printf("%s->Periph[%d].Name is now: %s\n", P->GetName(), Si, P->GetPeriphName(Si));
                        }
                    } 
                }
                for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
                    snprintf(Bx, sizeof(Bx), "B%d", Si);                      // get B0-Pos (Brother of Periph 0)
                    if (doc.containsKey(Bx)) 
                    {
                        int Brother = (int) doc[Bx];
                        //int BrotherId = P->GetPeriphPtr(Brother)->GetId();

                        if (P->GetPeriphBrotherPos(Si) != Brother)              
                        {
                            P->SetPeriphBrotherPos(Si, Brother);
                            SaveNeeded = true;
                            if (Self.GetDebugMode()) Serial.printf("%s->Periph[%d].BrotherPos is now: %d (%s)\n\r", P->GetName(), Si, P->GetPeriphBrotherPos(Si), P->GetPeriphName(Brother));
                        }
                     }
                }
                SendPairingConfirm(P); 
            }

            else // Peer known - no status, no pairing so read new values
            {
                for (int i=0; i<MAX_PERIPHERALS; i++) 
                {
                    if (doc.containsKey((const char*)P->GetPeriphName(i))) {
                        float TempSensor = (float)doc[P->GetPeriphName(i)];
                        
                        //Serial.print(P->GetPeriphName(i)); Serial.print(" found = "); Serial.println(TempSensor);
                        
                        if (TempSensor != P->GetPeriphValue(i)) {
                            P->SetPeriphOldValue(i, P->GetPeriphValue(i));
                            P->SetPeriphValue(i, TempSensor);
                            P->SetPeriphChanged(i, true);
                        }
                        else P->SetPeriphChanged(i, false);
                    }

                    if (doc.containsKey("Status")) 
                    {
                        int Status = doc["Status"];
                        P->SetDebugMode ((bool) bitRead(Status, 0));
                        P->SetSleepMode ((bool) bitRead(Status, 1));
                        P->SetDemoMode  ((bool) bitRead(Status, 2));
                        P->SetPairMode  ((bool) bitRead(Status, 3));
                    } 
                } 
            }
        } 
        
        if (SaveNeeded)
        {
            SavePeers();
            SaveNeeded = false;
        }
        if (NewPeer)
        {
            ReportAll();
            RegisterPeers();
            NewPeer = false;
        }
    }
    else 
    {        // Error bei JSON
        Serial.print(F("deserializeJson() failed: ")); 
        Serial.println(error.f_str());
        return;
    }
}

void setup() 
{
    delay(200);
    Serial.begin(115200);
    scr_lvgl_init();

    Self.Setup(_Name, MONITOR_ROUND, _Version, broadcastAddressAll, false, true, false, false);
    
    #ifdef KILL_NVS
        nvs_flash_erase(); nvs_flash_init(); ESP.restart();
        while(1)
        {}
    #endif
    //TFT & LVGL

    WiFi.mode(WIFI_AP_STA);
    //ESP-Now
    
    if (esp_now_init() != ESP_OK) { Serial.println("Error initializing ESP-NOW"); return; }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);    

    //Get saved Peers  
    preferences.begin("JeepifyInit", true);
        Self.SetDebugMode(preferences.getBool("DebugMode", true));
        Self.SetSleepMode(preferences.getBool("SleepMode", false));
        Self.SetName(preferences.getString("ModuleName", NODE_NAME).c_str());
    preferences.end();
    
    GetPeers();
    RegisterPeers();
    ReportAll();
  
    static uint32_t user_data = 10;
    lv_timer_t * TimerPing = lv_timer_create(SendPing, PING_INTERVAL,  &user_data);

    
    ui_init(); 
    Serial.println("Setup ende...");
}
void loop() 
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
#pragma endregion Main

#pragma region Send-Things
void SendPing(lv_timer_t * timer) {
    JsonDocument doc; String jsondata;

    PeerClass *P;
    
    doc["Node"] = NODE_NAME;   
    doc["Order"] = SEND_CMD_STAY_ALIVE;

    if (Self.GetPairMode())
    {
        doc["Pairing"] = "aktiv";
    }

    serializeJson(doc, jsondata);  
    
    for (int i=0; i<PeerList.size(); i++)
    {
        P = PeerList.get(i);
        if (P->GetType() > 0) esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  
    }
}
void SendPairingConfirm(PeerClass *P) {
  JsonDocument doc; String jsondata; 
  
  uint8_t *Broadcast = P->GetBroadcastAddress();
  
  doc["Node"]     = Self.GetName();   
  doc["Peer"]     = P->GetName();
  doc["Order"]    = SEND_CMD_YOU_ARE_PAIRED;
  doc["Type"]     = Self.GetType();
  doc["B0"]       = (uint8_t)Broadcast[0];
  doc["B1"]       = (uint8_t)Broadcast[1];
  doc["B2"]       = (uint8_t)Broadcast[2];
  doc["B3"]       = (uint8_t)Broadcast[3];
  doc["B4"]       = (uint8_t)Broadcast[4];
  doc["B5"]       = (uint8_t)Broadcast[5];

  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(broadcastAddressAll, (uint8_t *) jsondata.c_str(), 200); 
  if (Self.GetDebugMode())
  {
      Serial.print("Sent you are paired"); 
      Serial.println(jsondata);  
  }       
}
bool ToggleSwitch(PeerClass *P, int PerNr)
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]  = NODE_NAME;   
    doc["Order"] = SEND_CMD_SWITCH_TOGGLE;
    doc["Value"] = P->GetPeriphName(PerNr);
    doc["Pos"]   = P->GetPeriphPos(PerNr);
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}
bool ToggleSwitch(PeriphClass *Periph)
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]  = NODE_NAME;   
    doc["Order"] = SEND_CMD_SWITCH_TOGGLE;
    doc["Value"] = Periph->GetName();
    doc["Pos"]   = Periph->GetPos();

    Serial.printf("Toggle Value = %f\n\r", Periph->GetValue());
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(FindPeerById(Periph->GetPeerId())->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);
    
    return true;
}
void SendCommand(PeerClass *P, String Cmd) {
  JsonDocument doc; String jsondata; 
  
  doc["from"]  = Self.GetName();   
  doc["Order"] = Cmd;
  
  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  if (Self.GetDebugMode()) Serial.println(jsondata);
}
void SendCommand(PeerClass *P, int Cmd) {
  JsonDocument doc; String jsondata; 
  
  doc["from"]  = Self.GetName();   
  doc["Order"] = Cmd;
  
  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  if (Self.GetDebugMode()) Serial.println(jsondata);
}
void SendCommand(PeerClass *P, int Cmd, String Value) {
  JsonDocument doc; String jsondata; 
  
  doc["from"]  = Self.GetName();   
  doc["Order"] = Cmd;
  doc["Value"] = Value;
  
  serializeJson(doc, jsondata);  
  
  TSMsgSnd = millis();
  esp_now_send(P->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
  if (Self.GetDebugMode()) Serial.println(jsondata);
}
#pragma endregion Send-Things

#pragma region System-Screens
void PrepareJSON() {
  if (jsondataBuf) {
    JsonDocument doc;
  
    DeserializationError error = deserializeJson(doc, jsondataBuf);
    if (doc["Node"] != NODE_NAME) { 
      lv_textarea_set_placeholder_text(ui_TxtJSON1, jsondataBuf.c_str());
      jsondataBuf = "";
    }
  }
}
#pragma endregion System-Screens
#pragma region Other
void WriteStringToCharArray(String S, char *C) {
  int   ArrayLength = S.length()+1;    //The +1 is for the 0x00h Terminator
  S.toCharArray(C,ArrayLength);
}
bool ToggleSleepMode() 
{
    preferences.begin("JeepifyInit", false);
        Self.SetSleepMode(!Self.GetSleepMode());
        if (preferences.getBool("SleepMode", false) != Self.GetSleepMode()) preferences.putBool("SleepMode", Self.GetSleepMode());
    preferences.end();
    return Self.GetSleepMode();
}
bool ToggleDebugMode() {
  preferences.begin("JeepifyInit", false);
      Self.SetDebugMode(!Self.GetDebugMode());
      if (preferences.getBool("DebugMode", false) != Self.GetDebugMode()) preferences.putBool("DebugMode", Self.GetDebugMode());
  preferences.end();
  return Self.GetDebugMode();
}
bool TogglePairMode() {
  if (Self.GetPairMode())
  {
      Self.SetPairMode(false);
      TSPair = 0;
  }
  else 
  {
      Self.SetPairMode(true);
      TSPair = millis();
  };

  if (Self.GetDebugMode()) { Serial.print("PairMode changed to: "); Serial.println(Self.GetPairMode()); }
  
  return Self.GetPairMode();
}
void CalibVolt() {
  JsonDocument doc; String jsondata;
 
  doc["Node"]  = Self.GetName();  
  doc["Order"] = SEND_CMD_VOLTAGE_CALIB;
  doc["NewVoltage"] = lv_textarea_get_text(ui_TxtVolt);
  
  serializeJson(doc, jsondata);  

  esp_now_send(ActivePeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  
  if (Self.GetDebugMode()) Serial.println(jsondata);
}
void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
    if (Self.GetDebugMode()) {
        if (Self.GetDebugMode()) 
        {
            Serial.print("\r\nLast Packet Send Status:\t");
            Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
        }
    }
}

#pragma endregion Other
//
