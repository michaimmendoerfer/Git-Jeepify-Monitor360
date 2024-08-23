#ifndef COMPBUTTON_H
#define COMPBUTTON_H

#include <Arduino.h>
#include "lvgl.h"
#include "Ui/ui.h"
#include "lv_meter.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "main.h"
#include "CompButton.h"

class CompThing {
    static int  _ClassId;

    protected:
        int _ClassType;
        
        int _x;
        int _y;
	    int _Width;
	    int _Height;
        int _Id;
        int _Pos;

        lv_obj_t *_Button;
        lv_obj_t *_LblPeer;
        lv_obj_t *_LblPeriph;
        lv_obj_t *_LblPeriphId;
        lv_obj_t *_LblPos;
        lv_obj_t *_LblValue;

        PeriphClass *_Periph;
	
	float *_ValueToFollow;
        bool _ShowLabels;

        lv_event_cb_t _event_cb;

    public:
        CompThing();
        PeriphClass *GetPeriph() { return _Periph; }

        lv_obj_t* GetButton() { return _Button; }
        bool GetButtonState()   	{ if (lv_obj_has_state(_Button, LV_STATE_CHECKED)) return true; else return false; }
        void SetButtonState(bool State) { if (State) lv_imgbtn_set_state(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED); 
                                                else lv_imgbtn_set_state(_Button, LV_IMGBTN_STATE_RELEASED); }
        
        void SetPeer(char *Buf) { lv_label_set_text(_LblPeer, Buf); }
		void ShowPeer() { lv_obj_clear_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN); };			
        void HidePeer() { lv_obj_add_flag  (_LblPeer, LV_OBJ_FLAG_HIDDEN); };	

        void SetPeriph(char *Buf) { lv_label_set_text(_LblPeriph, Buf); }
		void ShowPeriph() { lv_obj_clear_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN); };			
        void HidePeriph() { lv_obj_add_flag  (_LblPeriph, LV_OBJ_FLAG_HIDDEN); };	

        void SetValue(char *Buf) { lv_label_set_text(_LblValue, Buf); }
        void ShowValue() { lv_obj_clear_flag(_LblValue, LV_OBJ_FLAG_HIDDEN); };			
        void HideValue() { lv_obj_add_flag  (_LblValue, LV_OBJ_FLAG_HIDDEN); };	

        int  GetPos() { return _Pos; }
        int  GetClassType() { return _ClassType; }
};

class CompButton : public CompThing {

    private:
	lv_obj_t *_Spinner;
	bool _MobileLabels;

    public:
        CompButton();
        ~CompButton();
        void Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb);
        void SpinnerOn() 		{ lv_obj_clear_flag(_Spinner, LV_OBJ_FLAG_HIDDEN); }
        void SpinnerOff()		{ lv_obj_add_flag  (_Spinner, LV_OBJ_FLAG_HIDDEN); }
        
        bool SpinnerIsVisible() 	{ return !lv_obj_has_flag_any(_Spinner, LV_OBJ_FLAG_HIDDEN); }

        void SetAmp(char *Buf) { SetValue(Buf); }
		void ShowAmp() { ShowValue(); }		
        void HideAmp() { HideValue(); }	

	bool GetMobileLabels() { return _MobileLabels; }
	void SetMobileLabels(bool MobileLabels) { _MobileLabels = MobileLabels; }

        void Hide() { lv_obj_add_flag  (_Button, LV_OBJ_FLAG_HIDDEN); SpinnerOff(); }
        void Show() { lv_obj_clear_flag(_Button, LV_OBJ_FLAG_HIDDEN); }
        

	//void Follow(float *Value);
};

class CompSensor : public CompThing {

    private:
        lv_obj_t *_Arc;

    public:
        CompSensor();
        ~CompSensor();

        void Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb);
        
        void Hide() { lv_obj_add_flag  (_Button, LV_OBJ_FLAG_HIDDEN); }
        void Show() { lv_obj_clear_flag(_Button, LV_OBJ_FLAG_HIDDEN); }

        lv_obj_t *GetArc() { return _Arc; }
	//void Follow(float *Value);
};

class CompMeter : public CompThing {
    private:
	lv_obj_t *_Meter;
	lv_meter_indicator_t * _Indic;
	lv_meter_indicator_t * _IndicNeedle;
	lv_meter_scale_t * _Scale;

    public:
	CompMeter();
        ~CompMeter();

        void SetupModern (lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb);
        void SetupVintage(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb);

	void Update();

	void SetNeedle(float value) { lv_meter_set_indicator_value(_Meter, _IndicNeedle, value); }

	void Hide() { lv_obj_add_flag  (_Meter, LV_OBJ_FLAG_HIDDEN); }
        void Show() { lv_obj_clear_flag(_Meter, LV_OBJ_FLAG_HIDDEN); }
};

#endif
