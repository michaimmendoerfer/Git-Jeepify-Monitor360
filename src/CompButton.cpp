#include <Arduino.h>
#include "Ui/ui.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "main.h"
#include "CompButton.h"

//#define DEBUG 1

CompThing::CompThing()
{
    _PeerVisible      = true;
    _PeriphVisible    = true;
    _ValueVisible     = true;
    _SystemVisible    = false;
    _PeriphValueCombo = false;
}
CompThing::~CompThing()
{
	if (_Button)  { lv_obj_del(_Button); _Button = NULL; }
    Serial.println("Button weg");
}
void CompThing::Update()
{ 
	
}
void CompThing::Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{

}

CompButton::CompButton()
{
    _ClassType = 1;
	_SpinnerVisible = false;
}
CompButton::~CompButton()
{
    lv_obj_remove_event_cb(_Button, _event_cb);

    Serial.println("CompButton Destructor");
    if (_Spinner) { lv_obj_del(_Spinner); _Spinner = NULL; }
    Serial.println("Spinner weg");
}
void CompButton::Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{
    _Periph = Periph;
    _event_cb = event_cb;
    _ShowLabels = ShowLabels;
    _Pos = Pos;
   
    _x = x;
    _y = y;			
	
    if (size == 1)
    {
        _Width  =  70;
        _Height = 120;
    }
    else
    {
        _Width  = 120;
        _Height = 205;
    }
	
    _Spinner = lv_spinner_create(comp_parent, 1000, 90);
    Serial.printf("Setup Switch: _Spinner: %p\n\r", (void *)_Spinner);

	
    lv_obj_set_align(_Spinner, LV_ALIGN_CENTER);
    lv_obj_set_width(_Spinner,  _Width+30);
    lv_obj_set_height(_Spinner, _Width+30);

    lv_obj_set_x(_Spinner, _x);
    lv_obj_set_y(_Spinner, _y);
    lv_obj_add_flag(_Spinner, LV_OBJ_FLAG_HIDDEN);     
    lv_obj_clear_flag(_Spinner, LV_OBJ_FLAG_CLICKABLE);     
    lv_obj_set_style_arc_color(_Spinner, lv_color_hex(0x83061F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(_Spinner, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(_Spinner, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(_Spinner, 5, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_arc_color(_Spinner, lv_color_hex(0x31020B), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(_Spinner, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(_Spinner, 5, LV_PART_INDICATOR | LV_STATE_CHECKED);
	
    _Button = lv_imgbtn_create(comp_parent);
    if (size == 1)
    {
        lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1640860301, NULL);
        lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_743505413, NULL);
        lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_743505413, NULL);
        lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_888658411, NULL);
    }
    else
    {
	    lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1134846501, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_1528892059, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_1528892059, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_715952573, NULL);
    }

    lv_obj_set_align(_Button, LV_ALIGN_CENTER);
    lv_obj_set_height(_Button, _Height);
    lv_obj_set_width (_Button, _Width);   /// 1
    lv_obj_set_x(_Button, _x);
    lv_obj_set_y(_Button, _y);
    
    lv_obj_add_flag(_Button, LV_OBJ_FLAG_CHECKABLE);     
    lv_obj_clear_flag(_Button, LV_OBJ_FLAG_SCROLLABLE);     
	   
    lv_obj_add_flag(_Button, LV_OBJ_FLAG_OVERFLOW_VISIBLE);  


	_LblPeer = lv_label_create(_Button);
	lv_obj_set_width (_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeer, LV_SIZE_CONTENT);    /// 1
    	
	if (size == 1)
	{
        	SetPeerPos(-50, -5);
	    	lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else
	{
        	SetPeerPos(0, -115);
	    	lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
    
	ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,   _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_color_BtnTxt);
    lv_obj_set_style_radius(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,   _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA,   _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(_LblPeer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_LblPeer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_LblPeer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!_PeerVisible) lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);

    
	_LblPeriph = lv_label_create(_Button);
    	lv_obj_set_width(_LblPeriph, LV_SIZE_CONTENT);   /// 1
    	lv_obj_set_height(_LblPeriph, LV_SIZE_CONTENT);    /// 1
    
	if (size == 1)
	{
		SetPeriphPos(40, -5);
        lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else
	{
		SetPeriphPos(0, -145);
        lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
	}

	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_BtnTxt);
	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_BtnTxt);
	    lv_obj_set_style_radius(_LblPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_BtnBg);
	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_BtnBg);
	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_BtnBorder);
	    ui_object_set_themeable_style_property(_LblPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_BtnBorder);
	    lv_obj_set_style_border_width(_LblPeriph, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_pad_left(_LblPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_pad_right(_LblPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_pad_top(_LblPeriph, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_pad_bottom(_LblPeriph, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    	if (!_PeriphVisible) lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);


    _LblValue = lv_label_create(_Button);
    lv_obj_set_width(_LblValue, LV_SIZE_CONTENT);
    lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);  
	
	if (size == 1)
	{
		SetValuePos(50, 70);
    	lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else 
	{
		SetValuePos(0, 125);
		lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
    
	lv_label_set_text(_LblValue, "--.- A");
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_BtnTxt);
    lv_obj_set_style_text_align(_LblValue, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(_LblValue, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(_LblValue, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(_LblValue, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_LblValue, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_LblValue, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_LblValue, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_LblValue, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!_ValueVisible) lv_obj_add_flag(_LblValue, LV_OBJ_FLAG_HIDDEN);


    _LblPeriphId = lv_label_create(_Button);
    lv_obj_set_width(_LblPeriphId, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriphId, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeriphId, 50);
    lv_obj_set_y(_LblPeriphId, 0);
    lv_obj_set_align(_LblPeriphId, LV_ALIGN_CENTER);
	lv_label_set_text_fmt(_LblPeriphId, "%d", _Periph->GetId());
    lv_obj_set_style_text_color(_LblPeriphId, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!_ShowLabels) lv_obj_add_flag(_LblPeriphId, LV_OBJ_FLAG_HIDDEN);


    _LblPos = lv_label_create(_Button);
    lv_obj_set_width(_LblPos, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPos, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(_LblPos, LV_ALIGN_CENTER);
    lv_obj_set_x(_LblPos, 50);
    lv_obj_set_y(_LblPos,-15);
    lv_label_set_text_fmt(_LblPos, "%d", _Pos);
    lv_obj_set_style_text_color(_LblPos, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!_SystemVisible) lv_obj_add_flag(_LblPos, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(_Button, _event_cb, LV_EVENT_ALL, NULL);  
    //Update();
	
}
void CompButton::Update()
{
    lv_obj_set_x(_Spinner, _x);
    lv_obj_set_y(_Spinner, _y);
    
    lv_obj_set_x(_Button, _x);
    lv_obj_set_y(_Button, _y);
	
	lv_obj_set_x(_LblPeer, _X_Peer);
    lv_obj_set_y(_LblPeer, _Y_Peer);
    
    lv_obj_set_x(_LblPeriph, _X_Periph);
    lv_obj_set_y(_LblPeriph, _Y_Periph);
	
    lv_obj_set_x(_LblValue,  _X_Value);
    lv_obj_set_y(_LblValue,  _Y_Value);

	if ((!PeerOf(_Periph)->GetName()) or (!_PeerVisible))
	{
	    lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
	    lv_label_set_text_fmt(_LblPeer, "%.6s", PeerOf(_Periph)->GetName());
     	lv_obj_clear_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);
	}

    if (_SpinnerVisible) 
		lv_obj_clear_flag(_Spinner, LV_OBJ_FLAG_HIDDEN);   
	else
		lv_obj_add_flag(_Spinner, LV_OBJ_FLAG_HIDDEN);     
	
	char Buf_Periph[20] = {};
	char Buf_Value[10] = {};

	printf(Buf_Periph, "%.6s", _Periph->GetName());
	if ((_ValueVisible) and (_Periph->GetBrotherPos() > -1))
	{
		printf(Buf_Value, "%0.2fA", GetBrotherValueOf(_Periph));
	}
	
	if (_PeriphValueCombo)
	{	
		strcat(Buf_Periph, " ");
		strcat(Buf_Periph, Buf_Value);
		lv_obj_add_flag(_LblValue, LV_OBJ_FLAG_HIDDEN);
	}

    if ((!_Periph->GetName()) or (!_PeriphVisible)) 
    {
        lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);
    }
    else 
    {
        lv_label_set_text(_LblPeriph, Buf_Periph);
        lv_obj_clear_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);
    
    }

    if ((_ValueVisible) and (_Periph->GetBrotherPos() > -1) and (!_PeriphValueCombo))
    {
        lv_label_set_text(_LblValue, Buf_Value);
        lv_obj_clear_flag(_LblValue, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(_LblValue, LV_OBJ_FLAG_HIDDEN);
    }  
}

CompSensor::CompSensor()
{
    _ClassType = 2;
}
CompSensor::~CompSensor()
{
    lv_obj_remove_event_cb(_Button, _event_cb);
    Serial.println("CompSensor Destructor");
    if (_Button) { lv_obj_del(_Button); _Button = NULL; }
}
void CompSensor::Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{
    _Periph = Periph;
    _event_cb = event_cb;
    _ShowLabels = ShowLabels;
    _Pos = Pos;
   
    _x = x;
    _y = y;		

    _Width  = 100;
    _Height = 100;
    
    //Serial.printf("Pos: %d, x:%d, y:%d\n\r", Pos, x, y);
    
    _Button = lv_btn_create(comp_parent);
    lv_obj_set_width(_Button, _Width);
    lv_obj_set_height(_Button, _Height);
    lv_obj_set_x(_Button, x);
    lv_obj_set_y(_Button, y);
    lv_obj_set_align(_Button, LV_ALIGN_CENTER);
    lv_obj_add_flag(_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(_Button, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_Button, lv_color_hex(0x165420), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_Button, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(_Button, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(_Button, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    
    _LblPeer = lv_label_create(_Button);
    if (!PeerOf(Periph)->GetName()) 
	{
	    lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
	    lv_label_set_text_fmt(_LblPeer, "%.6s", PeerOf(Periph)->GetName());
	}

    lv_obj_set_width(_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeer, 0);
    lv_obj_set_y(_LblPeer, 25);
    lv_obj_set_align(_LblPeer, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    _LblPeriph = lv_label_create(_Button);
    
    if (!_Periph->GetName()) lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);
    else lv_label_set_text_fmt(_LblPeriph, "%.6s", _Periph->GetName());
    lv_obj_set_width(_LblPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriph, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(_LblPeriph, LV_ALIGN_CENTER);
    lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    _LblValue = lv_label_create(_Button);
    lv_obj_set_width(_LblValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblValue, 0);
    lv_obj_set_y(_LblValue, -25);
    lv_obj_set_align(_LblValue, LV_ALIGN_CENTER);
    lv_label_set_text(_LblValue, "28.8 A");

    _LblPeriphId = lv_label_create(_Button);
    lv_obj_set_width(_LblPeriphId, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriphId, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeriphId, 80);
    lv_obj_set_y(_LblPeriphId, 0);
    lv_obj_set_align(_LblPeriphId, LV_ALIGN_CENTER);
	lv_label_set_text_fmt(_LblPeriphId, "%d", _Periph->GetId());
    lv_obj_set_style_text_color(_LblPeriphId, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(_LblPeriphId, LV_OBJ_FLAG_HIDDEN);

    _LblPos = lv_label_create(_Button);
    lv_obj_set_width(_LblPos, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPos, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPos, 25);
    lv_obj_set_y(_LblPos, 18);
    lv_obj_set_align(_LblPos, LV_ALIGN_CENTER);
    lv_label_set_text_fmt(_LblPos, "%d", _Pos);
    lv_obj_set_style_text_color(_LblPos, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPos, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    _Arc = lv_arc_create(_Button);
    lv_obj_set_width(_Arc, _Width);
    lv_obj_set_height(_Arc, _Height);
    lv_obj_set_align(_Arc, LV_ALIGN_CENTER);
    lv_obj_clear_flag(_Arc, LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_arc_set_value(_Arc, 0);
    lv_obj_set_style_arc_color(_Arc, lv_color_hex(0x4A4A4A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(_Arc, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(_Arc, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(_Arc, lv_color_hex(0x398F30), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(_Arc, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(_Arc, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(_Arc, lv_color_hex(0xDBDBDB), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_Arc, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_Arc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_Arc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_Arc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_Arc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(_Button, _event_cb, LV_EVENT_ALL, NULL);  
}

