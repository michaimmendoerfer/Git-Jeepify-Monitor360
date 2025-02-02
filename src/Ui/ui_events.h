// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQ-Jeepify-Monitor360

#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

void Ui_Init_Custom(lv_event_t * e);
void Ui_Menu_Btn1_Clicked(lv_event_t * e);
void Ui_Menu_Btn2_Clicked(lv_event_t * e);
void Ui_Set_Prepare(lv_event_t * e);
void Ui_Set_Leave(lv_event_t * e);
void Ui_Set_WebSvr(lv_event_t * e);
void Ui_Set_TogglePair(lv_event_t * e);
void Ui_Set_Restart(lv_event_t * e);
void Ui_Set_Reset(lv_event_t * e);
void Ui_Set_ToggleDebug(lv_event_t * e);
void Ui_Set_SavePeers(lv_event_t * e);
void Ui_Peers_Prepare(lv_event_t * e);
void Ui_Peers_Selected(lv_event_t * e);
void Ui_JSON_Prepare(lv_event_t * e);
void Ui_Single_Next(lv_event_t * e);
void Ui_Single_Last(lv_event_t * e);
void Ui_Single_Prepare(lv_event_t * e);
void Ui_Single_Leave(lv_event_t * e);
void Ui_Peer_Loaded(lv_event_t * e);
void Ui_Peer_Next(lv_event_t * e);
void Ui_Peer_Last(lv_event_t * e);
void Ui_Peer_Restart(lv_event_t * e);
void Ui_Peer_Reset(lv_event_t * e);
void Ui_Peer_ToggleSleep(lv_event_t * e);
void Ui_Peer_ToggleDemo(lv_event_t * e);
void Ui_Peer_Delete(lv_event_t * e);
void Ui_Eichen_Start(lv_event_t * e);
void Ui_Volt_Prepare(lv_event_t * e);
void Ui_Volt_Start(lv_event_t * e);
void Ui_PeriphChoice_Next(lv_event_t * e);
void Ui_PeriphChoice_Last(lv_event_t * e);
void Ui_PeriphChoice_Click(lv_event_t * e);
void Ui_Periph_Choice_Loaded(lv_event_t * e);
void Ui_Multi_Loaded(lv_event_t * e);
void Ui_Multi_Unload(lv_event_t * e);
void Ui_Multi_Next(lv_event_t * e);
void Ui_Multi_Prev(lv_event_t * e);
void Ui_Multi_Set_Panel1(lv_event_t * e);
void Ui_Multi_Set_Panel2(lv_event_t * e);
void Ui_Multi_Set_Panel3(lv_event_t * e);
void Ui_Multi_Set_Panel4(lv_event_t * e);
void Ui_Switch_Next(lv_event_t * e);
void Ui_Switch_Prev(lv_event_t * e);
void Ui_Switch_Loaded(lv_event_t * e);
void Ui_Switch_Leave(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
