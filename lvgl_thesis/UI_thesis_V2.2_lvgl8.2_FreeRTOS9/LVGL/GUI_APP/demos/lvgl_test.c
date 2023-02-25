#include "lvgl_test.h"
#include "lvgl.h"

lv_coord_t scr_act_width, scr_act_height;
lv_obj_t * parent_obj;
lv_obj_t * child_obj;

static void obj_event_cb(lv_event_t * e)  // can sense different events
{
  lv_obj_t * target = lv_event_get_target(e);
  if(target == parent_obj)
  {
      lv_obj_align(parent_obj, LV_ALIGN_CENTER, 0, 0);
  }
  else if(target == child_obj)
   {
     lv_obj_align(child_obj, LV_ALIGN_RIGHT_MID, 100, 0);
   }


 }
void my_event_test3()
{
   scr_act_width = lv_obj_get_width(lv_scr_act());
   scr_act_height = lv_obj_get_height(lv_scr_act());

   parent_obj = lv_obj_create(lv_scr_act());
   lv_obj_set_size(parent_obj, scr_act_width * 2/3, scr_act_height * 2/3);
   lv_obj_align(parent_obj, LV_ALIGN_OUT_TOP_MID, 0, 0);
   lv_obj_set_style_bg_color(parent_obj, lv_color_hex(0x099cef), 0);
   lv_obj_add_event_cb(parent_obj, obj_event_cb, LV_EVENT_LONG_PRESSED, NULL);
//
//
    child_obj = lv_obj_create(parent_obj);
    lv_obj_set_size(child_obj, scr_act_width* 1/3, scr_act_height*1/3);
    lv_obj_align(child_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(child_obj, lv_color_hex(0xe99cef), 0);
    lv_obj_add_event_cb(child_obj, obj_event_cb, LV_EVENT_CLICKED, NULL);
    
	  lv_obj_t *label = lv_label_create(child_obj);
   //method 1
   //lv_label_set_text(label, "hello \n");

   //method 2
   //lv_label_set_text_static(label, "hello hello hello hello hello hello hello hello hello hello hello hello hello"); // becarefull, the memory can be free, when the text in the memory is unreliable
   lv_label_set_text(label, "hello hello 123 456 789 012 456 234"); // becarefull, the memory can be free, when the text in the memory is unreliable
    lv_obj_set_size(label, 100, 50);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
}
