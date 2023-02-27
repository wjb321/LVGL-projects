

#include "lv_checkbox.h"


#include "lvgl.h"
#include <stdio.h>

lv_obj_t *checkbox ;

static void event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED)
    {
      if(lv_obj_has_state(checkbox, LV_STATE_CHECKED))
      {
        printf("on\n");
      }
    else
     {
        printf("off\n");
      }
    }
}


void lv_checkbox_test1()
{
   checkbox = lv_checkbox_create(lv_scr_act());
   lv_obj_set_align(checkbox, LV_ALIGN_CENTER);
   lv_checkbox_set_text(checkbox, "input password");
   lv_obj_add_state(checkbox, LV_STATE_CHECKED|LV_STATE_DISABLED);
   lv_obj_set_style_pad_column(checkbox, 50, LV_STATE_DEFAULT);
   lv_obj_clear_state(checkbox, LV_STATE_CHECKED|LV_STATE_DISABLED);

   lv_obj_set_size(checkbox,200, 50);
   lv_obj_add_event_cb(checkbox, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
   lv_label_set_long_mode(checkbox, LV_LABEL_LONG_SCROLL_CIRCULAR);



}




