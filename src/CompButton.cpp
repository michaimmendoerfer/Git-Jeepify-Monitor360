#include <Arduino.h>
#include "Ui/ui.h"
#include "PeerClass.h"
#include "pref_manager.h"
#include "LinkedList.h"
#include "Jeepify.h"
#include "main.h"
#include "CompButton.h"

//#define DEBUG 1
#define COMPBUTTON_WIDTH_1	70
#define COMPBUTOON_HEIGHT_1	120
#define COMPBUTTON_WIDTH_2	120
#define COMPBUTOON_HEIGHT_2	205

#define COMPSENSOR_WIDTH_1	100
#define COMPSENSOR_HEUGHT_1	100

#define COMPMETER_WIDTH_1	100
#define COMPMETER_HEIGHT_1	100
#define COMPMETER_WIDTH_2	240
#define COMPMETER_HEIGHT_2	240
#define COMPMETER_WIDTH_3	360
#define COMPMETER_HEIGHT_3	360
 
CompThing::CompThing()
{
}
CompButton::CompButton()
{
    _ClassType = 1;
}
CompButton::~CompButton()
{
    Hide();
    
    lv_obj_remove_event_cb(_Button, _event_cb);

    Serial.println("CompButton Destructor");
    if (_Spinner) { lv_obj_del(_Spinner); _Spinner = NULL; }
    Serial.println("Spinner weg");
    
    if (_Button)  { lv_obj_del(_Button); _Button = NULL; }
    Serial.println("Button weg");
}
void CompButton::Setup(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{
    _Periph = Periph;
    _event_cb = event_cb;
    _ShowLabels = ShowLabels;
    _Pos = Pos;
   
    _x = x;
    _y = y;			
	
    switch (size) 
    {
	case 1: _Width  = COMPBUTTON_WIDTH_1; _Height = COMPBUTOON_HEIGHT_1; break;
    	cas2 2: _Width  = COMPBUTTON_WIDTH_2; _Height = COMPBUTOON_HEIGHT_2; break;
    }
	
    _Spinner = lv_spinner_create(comp_parent, 1000, 90);
	
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
    	if (!PeerOf(Periph)->GetName()) 
	{
	    lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
	    lv_label_set_text_fmt(_LblPeer, "%.6s", PeerOf(Periph)->GetName());
	}

	lv_obj_set_width (_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeer, LV_SIZE_CONTENT);    /// 1
    	
	if (size == 1)
	{
	    lv_obj_set_align(_LblPeer, LV_ALIGN_BOTTOM_RIGHT);
	    lv_obj_set_x(_LblPeer, -50);
    	lv_obj_set_y(_LblPeer,  -5);
	    lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else
	{
	    lv_obj_set_align(_LblPeer, LV_ALIGN_CENTER);
        lv_obj_set_x(_LblPeer, 0);
    	lv_obj_set_y(_LblPeer, -115);
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
    if (!_ShowLabels) lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t *_LblPeriph = lv_label_create(_Button);
    if (!_Periph->GetName()) lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);
    else lv_label_set_text_fmt(_LblPeriph, "%.6s", _Periph->GetName());
	
	if (size == 1)
	{
		lv_obj_set_align(_LblPeriph, LV_ALIGN_BOTTOM_RIGHT);
		lv_obj_set_x(_LblPeriph, -50);
    	lv_obj_set_y(_LblPeriph, -90);
		lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else
	{
		lv_obj_set_align(_LblPeriph, LV_ALIGN_CENTER);
		lv_obj_set_x(_LblPeriph, 0);
    	lv_obj_set_y(_LblPeriph, -145);
		lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
	}

	lv_obj_set_width(_LblPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriph, LV_SIZE_CONTENT);    /// 1
    
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
    if (!_ShowLabels) lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);

    _LblValue = lv_label_create(_Button);
    if (size == 1)
	{
		lv_obj_set_width(_LblValue, LV_SIZE_CONTENT);
    	lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);    /// 1
		lv_obj_set_align(_LblValue, LV_ALIGN_BOTTOM_LEFT);
		lv_obj_set_x(_LblValue, 50);
    	lv_obj_set_y(_LblValue, -5);
    	lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else 
	{
		lv_obj_set_width(_LblValue, 80);
		lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);    /// 2
		lv_obj_set_x(_LblValue, 0);
    	lv_obj_set_y(_LblValue, 125);
    	lv_obj_set_align(_LblValue, LV_ALIGN_CENTER);
		lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
    lv_label_set_text(_LblValue, "28.8 A");
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
	
    if (!_ShowLabels) lv_obj_add_flag(_LblValue, LV_OBJ_FLAG_HIDDEN);

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
    lv_obj_set_x(_LblPos, 80);
    lv_obj_set_y(_LblPos,-15);
    lv_obj_set_align(_LblPos, LV_ALIGN_CENTER);
	lv_label_set_text_fmt(_LblPos, "%d", _Pos);
    lv_obj_set_style_text_color(_LblPos, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(_LblPos, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(_Button, _event_cb, LV_EVENT_ALL, NULL);  
	
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

    switch (size) 
    {
	case 1: _Width  = COMPSENSOR_WIDTH_1; _Height = COMPSENSOR_HEIGHT_1; break;
    }
	
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

CompMeter::CompMeter()
{
}
CompMeter::CompMeter()
{
    _ClassType = 3;
}
CompMeter::~CompMeter()
{
    Hide();
    
    lv_obj_remove_event_cb(_Button, _event_cb);
    lv_obj_remove_event_cb(_Meter, SingleMeter_cb);
	
    Serial.println("CompMeter Destructor");
    if (_Meter) { lv_obj_del(_Meter); _Meter = NULL; }
    Serial.println("Meter weg");
}

void CompMeter::SetupModern(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{
    _Periph = Periph;
    _event_cb = event_cb;
    _ShowLabels = ShowLabels;
    _Pos = Pos;
   
    _x = x;
    _y = y;		

    switch (size) {
	case 1: _Width  = COMPMETER_WIDTH_1; _Height = COMPMETER_HEIGHT_1; break
	case 2: _Width  = COMPMETER_WIDTH_2; _Height = COMPMETER_HEIGHT_2; break
	case 3: _Width  = COMPMETER_WIDTH_3; _Height = COMPMETER_HEIGHT_3; break
		}	
	if (Meter)
	{
		lv_obj_del(Meter);
		Meter = NULL;
	}
		
	_Meter = lv_meter_create(comp_parent);
	lv_obj_center(_Meter);
	lv_obj_set_style_bg_color(_Meter, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(_Meter, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_size(_Meter, _x, _y);
	_Scale = lv_meter_add_scale(_Meter);
	
	lv_obj_move_background(_Meter);
	lv_obj_set_style_text_color(_Meter, lv_color_hex(0xdbdbdb), LV_PART_TICKS);
	
	_IndicNeedle = lv_meter_add_needle_line(_Meter, _Scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

	
	if ((_Periph) and (_Periph->GetType() == SENS_TYPE_AMP))
	{
		lv_meter_set_scale_ticks(_Meter, _Scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(_Meter, _Scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(_Meter, _Scale, 0, 400, 240, 150);
	
		//Add a green arc to the start
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 0);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 250);

		_Indic = lv_meter_add_arc(_Meter, _Scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 300);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 400);

		//Make the tick lines red at the end of the scale
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(_Meter, _Indic, 300);
		lv_meter_set_indicator_end_value(_Meter, _Indic, 400);

		lv_obj_add_event_cb(_Meter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
		lv_obj_add_event_cb(_Meter, _event_cb, LV_EVENT_CLICKED, NULL);
	}
	else if ((_Periph) and (_Periph->GetType() == SENS_TYPE_VOLT))
	{	
		lv_meter_set_scale_ticks(_Meter, _Scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(_Meter, _Scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(_Meter, _Scale, 90, 150, 240, 150);
	
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 90);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 112);
		
		//Add a green arc to the start
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 112);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 144);

		_Indic = lv_meter_add_arc(_Meter, _Scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 144);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 150);

		//Make the tick lines red at the end of the scale
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(_Meter, _Indic, 144);
		lv_meter_set_indicator_end_value(_Meter, _Indic, 150);

		//Add draw callback to override default values
		lv_obj_add_event_cb(_Meter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
		lv_obj_add_event_cb(_Meter, _event_cb, LV_EVENT_CLICKED, NULL);
	}

	_LblPeer = lv_label_create(_Meter);
    lv_obj_set_width(_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeer, 0);
    lv_obj_set_y(_LblPeer, lv_pct(30));
    lv_obj_set_align(_LblPeer, LV_ALIGN_CENTER);
    lv_label_set_text(_LblPeer, "Peer");
    lv_obj_set_style_text_color(_LblPeer, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPeer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

	_LblPeriph = lv_label_create(_Meter);
    lv_obj_set_width(_LblPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriph, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeriph, 0);
    lv_obj_set_y(_LblPeriph, lv_pct(-20));
    lv_obj_set_align(_LblPeriph, LV_ALIGN_CENTER);
    lv_label_set_text(_LblPeriph, "Periph");
    lv_obj_set_style_text_color(_LblPeriph, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPeriph, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
		
    _LblValue = lv_label_create(_Meter);
    lv_obj_set_width(_LblValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblValue, 0);
    lv_obj_set_y(_LblValue, lv_pct(15));
    lv_obj_set_align(_LblValue, LV_ALIGN_CENTER);
    lv_label_set_text(_LblValue, "Value");
    lv_obj_set_style_text_color(_LblValue, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblValue, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

void CompMeter::SetupVintage(lv_obj_t * comp_parent, int x, int y, int Pos, int size, bool ShowLabels, PeriphClass *Periph, lv_event_cb_t event_cb)
{
    _Periph = Periph;
    _event_cb = event_cb;
    _ShowLabels = ShowLabels;
    _Pos = Pos;
   
    _x = x;
    _y = y;		

    switch (size) {
	case 1: _Width  = 100; _Height = 100; break
	case 2: _Width  = 240; _Height = 240; break
	case 3: _Width  = 360; _Height = 360; break
    }	
	if (Meter)
	{
		lv_obj_del(Meter);
		Meter = NULL;
	}
		
	_Meter = lv_meter_create(comp_parent);
	lv_obj_center(_Meter);
	lv_obj_set_style_bg_color(_Meter, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(_Meter, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_size(_Meter, 235,	235);
	_Scale = lv_meter_add_scale(_Meter);
	
	//lv_obj_move_background(_Meter);
	lv_obj_set_style_text_color(_Meter, lv_color_hex(0x000000), LV_PART_TICKS);
	
	_IndicNeedle = lv_meter_add_needle_line(_Meter, _Scale, 4, lv_color_hex(0x444444), -10);
	
	if ((_Periph) and (_Periph->GetType() == SENS_TYPE_AMP))
	{
		lv_meter_set_scale_ticks(_Meter, _Scale, 41, 3, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(_Meter, _Scale, 5, 4, 15, lv_color_black(), 15);
    	lv_meter_set_scale_range(_Meter, _Scale, 0, 400, 240, 150);
	
		//Add a green arc to the start
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 0);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 250);

		_Indic = lv_meter_add_arc(_Meter, _Scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 300);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 400);

		//Make the tick lines red at the end of the scale
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(_Meter, _Indic, 300);
		lv_meter_set_indicator_end_value(_Meter, _Indic, 400);

		lv_obj_add_event_cb(_Meter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
		lv_obj_add_event_cb(_Meter, _event_cb, LV_EVENT_CLICKED, NULL);
	}
	else if ((_Periph) and (_Periph->GetType() == SENS_TYPE_VOLT))
	{	
		lv_meter_set_scale_ticks(_Meter, _Scale, 31, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    	lv_meter_set_scale_major_ticks(_Meter, _Scale, 5, 4, 20, lv_color_black(), 20);
    	lv_meter_set_scale_range(_Meter, _Scale, 90, 150, 240, 150);
	
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 90);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 112);
		
		//Add a green arc to the start
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 112);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 144);

		_Indic = lv_meter_add_arc(_Meter, _Scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    	lv_meter_set_indicator_start_value(_Meter, _Indic, 144);
    	lv_meter_set_indicator_end_value(_Meter, _Indic, 150);

		//Make the tick lines red at the end of the scale
		_Indic = lv_meter_add_scale_lines(_Meter, _Scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
		lv_meter_set_indicator_start_value(_Meter, _Indic, 144);
		lv_meter_set_indicator_end_value(_Meter, _Indic, 150);

		//Add draw callback to override default values
		lv_obj_add_event_cb(_Meter, SingleMeter_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
		lv_obj_add_event_cb(_Meter, _event_cb, LV_EVENT_CLICKED, NULL);
	}

	_LblPeer = lv_label_create(_Meter);
    lv_obj_set_width(_LblPeer, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeer, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeer, 0);
    lv_obj_set_y(_LblPeer, lv_pct(30));
    lv_obj_set_align(_LblPeer, LV_ALIGN_CENTER);
    lv_label_set_text(_LblPeer, "Peer");
    lv_obj_set_style_text_color(_LblPeer, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPeer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

	_LblPeriph = lv_label_create(_Meter);
    lv_obj_set_width(_LblPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPeriph, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPeriph, 0);
    lv_obj_set_y(_LblPeriph, lv_pct(-20));
    lv_obj_set_align(_LblPeriph, LV_ALIGN_CENTER);
    lv_label_set_text(_LblPeriph, "Periph");
    lv_obj_set_style_text_color(_LblPeriph, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPeriph, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
		
    _LblValue = lv_label_create(_Meter);
    lv_obj_set_width(_LblValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblValue, 0);
    lv_obj_set_y(_LblValue, lv_pct(15));
    lv_obj_set_align(_LblValue, LV_ALIGN_CENTER);
    lv_label_set_text(_LblValue, "Value");
    lv_obj_set_style_text_color(_LblValue, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblValue, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_LblValue, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void CompMeter::Update()
{
	char buf[10];
	int nk = 0;
	float value;
	
	value = _Periph->GetValue();
	
	if (abs(value) < SCHWELLE) value = 0;

	if      (value<10)  nk = 2;
	else if (value<100) nk = 1;
	else                nk = 0;

	if (value == -99) strcpy(buf, "--"); 
	else dtostrf(value, 0, nk, buf);

	if (_Periph->GetType() == SENS_TYPE_AMP)  strcat(buf, " A");
	if (_Periph->GetType() == SENS_TYPE_VOLT) strcat(buf, " V");

	SetNeedle(value*10);
	SetValue(buf);

static void SingleMeter_cb(lv_event_t * e) {

	lv_obj_draw_part_dsc_t	*dsc  = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);
	double					value;

	if( dsc->text != NULL ) {		// Filter major ticks...
		value = dsc->value / 10;
		snprintf(dsc->text, sizeof(dsc->text), "%5.1f", value);
	}

}

