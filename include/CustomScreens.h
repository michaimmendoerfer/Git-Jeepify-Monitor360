#ifndef CUSTOMSCREENS_H
#define CUSTOMSCREENS_H

#include "PeerClass.h"
#include "Ui/ui.h"
#include "CompButton.h"

// SCREEN: ui_ScrMeterVolt
void ui_ScrMeter_screen_init(void);
void ui_event_ui_ScrMeter(lv_event_t * e);
void Ui_Meter_Next(lv_event_t * e);
void Ui_Meter_Prev(lv_event_t * e);
void Ui_Meter_Prepare(lv_event_t * e);
void Ui_Meter_Leave(lv_event_t * e);
void MeterUpdateTimer(lv_timer_t * timer);
void Ui_Meter_Clicked(lv_event_t * e);
void ui_event_ui_ScrMeter(lv_event_t * e);

extern lv_obj_t *ui_ScrMeter;

extern PeriphClass *ActivePeriphSingle;
extern PeriphClass *ActivePeriphSwitch;
extern CompThing   *CompThingArray[4];

#endif