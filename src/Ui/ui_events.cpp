// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Jeepify

#include <Arduino.h>
#include <String.h>
#include <nvs_flash.h>
#include "ui.h"
#include "lv_meter.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "ui_events.h"
#include "main.h"
#include "CompButton.h"

#pragma region Global_Definitions
uint8_t MultiPosToChange;

PeriphClass *ActivePeriphSingle;
PeriphClass *ActivePeriphSwitch;

lv_obj_t *Ui_LedSnd;
lv_obj_t *Ui_LedRcv;
lv_obj_t *Ui_LedPair;

lv_timer_t *SingleTimer;
lv_timer_t *MultiTimer;
lv_timer_t *SwitchTimer;
lv_timer_t *SettingsTimer;

#define MAX_SWITCHES 1

CompThing   *CompThingArray[4] = {NULL, NULL, NULL, NULL};
int          SwitchPositionX[4][4] = { {  0 ,   0,   0,   0},
									   { -50,  80,   0,   0},
									   {-100,  15, 130,   0},
									   {-120, -30,  60, 150} };

int FirstShownSwitch;
 
LV_IMG_DECLARE(ui_img_btn_png);      

void Keyboard_cb(lv_event_t * event);

void SingleUpdateTimer(lv_timer_t * timer);
void MultiUpdateTimer(lv_timer_t * timer);
void SwitchUpdateTimer(lv_timer_t * timer);
void SettingsUpdateTimer(lv_timer_t * timer);

void Ui_Single_Clicked(lv_event_t * e);
void Ui_Multi_Clicked(lv_event_t * e);

#pragma endregion Global_Definitions

#pragma region Screen_Peer
void Ui_Peer_Prepare()
{
	if (ActivePeer) {
		lv_label_set_text_static(ui_LblPeerName, ActivePeer->GetName());
		switch (ActivePeer->GetType())
		{
			case SWITCH_1_WAY:	 lv_img_set_src(ui_ImgPeerType, &ui_img_horstrelais2_png); break;
			case SWITCH_2_WAY:	 lv_img_set_src(ui_ImgPeerType, &ui_img_horstrelais2_png); break;
			case SWITCH_4_WAY:	 lv_img_set_src(ui_ImgPeerType, &ui_img_ansgarmodule_4_png); break;
			case MONITOR_ROUND:	 lv_img_set_src(ui_ImgPeerType, &ui_img_rolfmodule_round_png); break;
			case MONITOR_BIG:	 lv_img_set_src(ui_ImgPeerType, &ui_img_friedermodule_disp_png); break;
			case BATTERY_SENSOR: lv_img_set_src(ui_ImgPeerType, &ui_img_friedermodule_disp_png); break;
		}	

		if (ActivePeer->GetSleepMode()) {
			lv_obj_add_state(ui_BtnPeer3, LV_STATE_CHECKED);
		}
		else {
			lv_obj_clear_state(ui_BtnPeer3, LV_STATE_CHECKED);
		}
		if (ActivePeer->GetDemoMode()) {
			lv_obj_add_state(ui_BtnPeer6, LV_STATE_CHECKED);
		}
		else {
			lv_obj_clear_state(ui_BtnPeer6, LV_STATE_CHECKED);
		}
	}
}
void Ui_Peer_Loaded(lv_event_t * e)
{
	if (!ActivePeer) ActivePeer = FindFirstPeer(MODULE_ALL);
	Ui_Peer_Prepare();
}

void Ui_Peer_Restart(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, SEND_CMD_RESTART);
}

void Ui_Peer_Reset(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, SEND_CMD_RESET);
}

void Ui_Peer_ToggleSleep(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, SEND_CMD_SLEEPMODE_TOGGLE);
}

void Ui_Peer_ToggleDemo(lv_event_t * e)
{
	if (ActivePeer) SendCommand(ActivePeer, SEND_CMD_DEMOMODE_TOGGLE);
}

void Ui_Peer_Next(lv_event_t * e)
{
	Serial.println("Peer-Next");
	ActivePeer = FindNextPeer(ActivePeer, MODULE_ALL, true); 
	if (ActivePeer) Ui_Peer_Prepare();
}
void Ui_Peer_Last(lv_event_t * e)
{
	ActivePeer = FindPrevPeer(ActivePeer, MODULE_ALL, true); 
	if (ActivePeer) Ui_Peer_Prepare();
}
void Ui_Peer_Delete(lv_event_t * e)
{
	if (ActivePeer) DeletePeer(ActivePeer);
}
#pragma endregion Screen_Peer
#pragma region Screen_Settings
void Ui_Set_WebSvr(lv_event_t * e)
{
	//ToggleWebServer();
}

void Ui_Set_Prepare(lv_event_t * e)
{	
	uint32_t user_data = 10;
	if (SettingsTimer) 
		{
			lv_timer_resume(SettingsTimer);
			if (Self.GetDebugMode()) Serial.println("SettingsTimer resumed");
		}
		else 
		{
			SettingsTimer = lv_timer_create(SettingsUpdateTimer, 500,  &user_data);
			if (Self.GetDebugMode()) Serial.println("SettingsTimer created");
		}
}

void Ui_Set_TogglePair(lv_event_t * e)
{
	TogglePairMode();
}

void Ui_Set_Restart(lv_event_t * e)
{
	ESP.restart();
}

void Ui_Set_Reset(lv_event_t * e)
{
	nvs_flash_erase(); nvs_flash_init();
	ESP.restart();
}

void Ui_Set_ToggleDebug(lv_event_t * e)
{
	ToggleDebugMode();
}

void Ui_Set_SavePeers(lv_event_t * e)
{
    SavePeers();
	Self.SetChanged(false);
}
void SettingsUpdateTimer(lv_timer_t * timer)
{
	if (Self.GetPairMode()) lv_obj_add_state(ui_BtnSet2, LV_STATE_CHECKED);
	else lv_obj_clear_state(ui_BtnSet2, LV_STATE_CHECKED);
	
	if (Self.GetDebugMode()) lv_obj_add_state(ui_BtnSet7, LV_STATE_CHECKED);
	else lv_obj_clear_state(ui_BtnSet7, LV_STATE_CHECKED);
	
	if (Self.GetChanged()) lv_obj_add_state(ui_BtnSet8, LV_STATE_CHECKED);
	else lv_obj_clear_state(ui_BtnSet8, LV_STATE_CHECKED);

	if (WebServerActive) lv_obj_add_state(ui_BtnSet1, LV_STATE_CHECKED);
	else lv_obj_clear_state(ui_BtnSet1, LV_STATE_CHECKED);
}
void Ui_Set_Leave(lv_event_t * e)
{
	lv_timer_del(SettingsTimer);
	SettingsTimer = NULL;

	if (Self.GetDebugMode()) Serial.println("SettingsTimer deleted");
}

#pragma endregion Screen_Settings
#pragma region Screen_Peers
void Ui_Peers_Prepare(lv_event_t * e)
{
	String Options = "";
	PeerClass *P;

    for(int i = 0; i < PeerList.size(); i++){

		P = PeerList.get(i);

		if (Options != "") Options += "\n";
			
		if (millis()- P->GetTSLastSeen() > OFFLINE_INTERVAL) Options += "off: <";
		else Options += "on:  <"; 
			
		Options += P->GetName();

		switch (P->GetType()) {
			case SWITCH_1_WAY:   Options += "> PDC-1"; break;
			case SWITCH_2_WAY:   Options += "> PDC-2"; break;
			case SWITCH_4_WAY:   Options += "> PDC-4"; break;
			case SWITCH_8_WAY:   Options += "> PDC-8"; break;
			case PDC_SENSOR_MIX: Options += "> MIX";   break;
			case BATTERY_SENSOR: Options += "> Sens";  break;
			default:		     Options += "> ???";   break;
		}
	}
	
	lv_roller_set_options(ui_RollerPeers1, Options.c_str(), LV_ROLLER_MODE_NORMAL);
}

void Ui_Peers_Selected(lv_event_t * e)
{
	char buf[100];
	char SelectedName[50];
	
	lv_roller_get_selected_str(ui_RollerPeers1, buf, 100);
	Serial.print("Selected: ");
	Serial.println(buf);

  	char *Start = strchr(buf,'<'); 
	char *End = strchr(buf,'>'); 
	
  	memcpy(SelectedName, Start+1, End-Start-1);
	SelectedName[End-Start-1] = 0;
	Serial.println(SelectedName);

	PeerClass *TempPeer = FindPeerByName(SelectedName);

	if ((TempPeer) and (strcmp(SelectedName, "") != 0)) {
		ActivePeer = TempPeer;
		_ui_screen_change(&ui_ScrPeer, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrPeer_screen_init);
	}
}
#pragma endregion Screen_Peers
#pragma region Screen_JSON
void Ui_JSON_Prepare(lv_event_t * e)
{
	PrepareJSON();
}
#pragma endregion Screen_JSON
#pragma region Screen_SingleMeter
void Ui_Single_Next(lv_event_t * e)
{	
	if (ActivePeriphSingle) 
	{
		ActivePeriphSingle = FindNextPeriph(NULL, ActivePeriphSingle, SENS_TYPE_SENS, true);
	}
	
	if (ActivePeriphSingle)
	{
		Ui_Single_Leave(e);
		Ui_Single_Prepare(e);
	}
}

void Ui_Single_Last(lv_event_t * e)
{
	if (ActivePeriphSingle) 
	{
		ActivePeriphSingle = FindPrevPeriph(NULL, ActivePeriphSingle, SENS_TYPE_SENS, true);
	}
	
	if (ActivePeriphSingle)
	{
		Ui_Single_Leave(e);
		Ui_Single_Prepare(e);
	}
}

void Ui_Single_Prepare(lv_event_t * e)
{
	int Pos = 0;
	if (Self.GetDebugMode()) Serial.println("Single-Prepare");
	
	if (!ActivePeriphSingle) ActivePeriphSingle = FindFirstPeriph(NULL, SENS_TYPE_SENS);
		
	if (ActivePeriphSingle)
	{
		Serial.println("ActivePeriphSingle true");
		
		if (CompThingArray[Pos]) 
			{
				delete CompThingArray[Pos];
				CompThingArray[Pos] = NULL;
			}

		CompThingArray[Pos] = new CompMeter;
		//Serial.println("nach new Meter");
		CompThingArray[Pos]->Setup(ui_ScrSingle, 0, 0, 0, 360, ActivePeriphSingle, Ui_Single_Clicked);
		//Serial.println("nach new Meter-Setup");
		CompThingArray[Pos]->Update();
		
		static uint32_t user_data = 10;
		if (SingleTimer) 
		{
			lv_timer_resume(SingleTimer);
			
			//Serial.println("SingleTimer resumed");
		}
		else 
		{
			SingleTimer = lv_timer_create(SingleUpdateTimer, 500,  &user_data);
			//Serial.println("SingleTimer created");
		}

		Serial.println((unsigned)SingleTimer);
	}
}

void SingleUpdateTimer(lv_timer_t * timer)
{
	Serial.println("SinglUpdateTimer");
	int Pos= 0;
	if (CompThingArray[Pos]) CompThingArray[Pos]->Update();
}
void Ui_Single_Clicked(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Single_Next(e);
    }
    else if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Single_Last(e);
	}
	else if (event_code == LV_EVENT_CLICKED) {
		Ui_Single_Next(e);
	}
	else if (event_code == LV_EVENT_LONG_PRESSED) {
    }
}
void Ui_Single_Leave(lv_event_t * e)
{
	lv_timer_del(SingleTimer);
	SingleTimer = NULL;

	//Serial.println("SingleTimer deleted");
    	int Pos = 0;

	if (CompThingArray[Pos]) 
	{
		delete CompThingArray[Pos];
		CompThingArray[Pos] = NULL;
	}
}

static void SingleMeter_cb(lv_event_t * e) {

	lv_obj_draw_part_dsc_t	*dsc  = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);
	double					value;

	if( dsc->text != NULL ) {		// Filter major ticks...
		value = dsc->value / 10;
		snprintf(dsc->text, sizeof(dsc->text), "%5.1f", value);
	}

}

#pragma endregion Screen_SingleMeter
#pragma region Screen_MultiMeter
void Ui_Multi_Loaded(lv_event_t * e)
{
	static uint32_t user_data = 10;
		
	lv_label_set_text(ui_LblMultiScreenName,Screen[ActiveMultiScreen].GetName());
					
	for (int Pos=0; Pos<PERIPH_PER_SCREEN; Pos++)
	{
		int x; int y;
		switch (Pos) {
			case 0: x= -65; y=-50; break;
			case 1: x=  65; y=-50; break;
			case 2: x= -65; y= 70; break;
			case 3: x=  65; y= 70; break;
		}

		PeriphClass *Periph =  Screen[ActiveMultiScreen].GetPeriph(Pos);

		if (Periph)
		{
			Serial.printf("Name %s at Pos %d has Type %d\n\r", Periph->GetName(), Pos, Periph->GetType());
			lv_obj_add_flag(lv_obj_get_child(lv_scr_act(), Pos+1), LV_OBJ_FLAG_HIDDEN);
			
			if (CompThingArray[Pos]) 
				{
					delete CompThingArray[Pos];
					CompThingArray[Pos] = NULL;
				}

			if (Periph->IsSensor())
			{	
				CompThingArray[Pos] = new CompSensor;
				Serial.println("nach new sensor");
				CompThingArray[Pos]->Setup(ui_ScrMulti, x, y, Pos, 1, Periph, Ui_Multi_Clicked);
				Serial.println("nach setup");
				CompThingArray[Pos]->Update();
			}
			else if (Periph->IsSwitch())
			{
				CompThingArray[Pos] = new CompButton;
				CompThingArray[Pos]->Setup(ui_ScrMulti, x, y, Pos, 1, Periph, Ui_Multi_Clicked);
				CompThingArray[Pos]->Update();
			}
		}
		else
		{
			lv_obj_clear_flag(lv_obj_get_child(lv_scr_act(), Pos+1), LV_OBJ_FLAG_HIDDEN);
			CompThingArray[Pos] = NULL;
		}
	}
	if (MultiTimer) 
	{
		lv_timer_resume(MultiTimer);
		Serial.println("MultiTimer resumed");
	}
	else 
	{
		MultiTimer = lv_timer_create(MultiUpdateTimer, 500,  &user_data);
		Serial.println("MultiTimer created");
	}
}
void MultiUpdateTimer(lv_timer_t * timer)
{
	Serial.printf("MultiTimer - Screen[%d] \n\r",ActiveMultiScreen);
	
	for (int Pos=0; Pos<PERIPH_PER_SCREEN; Pos++) 
	{
		//Serial.printf("MultiTimer untersuche Pos:%d Id=(%d)\n\r", Pos, Screen[ActiveMultiScreen].GetPeriphId(Pos));

		if (Screen[ActiveMultiScreen].GetPeriphId(Pos) >= 0) CompThingArray[Pos]->Update();
	}
}
void Ui_Multi_Clicked(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Multi_Next(e);
    }
    else if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Multi_Prev(e);
	}
	else if (event_code == LV_EVENT_CLICKED) {
		PeriphClass *Periph = FindPeriphById(atoi(lv_label_get_text(lv_obj_get_child(target, 3))));
		if (Periph->IsSwitch())
		{
			Periph->SetChanged(true);
			
			if (lv_obj_get_state(target) == LV_IMGBTN_STATE_DISABLED) //komisch dass nicht Released
			{
				Periph->SetValue(0.0);
			}
			if (lv_obj_get_state(target) == LV_IMGBTN_STATE_CHECKED_RELEASED)
			{
				Periph->SetValue(1.0);
			}
			
			ToggleSwitch(Periph);
			//Serial.printf("Toggleswitch Pos:%d, PeerName:%s\n\r", SwitchArray[Pos]->GetPos(), FindPeerById(SwitchArray[Pos]->GetPeerId())->GetName());
		}
		else if (Periph->IsSensor())
		{
			ActivePeriphSingle = Periph;
			ActivePeer   = PeerOf(Periph);
			_ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScrSingle_screen_init);
		}
		
    }	
	else if (event_code == LV_EVENT_LONG_PRESSED) {
        MultiPosToChange = atoi(lv_label_get_text(lv_obj_get_child(target, 4)));
		///Ui_Multi_Unload(e);
		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
    }
}
void Ui_Multi_Set_Panel1(lv_event_t * e)
{
	if (PeriphList.size() > 0) 
	{
		MultiPosToChange = 0;
		Ui_Multi_Unload(e);
		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
	}
}
void Ui_Multi_Set_Panel2(lv_event_t * e)
{
	if (PeriphList.size() > 0) 
	{
		MultiPosToChange = 1;
		Ui_Multi_Unload(e);
		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
	}
}
void Ui_Multi_Set_Panel3(lv_event_t * e)
{
	if (PeriphList.size() > 0) 
	{
		MultiPosToChange = 2;
		Ui_Multi_Unload(e);
		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
	}
}
void Ui_Multi_Set_Panel4(lv_event_t * e)
{
	if (PeriphList.size() > 0) 
	{
		MultiPosToChange = 3;
		Ui_Multi_Unload(e);
		_ui_screen_change(&ui_ScrPeriph, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrPeriph_screen_init);
	}
}
void Ui_Multi_Unload(lv_event_t * e)
{
	if (MultiTimer) lv_timer_del(MultiTimer);
	MultiTimer = NULL;
	Serial.println("MultiTimer deleted");
	
	lv_obj_invalidate(lv_scr_act());
	
	for (int Pos = 0; Pos<PERIPH_PER_SCREEN; Pos++)
	{
		if (CompThingArray[Pos])
		{
			//CompThingArray[Pos]->Hide();
			//Serial.printf("CompThing[%d] (%s) hat Class %d\n\r", Pos, CompThingArray[Pos]->GetPeriph()->GetName(), CompThingArray[Pos]->GetClassType());
			
			Serial.println("will irgendeine Klasse löschen");
			delete CompThingArray[Pos];
			CompThingArray[Pos] = NULL;
			//Serial.println("Buttonclass gelöscht");
		}
	}
}
void Ui_Multi_Next(lv_event_t * e)
{
	ActiveMultiScreen++;
	if (ActiveMultiScreen == MULTI_SCREENS) ActiveMultiScreen = 0;
	Serial.printf("Schalte auf MultiScreen: %d\n\r", ActiveMultiScreen);
	
	Ui_Multi_Unload(e);
	Ui_Multi_Loaded(e);
	//_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrMulti_screen_init);
}
void Ui_Multi_Prev(lv_event_t * e)
{
	ActiveMultiScreen--;
	if (ActiveMultiScreen == -1) ActiveMultiScreen = MULTI_SCREENS-1;
	Serial.printf("Schalte auf MultiScreen: %d\n\r", ActiveMultiScreen);
	
	Ui_Multi_Unload(e);
	Ui_Multi_Loaded(e);
	//_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScrMulti_screen_init);
}
#pragma endregion Screen_MultiMeter
#pragma region Screen_Switch
void SwitchUpdateTimer(lv_timer_t * timer)
{
	int Pos = 0;

	Serial.println("Begin SwitchTimer");
	CompThingArray[Pos]->Update();
}
void Ui_Switch_Next(lv_event_t * e)
{
	PeriphClass *TestPeriph = FindNextPeriph(NULL, ActivePeriphSwitch, SENS_TYPE_SWITCH, true);

	if (TestPeriph)
	{
		ActivePeriphSwitch = TestPeriph;
		Ui_Switch_Leave(e);
		Ui_Switch_Loaded(e);
	}
}
void Ui_Switch_Clicked(lv_event_t * e)
{	
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
	PeriphClass *Periph;

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Switch_Next(e);
    }
    else if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Switch_Prev(e);
	}
	else if (event_code == LV_EVENT_CLICKED) {
		Periph = FindPeriphById(atoi(lv_label_get_text(lv_obj_get_child(target, 3))));

		Periph->SetChanged(true);
		Serial.printf("Button %s-State in event is %d\n\r", Periph->GetName(), lv_obj_get_state(target));

		if (lv_obj_get_state(target) == LV_IMGBTN_STATE_DISABLED) //komisch dass nicht Released
		{
			Periph->SetValue(0.0);
			Serial.printf("Button %s-State is DISABLED - Schalte aus\\r", Periph->GetName());
		}
		if (lv_obj_get_state(target) == LV_IMGBTN_STATE_CHECKED_RELEASED)
		{
			Periph->SetValue(1.0);
			Serial.printf("Button %s-State is CHECKED_RELEASED - Schalte ein\n\r", Periph->GetName());
		}
		Serial.printf("Schalter vor Toggle: Value = %f, Changed = %d\n\r", Periph->GetValue(), Periph->GetChanged());
	
		ToggleSwitch(Periph);
		//Serial.printf("Toggleswitch Pos:%d, PeerName:%s\n\r", SwitchArray[Pos]->GetPos(), FindPeerById(SwitchArray[Pos]->GetPeerId())->GetName());
    }	
}
void Ui_Switch_Prev(lv_event_t * e)
{
	PeriphClass *TestPeriph = FindPrevPeriph(NULL, ActivePeriphSwitch, SENS_TYPE_SWITCH, true);
	
	if (TestPeriph)
	{
		ActivePeriphSwitch = TestPeriph;
		Ui_Switch_Leave(e);
		Ui_Switch_Loaded(e);
	}
}
void Ui_Switch_Loaded(lv_event_t * e)
{
	int Pos = 0;
	if (!ActivePeriphSwitch) 
	{
		Serial.println("No ActivePeriphSwitch");
		ActivePeriphSwitch = FindNextPeriph(NULL, NULL, SENS_TYPE_SWITCH, true);
	}	
	if (ActivePeriphSwitch)
	{
		if (CompThingArray[Pos]) delete CompThingArray[Pos];

		CompThingArray[Pos] = new CompButton();
		CompThingArray[Pos]->Setup(ui_ScrSwitch, 0, 0, 0, 2, ActivePeriphSwitch, Ui_Switch_Clicked);
	}

	static uint32_t user_data = 10;
	if (!SwitchTimer) { 
		SwitchTimer = lv_timer_create(SwitchUpdateTimer, 500,  &user_data);
	}
	else {
		lv_timer_resume(SwitchTimer);
	}
}
void Ui_Switch_Leave(lv_event_t * e)
{
	int Pos = 0;
	
	if (SwitchTimer) 
	{
		lv_timer_del(SwitchTimer);
		SwitchTimer = NULL;
	}
	
	if (CompThingArray[Pos]) 
	{
		lv_obj_invalidate(CompThingArray[Pos]->GetButton());
		delete CompThingArray[Pos];
		CompThingArray[Pos] = NULL;
	}
}
#pragma endregion Screen_Switch
#pragma region Screen_PeriphChoice
void Ui_PeriphChoice_Next(lv_event_t * e)
{
	if (ActivePeriph) {
		PeriphClass *PeriphT = FindNextPeriph(NULL, ActivePeriph, SENS_TYPE_ALL, true);
		if (PeriphT) 
		{
			ActivePeriph = PeriphT;
			Ui_Periph_Choice_Loaded(e);
		}
	}
}
void Ui_PeriphChoice_Last(lv_event_t * e)
{
	if (ActivePeriph) {
		PeriphClass *PeriphT = FindPrevPeriph(NULL, ActivePeriph, SENS_TYPE_ALL, true);
		if (PeriphT) 
		{
			ActivePeriph = PeriphT;
			Ui_Periph_Choice_Loaded(e);
		}
	}
}
void Ui_PeriphChoice_Click(lv_event_t * e)
{
	Screen[ActiveMultiScreen].AddPeriph(MultiPosToChange, ActivePeriph);
	Self.SetChanged(true);
	_ui_screen_change(&ui_ScrMulti, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScrMulti_screen_init);
}
void Ui_Periph_Choice_Loaded(lv_event_t * e)
{
	Serial.println("PeriphChoice loaded");

	PeerClass *P;
	if (!ActivePeriph) {
		ActivePeriph = FindFirstPeriph(NULL, SENS_TYPE_ALL);
	}
	Serial.printf("aktueller Periph: %s", ActivePeriph->GetName());

	if (ActivePeriph) {
		Serial.println("ActivePeriph in PeriphCoice");

		P = PeerOf(ActivePeriph);

		lv_label_set_text(ui_LblPeriphChoicePeriph, ActivePeriph->GetName());
		lv_label_set_text(ui_LblPeriphChoicePeer, P->GetName());
	
		if (millis()- P->GetTSLastSeen() > OFFLINE_INTERVAL) 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Offline");
      	else 
			lv_label_set_text(ui_LblPeriphChoiceOnline, "Online");

		switch (ActivePeriph->GetType()) {
			case SENS_TYPE_SWITCH:	lv_label_set_text(ui_LblPeriphChoiceType, "Switch"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn2_png);
									break;
			case SENS_TYPE_AMP:		lv_label_set_text(ui_LblPeriphChoiceType, "Amp-Sensor"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn1_png);
									break;
			case SENS_TYPE_VOLT:	lv_label_set_text(ui_LblPeriphChoiceType, "Volt-Sensor"); 
									lv_img_set_src(ui_ImgPeerChoice, &ui_img_menubtn1_png);
									break;
			default:				lv_label_set_text(ui_LblPeriphChoiceType, "unknown type"); break;
		}
		Serial.println("ActivePeriph in PeriphCoice fertig");
	}
	Serial.println("PeriphCoice ende");
}
/*void Ui_Periph_Choice_prepare(lv_event_t * e)
{
	if (!ActivePeriphSwitch) ActivePeriphSwitch = FindNextPeriph(NULL, NULL, SENS_TYPE_SWITCH, true);
	if (ActivePeriphSwitch) _ui_screen_change(&ui_ScrSwitch, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSwitch_screen_init);
}*/
#pragma endregion Screen_PeriphChoice
#pragma region System_TimerAndInit
void TopUpdateTimer(lv_timer_t * timer)
{
	if ((TSMsgSnd) and (millis() - TSMsgSnd < MSGLIGHT_INTERVAL)) {
		lv_led_on(Ui_LedSnd);
	}
	else {
		lv_led_off(Ui_LedSnd);
		TSMsgSnd = 0;

	}

	if ((TSMsgRcv) and (millis() - TSMsgRcv < MSGLIGHT_INTERVAL)) {
		lv_led_on(Ui_LedRcv);
	}
	else {
		lv_led_off(Ui_LedRcv);
		TSMsgRcv = 0;
	}

	if ((TSPair)  and (millis() - TSPair < PAIR_INTERVAL)){
		lv_led_on(Ui_LedPair);
	}
	else {
		lv_led_off(Ui_LedPair);
		TSPair = 0;
		Self.SetPairMode(false);
	}
}

void Ui_Init_Custom(lv_event_t * e)
{
	//LED-Layer
	static uint32_t user_data = 10; 
	char LEDSize = 10;
	lv_timer_t * timer = lv_timer_create(TopUpdateTimer, 100,  &user_data);

	Ui_LedRcv  = lv_led_create(lv_layer_top());
	lv_obj_set_size(Ui_LedRcv, LEDSize, LEDSize);
    lv_obj_align(Ui_LedRcv, LV_ALIGN_CENTER, 0, 170);
    lv_led_set_color(Ui_LedRcv, lv_palette_main(LV_PALETTE_GREEN));
	lv_led_off(Ui_LedRcv);

    Ui_LedSnd  = lv_led_create(lv_layer_top());
	lv_obj_set_size(Ui_LedSnd, LEDSize, LEDSize);
    lv_obj_align(Ui_LedSnd, LV_ALIGN_CENTER, -20, 167);
    lv_led_set_color(Ui_LedSnd, lv_palette_main(LV_PALETTE_BLUE));

    Ui_LedPair  = lv_led_create(lv_layer_top());
    lv_obj_set_size(Ui_LedPair, LEDSize, LEDSize);
	lv_obj_align(Ui_LedPair, LV_ALIGN_CENTER, 20, 167);
    lv_led_set_color(Ui_LedPair, lv_palette_main(LV_PALETTE_RED));

	//Keyboard
	static const char* kb_map[] = { "1", "2", "3", "\n",
								"4", "5", "6", "\n",
								"7", "8", "9", "\n",
								"." ,"0", LV_SYMBOL_BACKSPACE,"\n",
								LV_SYMBOL_CLOSE, LV_SYMBOL_OK, "" };
	static const lv_btnmatrix_ctrl_t kb_ctrl[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    
	lv_keyboard_set_map(ui_Keyboard, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(ui_Keyboard, LV_KEYBOARD_MODE_USER_1);
	lv_obj_add_event_cb(ui_Keyboard, Keyboard_cb, LV_EVENT_READY, NULL);

	lv_label_set_text(ui_LblMenuVersion, _Version);
}

void Keyboard_cb(lv_event_t * event)
{
    CalibVolt();
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}
#pragma endregion System_TimerAndInit
#pragma region System_Eichen
void Ui_Eichen_Start(lv_event_t * e)
{
	TSMsgSnd = millis();
	//SendCommand(ActivePeer, SEND_CMD_CURRENT_CALIB);
	CalibAmp();
	
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}

void Ui_Volt_Prepare(lv_event_t * e)
{
	if (ActivePeer) lv_label_set_text(ui_LblVoltPeer, ActivePeer->GetName());
}
void Ui_Volt_Start(lv_event_t * e)
{
	CalibVolt();
	_ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrMenu_screen_init);
}

#pragma endregion System_Eichen
#pragma region Menu
void Ui_Menu_Loaded(lv_event_t * e)
{
	lv_label_set_text(ui_LblMenuVersion, Self.GetVersion());
}

void Ui_Menu_Btn1_Clicked(lv_event_t * e)
{
	if (!ActivePeriphSingle) ActivePeriphSingle = FindFirstPeriph(NULL, SENS_TYPE_SENS);
	if (ActivePeriphSingle) _ui_screen_change(&ui_ScrSingle, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSingle_screen_init);
}

void Ui_Menu_Btn2_Clicked(lv_event_t * e)
{
	if (!ActivePeriphSwitch) ActivePeriphSwitch = FindNextPeriph(NULL, NULL, SENS_TYPE_SWITCH, true);
	if (ActivePeriphSwitch) _ui_screen_change(&ui_ScrSwitch, LV_SCR_LOAD_ANIM_FADE_ON, 50, 0, &ui_ScrSwitch_screen_init);
}
#pragma endregion Menu
