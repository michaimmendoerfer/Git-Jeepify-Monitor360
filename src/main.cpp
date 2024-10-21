#include <Arduino.h>

#include "scr_st77916.h"

#define NODE_NAME "Monitor 360"
#define NODE_TYPE MONITOR_ROUND
//#define KILL_NVS 1

const char *_Version = "V 3.71";
const char *_Name = "Monitor 360";
const char _Protokoll_Version[] = "1.20";

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
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#pragma endregion Includes

#pragma region Globals

char *T[MAX_PERIPHERALS] = {"T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"};
char *N[MAX_PERIPHERALS] = {"N0", "N1", "N2", "N3", "N4", "N5", "N6", "N7", "N8"};
char *B[MAX_PERIPHERALS] = {"Br0", "Br1", "Br2", "Br3", "Br4", "B5r", "B6r", "B7r", "B8r"};
char *ArrNullwert[MAX_PERIPHERALS] = {"NW0", "NW1", "NW2", "NW3", "NW4", "NW5", "NW6", "NW7", "NW8"};
char *ArrVperAmp[MAX_PERIPHERALS] = {"VpA0", "VpA1", "VpA2", "VpA3", "VpA4", "VpA5", "VpA6", "VpA7", "VpA8"};
char *ArrVin[MAX_PERIPHERALS] = {"Vin0", "Vin1", "Vin2", "Vin3", "Vin4", "Vin5", "Vin6", "Vin7", "Vin8"};

int PeerCount;
Preferences preferences;
uint8_t broadcastAddressAll[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

struct ConfirmStruct {
    uint8_t  Address[6];
    char     Message[250];
    volatile uint32_t TSMessage;
    int      Try;
    bool     Confirmed;
};

MyLinkedList<ConfirmStruct*> ConfirmList = MyLinkedList<ConfirmStruct*>();

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

volatile uint32_t TSConfirm = 0;

lv_timer_t *WDButtonVars;

int ActiveMultiScreen;
bool WebServerActive = true;
#pragma endregion Globals

#pragma region WebServer
AsyncWebServer server(80);
const char* ssid = "Jeepify_Monitor";
const char* password = "";

const char* PARAM_MESSAGE = "message";

PeerClass   *ActiveWebPeer = NULL;
PeriphClass *ActiveWebPeriph = NULL;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Form</title>
<style>
body{ margin: 0;padding: 0;font-family: Arial, Helvetica, sans-serif;background-color: #2c257a;}
.box{ width: 70%%; padding: 10px; position: absolute; top: 50%%; left: 50%%; transform: translate(-50%%,-50%%); background-color: #191919; color: white; text-align: center; border-radius: 24px; box-shadow: 0px 1px 32px 0px rgba(0,227,197,0.59);}
h1{ text-transform: uppercase; font-weight: 500;}
input{ border: 0; background: none; margin: 20px auto; text-align: center; border: 2px solid #4834d4; padding: 14px 10px; width: 40%%; outline: none; border-radius: 24px; color: white; font-size: smaller; transition: 0.3s;}
input:focus{ width: 40%%; border-color:#22a6b3 ;}
input[type='submit']{ border: 0;background: none; margin: 20px auto; text-align: center; border: 2px solid #22a6b3; padding: 14px 10px; width: 120px; outline: none; border-radius: 24px; color: white; transition: 0.3s; cursor: pointer;}
input[type='submit']:hover{ background-color: #22a6b3;}
</style>
</head>
<body>
<p>&nbsp;</p>
<form id="values" class="box" action="/get">
<h1>%PeerName%</h1>
<div class="part"><input name="PeerName" type="text" placeholder="%PeerName%" />
<input name="message" type="submit" value="UpdPeerName" /></div>

<div class="part"><input name="PeriphName" type="%TYPE%" placeholder="%PeriphName%" />
<input name="message" type="submit" value="UpdPeriphName" /></div>
<div class="part"><input name="Nullwert" type="%TYPE%" placeholder="%Nullwert%" />
<input name="message" type="submit" value="UpdNullwert" /></div>
<div class="part"><input name="VperAmp" type="%TYPE%" placeholder="%VperAmp%" />
<input name="message" type="submit" value="UpdVperAmp" /></div>
<div class="part"><input name="Vin" type="%TYPE%" placeholder="%Vin%" />
<input name="message" type="submit" value="UpdVin" /></div>

<div class="part">
<table align="center">
<tbody>
<tr>
<td>
<div class="part"><input name="message" type="submit" value="module" /></div>
</td>
</tr>
<tr>
<td>
<div class="part"><input name="message" type="submit" value="prev" /></div>
</td>
<td>
<div class="part"><input name="message" type="submit" value="next" /></div>
</td>
</tr>
</tbody>
</table>
</div>
</form></body></html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var)
{
    char Buf[10];
    
    if (var == "TYPE")        if (ActiveWebPeriph) return "text";
                              else return "hidden";
    if (var == "PeerName")    return ActiveWebPeer->GetName();
    if (var == "PeriphName")  if (ActiveWebPeriph) return ActiveWebPeriph->GetName();
    if (var == "Nullwert")    if (ActiveWebPeriph) { dtostrf(ActiveWebPeriph->GetNullwert(), 0, 3, Buf); return String(Buf); }
    if (var == "VperAmp")     if (ActiveWebPeriph) { dtostrf(ActiveWebPeriph->GetVperAmp(), 0, 3, Buf); return String(Buf); }
    if (var == "Vin")         if (ActiveWebPeriph) return String(ActiveWebPeriph->GetVin());
    
    return String();
}
bool SendWebPeriphNameChange()
{
    JsonDocument doc; String jsondata; 

    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NAME;
    doc["NewName"] = ActiveWebPeriph->GetName();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}
bool SendWebPeerNameChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NAME;
    doc["NewName"] = ActiveWebPeer->GetName();
    doc["Pos"]     = 99;
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}
bool SendWebVinChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_VIN;
    doc["Value"]   = ActiveWebPeriph->GetVin();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}
bool SendWebVperAmpChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_VPERAMP;
    doc["Value"]   = ActiveWebPeriph->GetVperAmp();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}
bool SendWebNullwertChange()
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]    = NODE_NAME;   
    doc["Order"]   = SEND_CMD_UPDATE_NULLWERT;
    doc["Value"]   = ActiveWebPeriph->GetNullwert();
    doc["Pos"]     = ActiveWebPeriph->GetPos();
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(ActiveWebPeer->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);

    return true;
}

void InitWebServer()
{
    Serial.printf("create AP = %d", WiFi.softAP(ssid, password));
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    int txPower = WiFi.getTxPower();
    Serial.print("TX power: ");
    Serial.println(txPower);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    
    ActiveWebPeer = &Self;
    ActiveWebPeriph = NULL;
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        String WebBuffer;

        bool SaveNeeded  = false;

        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
            if (message == "UpdPeerName") 
            {
                if (request->hasParam("PeerName"))
                {
                  WebBuffer = request->getParam("PeerName")->value();
                  if (WebBuffer != "")
                    { 
                        if (Self.GetDebugMode()) Serial.printf("Received from web: NewPeerName = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeer) 
                        {   
                            SaveNeeded = true;
                            ActiveWebPeer->SetName(WebBuffer.c_str());
                            if (ActiveWebPeer != &Self) SendWebPeerNameChange();
                        }
                    }
                }
            }   
            if (message == "UpdPeriphName") 
            {    
                if (request->hasParam("PeriphName"))
                {
                  WebBuffer = request->getParam("PeriphName")->value();
                  if (WebBuffer != "")
                    { 
                        if (Self.GetDebugMode()) Serial.printf("Received from web: NewPeriphName = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetName(WebBuffer.c_str());
                            if (ActiveWebPeer != &Self) SendWebPeriphNameChange();
                        }
                    }
                }
            }
            if (message == "UpdNullwert") 
            {
                if (request->hasParam("Nullwert"))
                {
                  WebBuffer = request->getParam("Nullwert")->value();
                  if (WebBuffer != "")
                    { 
                        if (Self.GetDebugMode()) Serial.printf("Received from web: NewNullwert = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetNullwert(atof(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebNullwertChange();
                        }
                    }
                }
            }
            if (message == "UpdVperAmp") 
            {   
                if (request->hasParam("VperAmp"))
                {
                  WebBuffer = request->getParam("VperAmp")->value();
                  if (WebBuffer != "")
                    { 
                        if (Self.GetDebugMode()) Serial.printf("Received from web: NewVperAmp = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetVperAmp(atof(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebVperAmpChange();
                        }
                    }
                }
            }
            if (message == "UpdVin") 
            {
                if (request->hasParam("Vin"))
                {
                  WebBuffer = request->getParam("Vin")->value();
                  if (WebBuffer != "")
                    { 
                        if (Self.GetDebugMode()) Serial.printf("Received from web: NewVin = %s\n\r", WebBuffer.c_str());  
                        if (ActiveWebPeriph) 
                        {
                            SaveNeeded = true;
                            ActiveWebPeriph->SetVperAmp(atoi(WebBuffer.c_str()));
                            if (ActiveWebPeer != &Self) SendWebVinChange();
                        }
                    }
                }  
            }
            
            if (message == "module") 
            {
                if (Self.GetDebugMode()) Serial.println("Module aufgerufen");
                ActiveWebPeer   = &Self;
                ActiveWebPeriph = NULL;
                Serial.printf("aktueller Name = %s\n\r", ActiveWebPeer->GetName());
            }
            if (message == "prev") 
            {
                if (Self.GetDebugMode()) Serial.println("Prev aufgerufen");
                if (ActiveWebPeer == &Self) 
                {
                    PeerClass *TempP = FindFirstPeer(MODULE_ALL);
                    if (TempP) ActiveWebPeer = TempP;
                    ActiveWebPeriph = FindFirstPeriph(ActiveWebPeer, SENS_TYPE_ALL);  
                }
                else
                {
                    ActiveWebPeriph = FindPrevPeriph(NULL, ActiveWebPeriph, SENS_TYPE_ALL, true);
                    ActiveWebPeer   = FindPeerById(ActiveWebPeriph->GetPeerId());
                }

            }
            if (message == "next") 
            {
                if (Self.GetDebugMode()) Serial.println("Next aufgerufen");
                if (ActiveWebPeer == &Self) 
                {
                    PeerClass *TempP = FindFirstPeer(MODULE_ALL);
                    if (TempP) ActiveWebPeer = TempP;
                    ActiveWebPeriph = FindFirstPeriph(ActiveWebPeer, SENS_TYPE_ALL);  
                }
                else
                {
                    ActiveWebPeriph = FindNextPeriph(NULL, ActiveWebPeriph, SENS_TYPE_ALL, true);
                    ActiveWebPeer   = FindPeerById(ActiveWebPeriph->GetPeerId());
                }
            }
        } else {
            message = "No message sent";
        }
        request->send_P(200, "text/html", index_html, processor);
        
        if (SaveNeeded)
        {   
            if (ActiveWebPeer != &Self) SavePeers();
            else 
            {
                preferences.begin("JeepifyInit", false);
                preferences.putString("ModuleName", Self.GetName());
                preferences.end();
                Serial.println("Neuer Module Name gespeichert");
            }
            SaveNeeded = false;
        }
    });

  server.onNotFound(notFound);
}
void ToggleWebServer()
{   
    WebServerActive = !WebServerActive;
    if (WebServerActive) 
    {
        ActiveWebPeer = PeerList.get(0);
        ActiveWebPeriph = PeriphList.get(0);
        Serial.println("Server startet");
        server.begin();
    }
    else 
    {
        Serial.println("Server beendet");
        server.end();
    }
}
#pragma endregion WebServer

#pragma region Main
void OnDataRecv(const esp_now_recv_info *info, const uint8_t* incomingData, int len)
{
    PeerClass *P;
    
    char* buff = (char*) incomingData;   
    JsonDocument doc; 
    String jsondata = String(buff); 
    
    String BufS; char Buf[50] = {};
    bool SaveNeeded = false;
    bool NewPeer    = false;
    bool PeerCanConfirm = false;
    
    jsondataBuf = jsondata;
    PrepareJSON();
    
    DeserializationError error = deserializeJson(doc, jsondata);

    if (!error) // erfolgreich JSON
    {
        P = FindPeerByMAC(info->src_addr);
        TSMsgRcv = millis();
        int Order = (int)doc["Order"];

        // new Peer wants to pair and module too - create it
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
            if (Self.GetDebugMode()) Serial.printf("%d: bekannter Node: %s - LastSeen at %d\n\r", millis(), P->GetName(), P->GetTSLastSeen());

            // first time register periphs or periodically check if changed - save if needed
            if (Order == SEND_CMD_PAIR_ME) 
            { 
                int    Status       = doc["Status"];
                String PeerName     = doc["Node"];
                String PeerVersion  = doc["Version"];

                if ((strcmp(PeerName.c_str(), P->GetName()) != 0) or (strcmp(PeerVersion.c_str(), P->GetVersion()) != 0))
                {
                    SaveNeeded = true;
                    P->Setup(PeerName.c_str(), (int)doc["Type"], PeerVersion.c_str(), info->src_addr, 
                        (bool) bitRead(Status, 1), (bool) bitRead(Status, 0), (bool) bitRead(Status, 2), (bool) bitRead(Status, 3));
                    //P->SetConfirm(bitRead(Status, 4));
                } 
                
                // Message-Bsp: "Node":"ESP32-1"; "T0":"1"; "N0":"Switch1"
                for (int Si=0; Si<MAX_PERIPHERALS; Si++) {
                    if (Self.GetDebugMode()) Serial.printf("Check Pairing for: %s\n\r", Buf);
                    
                    int TempType = doc[T[Si]]; 
                    if (TempType)
                    {
                        if (Self.GetDebugMode()) Serial.printf("Pairing found: %s\n\r", Buf);       
                        String PName = doc[N[Si]];

                        if ((strcmp(PName.c_str(), P->GetPeriphName(Si)) != 0) or (TempType != P->GetPeriphType(Si)))
                        {
                                P->PeriphSetup(Si, PName.c_str(), TempType, false, false, 0, 0, 0, P->GetId());
                                if (NewPeer) PeriphList.add(P->GetPeriphPtr(Si));
                                SaveNeeded = true;
                                if (Self.GetDebugMode()) Serial.printf("%s->Periph[%d].Name is now: %s\n\r", P->GetName(), Si, P->GetPeriphName(Si));
                        }
                    } 
                    
                    int BrotherPos = (int) doc[B[Si]];
                    if (BrotherPos)
                    {
                        if (P->GetPeriphBrotherPos(Si) != BrotherPos)              
                        {
                            P->SetPeriphBrotherPos(Si, BrotherPos);
                            if (Self.GetDebugMode()) Serial.printf("%s->Periph[%d].BrotherPos is now: %d (%s)\n\r", P->GetName(), Si, P->GetPeriphBrotherPos(Si), P->GetPeriphBrotherPtr(Si)->GetName());
                        }
                     }
                }
                
                SendPairingConfirm(P); 
            }
            else // Peer known - no status-report, no pairing so read new values
            {
                for (int Si=0; Si<MAX_PERIPHERALS; Si++) 
                {
                    float TempSensor = (float)doc[P->GetPeriphName(Si)];
                    if (TempSensor)
                    {
                        if (abs(TempSensor) < SCHWELLE) TempSensor = 0;
                        //Serial.print(P->GetPeriphName(i)); Serial.print(" found = "); Serial.println(TempSensor);
                        
                        if (TempSensor != P->GetPeriphValue(Si)) {
                            P->SetPeriphOldValue(Si, P->GetPeriphValue(Si));
                            P->SetPeriphValue(Si, TempSensor);
                            P->SetPeriphChanged(Si, true);
                        }
                        else P->SetPeriphChanged(Si, false);
                    }

                    int Status = doc["Status"];
                    if (Status)
                    {
                        P->SetDebugMode ((bool) bitRead(Status, 0));
                        P->SetSleepMode ((bool) bitRead(Status, 1));
                        P->SetDemoMode  ((bool) bitRead(Status, 2));
                        P->SetPairMode  ((bool) bitRead(Status, 3));
                    } 

                    float TempNullWert = doc[ArrNullwert[Si]];
                    if (TempNullWert) 
                    {
                        P->SetPeriphNullwert(Si, TempNullWert);
                        Serial.printf("%s[%d] gefunden. Wert ist: %.3f. %s->Nullwert = %.3f\n\r", ArrNullwert[Si], Si, TempNullWert, P->GetName(), P->GetPeriphNullwert(Si));
                    }
                    
                    float TempVperAmp = doc[ArrVperAmp[Si]];
                    if (TempVperAmp) P->SetPeriphVperAmp(Si, TempVperAmp);
                    
                    float TempVin = doc[ArrVin[Si]];
                    if (TempVin) P->SetPeriphVin(Si, TempVin);
                } 

                uint32_t TempTS = (uint32_t) doc["TSConfirm"];
                if (TempTS)
                {
                    Serial.printf("Confirm (%d) empfangen von %s\n\r", TempTS, P->GetName());
                    for (int i=0; i<ConfirmList.size(); i++)
                    {
                        ConfirmStruct *TempConfirm;
                        TempConfirm = ConfirmList.get(i);
                    Serial.printf("empfangener TS ist: %d - durchsuchter TS (List[%d]) ist: %d\n\r", TempTS, i, TempConfirm->TSMessage);
                    if (TempConfirm->TSMessage == TempTS)
                        {
                            TempConfirm->Confirmed = true;
                            Serial.printf("Found at list[%d] - DELETED\n\r", i);
                        }
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
            //ReportAll();
            RegisterPeers();
            NewPeer = false;
        }
    }
    else // Error bei JSON
    {        
        Serial.print(F("deserializeJson() failed: ")); 
        Serial.println(error.f_str());
        Serial.printf("jsondata was: %s\n\r", jsondata);
        return;
    }
}

void setup() 
{
    delay(2000);
    Serial.begin(115200);
    scr_lvgl_init();

    Self.Setup(_Name, MONITOR_ROUND, _Version, broadcastAddressAll, false, true, false, false);
    
    #ifdef KILL_NVS
        nvs_flash_erase(); nvs_flash_init();
        while(1)
        {}
    #endif
    
    WiFi.mode(WIFI_AP_STA);
    InitWebServer();
    
    WebServerActive = !WebServerActive;
    ToggleWebServer();

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
}
void loop() 
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
#pragma endregion Main

#pragma region Send-Things
esp_err_t  JeepifySend(PeerClass *P, const uint8_t *data, size_t len, uint32_t TSConfirm, bool ConfirmNeeded = false)
{
    esp_err_t SendStatus = esp_now_send(P->GetBroadcastAddress(), data, len);
    
    Serial.printf("SendStatus was %d, ConfirmNeeded = %d\n\r", SendStatus, ConfirmNeeded);
    if (ConfirmNeeded)
    {   
        ConfirmStruct *Confirm = new ConfirmStruct;
        memcpy(Confirm->Address, P->GetBroadcastAddress(), 6);
        strcpy(Confirm->Message, (const char *)data);
        Confirm->Confirmed = false;
        Confirm->TSMessage = TSConfirm;
        Confirm->Try = 1;

        ConfirmList.add(Confirm);

        Serial.printf("added Msg: %s to ConfirmList\n\r", Confirm->Message, Confirm->Try);   
    }
    return SendStatus;
}
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

    if (ConfirmList.size() > 0)
    { 
        for (int i=ConfirmList.size()-1; i>=0; i--)
        {
            ConfirmStruct *Confirm = ConfirmList.get(i);
            Confirm->Try++;
            
            if (Confirm->Confirmed == true)
            {
                Serial.printf("deleted Msg: %s from ConfirmList: SUCCESS (tries: %d)\n\r", Confirm->Message, Confirm->Try);
                delete Confirm;
                ConfirmList.remove(i);
            }
            else if (Confirm->Try == 21)
            {
                Serial.printf("deleted Msg: %s from ConfirmList: FAILED (tries: %d)\n\r", Confirm->Message, Confirm->Try);
                delete Confirm;
                ConfirmList.remove(i);
            }
            else
            {
                Serial.printf("%d: reSending Msg: %s from ConfirmList Try: %d\n\r", millis(), Confirm->Message, Confirm->Try);
                esp_err_t SendStatus = esp_now_send(Confirm->Address, (uint8_t*) Confirm->Message, 200); 
            }
            
        }
    }
    
}
void SendPairingConfirm(PeerClass *P) {
  JsonDocument doc; String jsondata; 
  
  uint8_t *Broadcast = P->GetBroadcastAddress();
  
  doc["Node"]     = Self.GetName();   
  doc["Peer"]     = P->GetName();
  doc["Order"]    = SEND_CMD_YOU_ARE_PAIRED;
  doc["Type"]     = Self.GetType();
  doc["B0"]       = (uint8_t)Broadcast[0]; doc["B1"] = (uint8_t)Broadcast[1]; doc["B2"] = (uint8_t)Broadcast[2];
  doc["B3"]       = (uint8_t)Broadcast[3]; doc["B4"] = (uint8_t)Broadcast[4]; doc["B5"] = (uint8_t)Broadcast[5];

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
    return ToggleSwitch(P->GetPeriphPtr(PerNr));
}
bool ToggleSwitch(PeriphClass *Periph)
{
    JsonDocument doc; String jsondata; 
    
    doc["from"]  = Self.GetName();  
    doc["Order"] = SEND_CMD_SWITCH_TOGGLE;
    doc["Value"] = Periph->GetName();
    doc["Pos"]   = Periph->GetPos();

    //Serial.printf("Toggle Value = %f\n\r", Periph->GetValue());
    
    serializeJson(doc, jsondata);  
    
    TSMsgSnd = millis();
    esp_now_send(FindPeerById(Periph->GetPeerId())->GetBroadcastAddress(), (uint8_t *) jsondata.c_str(), 100);  //Sending "jsondata"  
    if (Self.GetDebugMode()) Serial.println(jsondata);
    
    return true;
}
void SendCommand(PeerClass *P, int Cmd, String Value) {
  JsonDocument doc; String jsondata; 
  
  doc["from"]  = Self.GetName();   
  doc["Order"] = Cmd;
  if (Value != "") doc["Value"] = Value;
  
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
    
    uint32_t TSConfirm = millis();

    doc["Node"]  = Self.GetName();  
    doc["Order"] = SEND_CMD_VOLTAGE_CALIB;
    doc["NewVoltage"] = lv_textarea_get_text(ui_TxtVolt);
    doc["TSConfirm"] = TSConfirm;
    
    serializeJson(doc, jsondata);  

    JeepifySend(ActivePeer, (uint8_t *) jsondata.c_str(), 100, TSConfirm, true);  
    
    if (Self.GetDebugMode()) Serial.println(jsondata);
}
void CalibAmp() 
{
    JsonDocument doc; String jsondata;

    uint32_t TSConfirm = millis();

    doc["Node"]  = Self.GetName();  
    doc["Order"] = SEND_CMD_CURRENT_CALIB;
    doc["TSConfirm"] = TSConfirm;
    
    serializeJson(doc, jsondata);  
    JeepifySend(ActivePeer, (uint8_t *) jsondata.c_str(), 100, TSConfirm, true);  

    if (Self.GetDebugMode()) Serial.println(jsondata);
}

void PrintMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
    if (Self.GetDebugMode()) 
    {
        //Serial.print("Last Packet Send Status: ");
        //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
        
        if (status == ESP_NOW_SEND_SUCCESS)
        {
            Serial.println("Message send SUCCESS");
        }
        else 
        {
            Serial.println("Message send FAILED");
        }
    }
}

#pragma endregion Other
//
