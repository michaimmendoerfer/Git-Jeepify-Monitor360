#ifndef COMPBUTTON_H
#define COMPBUTTON_H

#include <Arduino.h>
#include "lvgl.h"
#include "Ui/ui.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "main.h"
#include "CompButton.h"

class CompButton {
    static int  _ClassId;

    private:
        int _x;
        int _y;
	    int _Width;
	    int _Height;
        int _Id;
        int _Pos;
        
	    float *_ValueToFollow;
        bool _ShowLabels;

	    PeriphClass *_Periph;

        lv_obj_t *_Spinner;
        lv_obj_t *_Button;
        lv_obj_t *_LblPeer;
        lv_obj_t *_LblPeriph;
        lv_obj_t *_LblPeriphId;
        lv_obj_t *_LblPos;
        lv_obj_t *_LblAmp;

        lv_obj_t *_FollowTimer;

        lv_event_cb_t _event_cb;

    public:
        CompButton();
        //CompButton(lv_obj_t * comp_parent, int x, int y, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb, float *ValueToFollow);
        ~CompButton();
        void Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb);
        void SpinnerOn() 		{ lv_obj_clear_flag(_Spinner, LV_OBJ_FLAG_HIDDEN); }
        void SpinnerOff()		{ lv_obj_add_flag  (_Spinner, LV_OBJ_FLAG_HIDDEN); }
        
        bool SpinnerIsVisible() 	{ return !lv_obj_has_flag_any(_Spinner, LV_OBJ_FLAG_HIDDEN); }

        bool GetButtonState()   	{ if (lv_obj_has_state(_Button, LV_STATE_CHECKED)) return true; else return false; }
        void SetButtonState(bool State) { if (State) lv_imgbtn_set_state(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED); 
                                                else lv_imgbtn_set_state(_Button, LV_IMGBTN_STATE_RELEASED); }
        
        void SetAmp(char *Buf) { lv_label_set_text(_LblAmp, Buf); }
		void ShowAmp() { lv_obj_clear_flag(_LblAmp, LV_OBJ_FLAG_HIDDEN); };			
        void HideAmp() { lv_obj_add_flag  (_LblAmp, LV_OBJ_FLAG_HIDDEN); };	

        void SetPeer(char *Buf) { lv_label_set_text(_LblPeer, Buf); }
		void ShowPeer() { lv_obj_clear_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN); };			
        void HidePeer() { lv_obj_add_flag  (_LblPeer, LV_OBJ_FLAG_HIDDEN); };	

        void SetPeriph(char *Buf) { lv_label_set_text(_LblPeriph, Buf); }
		void ShowPeriph() { lv_obj_clear_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN); };			
        void HidePeriiph() { lv_obj_add_flag  (_LblPeriph, LV_OBJ_FLAG_HIDDEN); };	

        int  GetPos() { return _Pos; }
        void Hide() { lv_obj_add_flag  (_Button, LV_OBJ_FLAG_HIDDEN); SpinnerOff(); }
        void Show() { lv_obj_clear_flag(_Button, LV_OBJ_FLAG_HIDDEN); }

        lv_obj_t *GetButton() { return _Button; }
        PeriphClass *GetPeriph() { return _Periph; }

	//void Follow(float *Value);
};

#endif