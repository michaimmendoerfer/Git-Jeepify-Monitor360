#include "Ui/ui.h"
#include "CustomScreens.h"
#include <Arduino.h>
#include "lv_meter.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "main.h"
#include "CompButton.h"

lv_timer_t *MeterTimer;
lv_obj_t *ui_ScrMeter;

void ui_ScrMeter_screen_init(void)
{
    ui_ScrMeter = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrMeter, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScrMeter, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrMeter, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_ScrMeter, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_ScrMeter, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScrMeter, ui_event_ui_ScrMeter, LV_EVENT_ALL, NULL);

}
void Ui_Meter_Next(lv_event_t * e)
{	
	ActivePeriphSingle = FindNextPeriph(NULL, ActivePeriphSingle, ActivePeriphSingle->GetType(), CIRCULAR);
	
	if (ActivePeriphSingle)
	{
		Ui_Meter_Leave(e);
		Ui_Meter_Prepare(e);
	}
}
void Ui_Meter_Prev(lv_event_t * e)
{
	ActivePeriphSingle = FindPrevPeriph(NULL, ActivePeriphSingle, ActivePeriphSingle->GetType(), CIRCULAR);
	
	if (ActivePeriphSingle)
	{
		Ui_Meter_Leave(e);
		Ui_Meter_Prepare(e);
	}
}
void Ui_Meter_Prepare(lv_event_t * e)
{
	int Pos = 0;
	if (!ActivePeriphSingle) ActivePeriphSingle = FindNextPeriph(NULL, NULL, SENS_TYPE_SENS, ONLINE);
	if (!ActivePeriphSingle) ActivePeriphSingle = FindNextPeriph(NULL, NULL, SENS_TYPE_AMP, ONLINE);
		
	if (ActivePeriphSingle)
	{
		Serial.printf("ActivePeriphSinge = %s\n\r", ActivePeriphSingle->GetName());
		if (CompThingArray[Pos]) 
			{
				delete CompThingArray[Pos];
				CompThingArray[Pos] = NULL;
			}

		CompThingArray[Pos] = new CompMeter;
		CompThingArray[Pos]->Setup(ui_ScrMeter, 0, 0, 0, 360, ActivePeriphSingle, Ui_Meter_Clicked);
		CompThingArray[Pos]->Update();
		
		static uint32_t user_data = 10;
		
		if (MeterTimer) 
		{
			lv_timer_resume(MeterTimer);
		}
		else 
		{
			MeterTimer = lv_timer_create(MeterUpdateTimer, 500,  &user_data);
		}
	}
}

void MeterUpdateTimer(lv_timer_t * timer)
{
	if (CompThingArray[0]) CompThingArray[0]->Update();
}
void Ui_Meter_Clicked(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Meter_Next(e);
    }
    else if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Meter_Prev(e);
	}
	else if (event_code == LV_EVENT_CLICKED) {
		Ui_Meter_Next(e);
	}
	else if (event_code == LV_EVENT_LONG_PRESSED) {
    }
}
void Ui_Meter_Leave(lv_event_t * e)
{
	lv_timer_del(MeterTimer);
	MeterTimer = NULL;

	int Pos = 0;

	if (CompThingArray[Pos]) 
	{
		delete CompThingArray[Pos];
		CompThingArray[Pos] = NULL;
	}
}

void ui_event_ui_ScrMeter(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_ScrMenu, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, &ui_ScrMenu_screen_init);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Meter_Next(e);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Ui_Meter_Prev(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        Ui_Meter_Prepare(e);
    }
    if(event_code == LV_EVENT_SCREEN_UNLOAD_START) {
        Ui_Meter_Leave(e);
    }
}
