#include <Arduino.h>
#include "ui.h"
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

	float *_ValueToFollow;

	PeriphClass *_Periph;

	lv_obj_t *_Spinner;
	lv_obj_t *_Button;
	lv_obj_t *_LblPeer;
	lv_obj_t *_LblPeriph;
	lv_obj_t *_LblPos;
	lv_obj_t *_LblAmp;

	lv_obj_t *_FollowTimer;

    public:
        CompButton::CompButton(lv_obj_t * comp_parent, int x, int y, int size, PeriphClass *Periph);
	CompButton::CompButton(lv_obj_t * comp_parent, int x, int y, int size, PeriphClass *Periph, float *ValueToFollow);
	~CompButton();
        
        void SpinnerOn() 		{ lv_obj_add_flag  (_Spinner, LV_OBJ_FLAG_HIDDEN); }
        void SpinnerOff()		{ lv_obj_clear_flag(_Spinner, LV_OBJ_FLAG_HIDDEN); }
        
	bool SpinnerIsVisible() 	{ return !lv_obj_has_flag_any(_Spinner, LV_OBJ_FLAG_HIDDEN); }

	bool GetButtonState()   	{ if (lv_obj_has_state(_Button, LV_STATE_CHECKED)) return true; else return false; }
	void SetButtonState(bool State) { if (State) lv_imagebutton_set_state(_Button, LV_IMAGEBUTTON_STATE_CHECKED); else lv_imagebutton_set_state(_Button, LV_IMAGEBUTTON_STATE_RELEASED);
					 
        void Hide();
        void Show();

	void Follow(float *Value);

};

CompButton::CompButton(lv_obj_t * comp_parent, int x, int y, int size, PeriphClass *Periph)
{
    _SpinnerOn = false;
    _Periph = Periph;
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
	lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1640860301, NULL);
	lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_743505413, NULL);
	lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_743505413, NULL);
	lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_888658411, NULL);
    }

    lv_obj_set_align(_Button, LV_ALIGN_CENTER);
    lv_obj_set_height(_Button, _Height);
    lv_obj_set_width (_Button, _Width);   /// 1
    lv_obj_set_x(_Button, _x);
    lv_obj_set_y(_Button, _y);
    
    lv_obj_add_flag(_Button, LV_OBJ_FLAG_CHECKABLE);     	   
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
	    /*lv_obj_set_x(_LblPeer, 0);
    	    lv_obj_set_y(_LblPeer, -65);
	    lv_obj_set_style_text_font(_LblPeer, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    	    lv_obj_set_align(_LblPeer, LV_ALIGN_CENTER);
	    */
	    lv_obj_add_flag(_LblPeer, LV_OBJ_FLAG_HIDDEN);
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
		/*lv_obj_set_align(_LblPeriph, LV_ALIGN_CENTER);
		lv_obj_set_x(_LblPeriph, 0);
    	lv_obj_set_y(_LblPeriph, -95);
		lv_obj_set_style_text_font(_LblPeriph, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
		*/
		lv_obj_add_flag(_LblPeriph, LV_OBJ_FLAG_HIDDEN);
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
    lv_obj_set_style_pad_bottom(v, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    _LblAmp = lv_label_create(_Button);
    if (size == 1)
	{
		lv_obj_set_width(_LblAmp, LV_SIZE_CONTENT);
    	lv_obj_set_height(_LblAmp, LV_SIZE_CONTENT);    /// 1
		lv_obj_set_align(_LblAmp, LV_ALIGN_BOTTOM_LEFT);
		lv_obj_set_x(_LblAmp, 50);
    	lv_obj_set_y(_LblAmp, -5);
    	lv_obj_set_style_text_font(_LblAmp, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else 
	{
		lv_obj_set_width(_LblAmp, 80);
		lv_obj_set_height(_LblAmp, LV_SIZE_CONTENT);    /// 2
		lv_obj_set_x(_LblAmp, 0);
    	lv_obj_set_y(_LblAmp, 115);
    	lv_obj_set_align(v, LV_ALIGN_CENTER);
		lv_obj_set_style_text_font(_LblAmp, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
    lv_label_set_text(_LblAmp, "28.8 A");
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_BtnTxt);
    lv_obj_set_style_text_align(_LblAmp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(_LblAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(_LblAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(_LblAmp, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_LblAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_LblAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_LblAmp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_LblAmp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_add_flag(_LblAmp, LV_OBJ_FLAG_HIDDEN);

    _LblPos = lv_label_create(_Button);
    lv_obj_set_width(_LblPos, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(_LblPos, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(_LblPos, 20);
    lv_obj_set_y(_LblPos, 0);
    lv_obj_set_align(_LblPos, LV_ALIGN_CENTER);
	lv_label_set_text_fmt(_LblPos, "%d", Pos);
    lv_obj_set_style_text_color(_LblPos, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(_LblPos, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(cui_ButtonSwitchSmall, get_component_child_event_cb, (lv_event_code_t)LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_ButtonSwitchSmall, del_component_child_event_cb, LV_EVENT_DELETE, children);
}
CompButton::CompButton(lv_obj_t * comp_parent, int x, int y, int size, PeriphClass *Periph, float *ValueToFollow);
{
    _ValueToFollow = ValueToFollow;
    _FollowTimer = newTimer...
	    
}

CompButton::~CompButton()
{
    if (_Spinner) l_obj_del(_Spinner);
    if (_Button)  l_obj_del(_Button);
}

