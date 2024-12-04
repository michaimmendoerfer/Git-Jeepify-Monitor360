// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQ-Jeepify-Monitor360

#include "../ui.h"

void ui_ScrMulti_screen_init(void)
{
    ui_ScrMulti = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScrMulti, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScrMulti, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrMulti, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_ScrMulti, &ui_img_jeepifybackground_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScrMulti, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LblMultiScreenName = lv_label_create(ui_ScrMulti);
    lv_obj_set_width(ui_LblMultiScreenName, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LblMultiScreenName, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LblMultiScreenName, 0);
    lv_obj_set_y(ui_LblMultiScreenName, -148);
    lv_obj_set_align(ui_LblMultiScreenName, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LblMultiScreenName, "Screen");
    lv_obj_set_style_text_color(ui_LblMultiScreenName, lv_color_hex(0xDBDBDB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LblMultiScreenName, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonMulti1 = lv_btn_create(ui_ScrMulti);
    lv_obj_set_width(ui_ButtonMulti1, lv_pct(30));
    lv_obj_set_height(ui_ButtonMulti1, lv_pct(30));
    lv_obj_set_x(ui_ButtonMulti1, lv_pct(-18));
    lv_obj_set_y(ui_ButtonMulti1, lv_pct(-18));
    lv_obj_set_align(ui_ButtonMulti1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonMulti1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonMulti1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_ButtonMulti1, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ButtonMulti1, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonMulti1, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_ButtonMulti1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_ButtonMulti1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6 = lv_label_create(ui_ButtonMulti1);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "+");
    lv_obj_set_style_text_font(ui_Label6, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonMulti2 = lv_btn_create(ui_ScrMulti);
    lv_obj_set_width(ui_ButtonMulti2, lv_pct(30));
    lv_obj_set_height(ui_ButtonMulti2, lv_pct(30));
    lv_obj_set_x(ui_ButtonMulti2, lv_pct(18));
    lv_obj_set_y(ui_ButtonMulti2, lv_pct(-18));
    lv_obj_set_align(ui_ButtonMulti2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonMulti2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonMulti2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_ButtonMulti2, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ButtonMulti2, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonMulti2, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_ButtonMulti2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_ButtonMulti2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_ButtonMulti2);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "+");
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonMulti3 = lv_btn_create(ui_ScrMulti);
    lv_obj_set_width(ui_ButtonMulti3, lv_pct(30));
    lv_obj_set_height(ui_ButtonMulti3, lv_pct(30));
    lv_obj_set_x(ui_ButtonMulti3, lv_pct(-18));
    lv_obj_set_y(ui_ButtonMulti3, lv_pct(18));
    lv_obj_set_align(ui_ButtonMulti3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonMulti3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonMulti3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_ButtonMulti3, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ButtonMulti3, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonMulti3, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_ButtonMulti3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_ButtonMulti3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(ui_ButtonMulti3);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "+");
    lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ButtonMulti4 = lv_btn_create(ui_ScrMulti);
    lv_obj_set_width(ui_ButtonMulti4, lv_pct(30));
    lv_obj_set_height(ui_ButtonMulti4, lv_pct(30));
    lv_obj_set_x(ui_ButtonMulti4, lv_pct(18));
    lv_obj_set_y(ui_ButtonMulti4, lv_pct(18));
    lv_obj_set_align(ui_ButtonMulti4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ButtonMulti4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ButtonMulti4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_ButtonMulti4, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ButtonMulti4, lv_color_hex(0xAD0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ButtonMulti4, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_ButtonMulti4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_ButtonMulti4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label4 = lv_label_create(ui_ButtonMulti4);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "+");
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ButtonMulti1, ui_event_ButtonMulti1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ButtonMulti2, ui_event_ButtonMulti2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ButtonMulti3, ui_event_ButtonMulti3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ButtonMulti4, ui_event_ButtonMulti4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScrMulti, ui_event_ScrMulti, LV_EVENT_ALL, NULL);

}
