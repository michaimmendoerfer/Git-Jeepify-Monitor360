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
	int _size;
	bool _SpinnerOn;
	bool _ButtonState;

	PeriphClass *_Periph;

	lv_obj_t *_Spinner;
	lv_obj_t *_Button;
	lv_obj_t *_LblPeer;
	lv_obj_t *_LblPeriph;
	lv_obj_t *_LblBrother;


   
    public:
        CompButton();
        void Setup(PeriphClass *P, int x, int y, int size);
        void SpinnerOn();
        void SpinnerOff();
        void SpinnerShow();
        void SpinnerHide();
	bool SpinnerIsActive();
	bool SpinnerIsVisible();

	bool GetButtonState();
	void SetButtonState();
        void Hide();
        void Show();

};

CompButton::CompButton(lv_obj_t * comp_parent, int x, int y, int size, PeriphClass *Periph)
{
    _Button = lv_imgbtn_create(comp_parent);
    if (size == 1)
	{
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1640860301, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_743505413, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_743505413, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_888658411, NULL);
		lv_obj_set_height(_Button, 120);
		lv_obj_set_width (_Button, 70);   /// 1
	}
	else
	{
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_1134846501, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_1528892059, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &ui_img_1528892059, NULL);
		lv_imgbtn_set_src(_Button, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &ui_img_715952573, NULL);
		lv_obj_set_height(_Button, 205);
		lv_obj_set_width (_Button,  120);   /// 2
	}

	lv_obj_set_x(_Button, x);
    	lv_obj_set_y(_Button, y);
    	lv_obj_set_align(_Button, LV_ALIGN_CENTER);
    	lv_obj_add_flag(_Button, LV_OBJ_FLAG_CHECKABLE);     	    /// Flags
    	lv_obj_add_flag(_Button, LV_OBJ_FLAG_OVERFLOW_VISIBLE);     /// Flags


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
    if (PeerOf(Periph)->GetName()) lv_label_set_text_fmt(_LblPeer, "%.6s", PeerOf(Periph)->GetName());
    
ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_color_BtnTxt);
    lv_obj_set_style_radius(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR,
                                           _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(_LblPeer, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA,
                                           _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(_LblPeer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_LblPeer, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_LblPeer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_LblPeer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_t *cui_LblSwitchPeriph = lv_label_create(cui_ButtonSwitchSmall);
    if (!PeriphName) lv_obj_add_flag(cui_LblSwitchPeriph, LV_OBJ_FLAG_HIDDEN);
	if (size == 1)
	{
		lv_obj_set_align(cui_LblSwitchPeriph, LV_ALIGN_BOTTOM_RIGHT);
		lv_obj_set_x(cui_LblSwitchPeriph, -50);
    	lv_obj_set_y(cui_LblSwitchPeriph, -90);
		lv_obj_set_style_text_font(cui_LblSwitchPeriph, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else
	{
		/*lv_obj_set_align(cui_LblSwitchPeriph, LV_ALIGN_CENTER);
		lv_obj_set_x(cui_LblSwitchPeriph, 0);
    	lv_obj_set_y(cui_LblSwitchPeriph, -95);
		lv_obj_set_style_text_font(cui_LblSwitchPeriph, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
		*/
		lv_obj_add_flag(cui_LblSwitchPeriph, LV_OBJ_FLAG_HIDDEN);
	}

	lv_obj_set_width(cui_LblSwitchPeriph, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_LblSwitchPeriph, LV_SIZE_CONTENT);    /// 1
    if (PeriphName) lv_label_set_text_fmt(cui_LblSwitchPeriph, "%.6s", PeriphName);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_BtnTxt);
    lv_obj_set_style_radius(cui_LblSwitchPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR,
                                           _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(cui_LblSwitchPeriph, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA,
                                           _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(cui_LblSwitchPeriph, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_LblSwitchPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_LblSwitchPeriph, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_LblSwitchPeriph, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_LblSwitchPeriph, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_LblSwitchAmp = lv_label_create(cui_ButtonSwitchSmall);
    if (size == 1)
	{
		lv_obj_set_width(cui_LblSwitchAmp, LV_SIZE_CONTENT);
    	lv_obj_set_height(cui_LblSwitchAmp, LV_SIZE_CONTENT);    /// 1
		lv_obj_set_align(cui_LblSwitchAmp, LV_ALIGN_BOTTOM_LEFT);
		lv_obj_set_x(cui_LblSwitchAmp, 50);
    	lv_obj_set_y(cui_LblSwitchAmp, -5);
    	lv_obj_set_style_text_font(cui_LblSwitchAmp, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
	else 
	{
		lv_obj_set_width(cui_LblSwitchAmp, 80);
		lv_obj_set_height(cui_LblSwitchAmp, LV_SIZE_CONTENT);    /// 1
		lv_obj_set_x(cui_LblSwitchAmp, 0);
    	lv_obj_set_y(cui_LblSwitchAmp, 115);
    	lv_obj_set_align(cui_LblSwitchAmp, LV_ALIGN_CENTER);
		lv_obj_set_style_text_font(cui_LblSwitchAmp, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
	}
    lv_label_set_text(cui_LblSwitchAmp, "28.8 A");
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR,
                                           _ui_theme_color_BtnTxt);
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA,
                                           _ui_theme_alpha_BtnTxt);
    lv_obj_set_style_text_align(cui_LblSwitchAmp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_LblSwitchAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR,
                                           _ui_theme_color_BtnBg);
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA,
                                           _ui_theme_alpha_BtnBg);
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR,
                                           _ui_theme_color_BtnBorder);
    ui_object_set_themeable_style_property(cui_LblSwitchAmp, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA,
                                           _ui_theme_alpha_BtnBorder);
    lv_obj_set_style_border_width(cui_LblSwitchAmp, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_LblSwitchAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_LblSwitchAmp, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_LblSwitchAmp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_LblSwitchAmp, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_add_flag(cui_LblSwitchAmp, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t * cui_LblPosition;
    cui_LblPosition = lv_label_create(cui_ButtonSwitchSmall);
    lv_obj_set_width(cui_LblPosition, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_LblPosition, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cui_LblPosition, 20);
    lv_obj_set_y(cui_LblPosition, 0);
    lv_obj_set_align(cui_LblPosition, LV_ALIGN_CENTER);
	lv_label_set_text_fmt(cui_LblPosition, "%d", Pos);
    lv_obj_set_style_text_color(cui_LblPosition, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_LblPosition, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t ** children = (lv_obj_t **) lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_BUTTONSWITCHSMALL_NUM);
    children[UI_COMP_BUTTONSWITCHSMALL_BUTTONSWITCHSMALL] = cui_ButtonSwitchSmall;
    children[UI_COMP_BUTTONSWITCHSMALL_LBLPEER] = cui_LblSwitchPeer;
    children[UI_COMP_BUTTONSWITCHSMALL_LBLPERIPH] = cui_LblSwitchPeriph;
    children[UI_COMP_BUTTONSWITCHSMALL_LBLVALUE] = cui_LblSwitchAmp;
    children[UI_COMP_BUTTONSWITCHSMALL_LBLPOSITION] = cui_LblPosition;
	lv_obj_add_event_cb(cui_ButtonSwitchSmall, get_component_child_event_cb, (lv_event_code_t)LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_ButtonSwitchSmall, del_component_child_event_cb, LV_EVENT_DELETE, children);
    //ui_comp_ButtonSwitchSmall_create_hook(cui_ButtonSwitchSmall);
    return cui_ButtonSwitchSmall;
}

lv_obj_t * ui_ButtonSwitchSpinner_create(lv_obj_t * comp_parent, lv_obj_t *Switch)
{	
lv_obj_t *cui_SpinnerSwitch = lv_spinner_create(comp_parent, 1000, 90);
    lv_obj_set_align(cui_SpinnerSwitch, LV_ALIGN_CENTER);
    lv_obj_set_width(cui_SpinnerSwitch, lv_obj_get_width(Switch)+30);
    lv_obj_set_height(cui_SpinnerSwitch, lv_obj_get_width(Switch)+30);

	Serial.printf("X von Schalter %d", lv_obj_get_x(Switch));
	lv_obj_set_x(cui_SpinnerSwitch, lv_obj_get_x(Switch)-(360-lv_obj_get_width(Switch))/2);
    lv_obj_set_y(cui_SpinnerSwitch, 0);
    lv_obj_set_align(cui_SpinnerSwitch, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_SpinnerSwitch, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_clear_flag(cui_SpinnerSwitch, LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_arc_color(cui_SpinnerSwitch, lv_color_hex(0x83061F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(cui_SpinnerSwitch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(cui_SpinnerSwitch, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(cui_SpinnerSwitch, 5, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_arc_color(cui_SpinnerSwitch, lv_color_hex(0x31020B), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(cui_SpinnerSwitch, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(cui_SpinnerSwitch, 5, LV_PART_INDICATOR | LV_STATE_CHECKED);

	return cui_SpinnerSwitch;
}
