#include "my_gui.h"
#include "lvgl.h"
void my_GUI(void)
{
   /*Create a switch and apply the styles*/
    lv_obj_t* sw1 = lv_switch_create(lv_scr_act());
    lv_obj_set_size(sw1, 80, 60);
    lv_obj_align(sw1,LV_ALIGN_CENTER, 50, -50);

//          /*align test*/
//      lv_obj_t* obj3 = lv_obj_create(lv_scr_act());
//      lv_obj_set_size(obj3, 20, 60);
//      lv_obj_align(obj3,LV_ALIGN_CENTER,10,-50);

//
    lv_obj_t* s1i1 = lv_slider_create(lv_scr_act());
    lv_obj_set_size(s1i1, 20, 90);
    lv_obj_align(s1i1,LV_ALIGN_CENTER, 50, 50);

//      lv_obj_t *obj1 = lv_obj_create(lv_scr_act());
//      lv_obj_set_x(obj1, 100);
//      lv_obj_set_y(obj1, 300);
//      lv_obj_set_size(obj1, 300, 300);
//      lv_obj_set_pos(obj1, 200, 400);
//
//      lv_obj_t *obj2 = lv_obj_create(lv_scr_act());
//      lv_obj_set_pos(obj2, 205, 40);



}

void my_style_test()
{
  static lv_style_t style; // has to be static, otherwise the style will be deleted
  lv_style_init(&style);   // init the style
  lv_style_set_bg_color(&style, lv_color_hex(0x22b23));

  lv_obj_t * style1 = lv_obj_create(lv_scr_act());
  lv_obj_add_style(style1, &style, LV_STATE_CHECKED);
  lv_obj_align(style1, LV_ALIGN_CENTER, 50, 50);

  lv_obj_t * obj1 = lv_obj_create(lv_scr_act());
  lv_obj_set_style_bg_color(obj1, lv_color_hex(0x9dddd),LV_STATE_PRESSED);


}

void my_style_test1()
{
  lv_obj_t * obj1 = lv_obj_create(lv_scr_act());
  lv_obj_set_align(obj1, LV_ALIGN_OUT_TOP_MID);

  lv_obj_set_style_border_color(obj1, lv_color_hex(0x2ee2344), LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(obj1, 10, LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(obj1, 90, LV_STATE_DEFAULT);//transparency of border, the value can be 0-255, high less tranparency

  lv_obj_set_style_outline_color(obj1, lv_color_hex(0x7662344), LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(obj1, 10, LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(obj1, 90, LV_STATE_DEFAULT);//transparency of border, the value can be 0-255, high less tranparency
}


void my_style_test2()
{
    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_align(slider, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(slider, lv_color_hex(0x34e73e), LV_STATE_DEFAULT|LV_PART_INDICATOR); // selection "|" will disable some funcs
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x64e73e), LV_STATE_DEFAULT|LV_PART_KNOB); // selection "|" will disable some funcs
}

//static void my_event_cb(lv_event_t * e)
//{
//
// lv_event_code_t code = lv_event_get_code(e); // get event type
// if(code == LV_EVENT_CLICKED)
//   {
//       printf("lv_event_clicked \r\n");
//   }
// else if(code == LV_EVENT_LONG_PRESSED)
//   {
//       printf("event has long pressed \r\n");
//   }
// }

lv_obj_t * obj1_test2;
lv_obj_t * obj2_test2;
static void my_event_cb(lv_event_t * e)  // can sense different events
{
  lv_obj_t * target = lv_event_get_target(e);
  if(target == obj1_test2)
  {
      printf("obj1 \n");
  }
  else if(target = obj2_test2)
   {
        printf("obj2 \n");
   }
}


void my_event_test2()
{
     obj1_test2 = lv_obj_create(lv_scr_act());
     lv_obj_add_event_cb(obj1_test2, my_event_cb, LV_EVENT_CLICKED, NULL);
      //lv_obj_add_event_cb(obj1, my_event_cb, LV_EVENT_LONG_PRESSED, NULL);
     lv_obj_set_size(obj1_test2, 300, 200);
     lv_obj_align(obj1_test2, LV_ALIGN_DEFAULT, 0, 0);
      lv_obj_set_style_bg_color(obj1_test2, lv_color_hex(0x34e73e), LV_STATE_DEFAULT); // selection "|" will disable some funcs

      obj2_test2 = lv_obj_create(lv_scr_act());
      lv_obj_add_event_cb(obj2_test2, my_event_cb, LV_EVENT_LONG_PRESSED, NULL);
      lv_obj_set_size(obj2_test2, 300, 200);
      lv_obj_align(obj2_test2, LV_ALIGN_CENTER, 100, 100);
      lv_obj_set_style_bg_color(obj2_test2, lv_color_hex(0xe4e73e), LV_STATE_DEFAULT); // selection "|" will disable some funcs
}

//void my_event_test1()
//{
//     lv_obj_t * obj1 = lv_obj_create(lv_scr_act());
//     lv_obj_add_event_cb(obj1, my_event_cb, LV_EVENT_CLICKED, NULL);
//      //lv_obj_add_event_cb(obj1, my_event_cb, LV_EVENT_LONG_PRESSED, NULL);
//      lv_obj_set_size(obj1. 300, 200);
//
//      lv_obj_t * obj2 = lv_obj_create(lv_scr_act());
//      lv_obj_add_event_cb(obj2, my_event_cb, LV_EVENT_LONG_PRESSED, NULL);
//      lv_obj_set_size(obj2, 200, 300);
//}


static lv_coord_t scr_act_width, scr_act_height;
static lv_obj_t * parent_obj;
static lv_obj_t * child_obj;
//
//static void obj_event_cb(lv_event_t * e)  // can sense different events
//{
//  lv_obj_t * target = lv_event_get_target(e);
//  if(target == parent_obj)
//  {
//      // printf("obj1 \n");
//      printf("long pressed\n");
//      lv_obj_align(parent_obj, LV_ALIGN_CENTER, 0, 0);
//  }
//  else if(target == child_obj)
//   {
//     lv_obj_align(child_obj, LV_ALIGN_RIGHT_MID, 100, 0);
//   }
//
//
// }
//


 static void obj_event_cb(lv_event_t *e)
{
    lv_obj_t * target = lv_event_get_target(e);                                 /* 获取事件触发源 */

    if (target == parent_obj)                                                   /* 判断触发源：是不是父对象？ */
    {
        lv_obj_align(parent_obj, LV_ALIGN_CENTER, 0, 0);                        /* 重新设置父对象的位置：居中 */
    }
    else if (target == child_obj)                                               /* 判断触发源：是不是子对象？ */
    {
        lv_obj_align(child_obj, LV_ALIGN_RIGHT_MID, 100, 0);                    /* 重新设置子对象的位置：右侧居中，再向X轴偏移100 */
    }
}


void my_event_test3()
{
    scr_act_width = lv_obj_get_width(lv_scr_act());
    scr_act_height = lv_obj_get_height(lv_scr_act());

    parent_obj = lv_obj_create(lv_scr_act());
   lv_obj_set_size(parent_obj, scr_act_width * 2/3, scr_act_height * 2/3);
   lv_obj_align(parent_obj, LV_ALIGN_TOP_MID, 0, 0);
   lv_obj_set_style_bg_color(parent_obj, lv_color_hex(0x099cef), 0);
   lv_obj_add_event_cb(parent_obj, obj_event_cb, LV_EVENT_LONG_PRESSED, NULL);
//
//
    child_obj = lv_obj_create(parent_obj);
    lv_obj_set_size(child_obj, scr_act_width* 1/3, scr_act_height*1/3);
    lv_obj_align(child_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(child_obj, lv_color_hex(0xe99cef), 0);
    lv_obj_add_event_cb(child_obj, obj_event_cb, LV_EVENT_CLICKED, NULL);

}

// same color for all text
void my_label1()
{
   lv_obj_t *label = lv_label_create(lv_scr_act());
   //method 1
   //lv_label_set_text(label, "hello \n");

   //method 2
   //lv_label_set_text_static(label, "hello hello hello hello hello hello hello hello hello hello hello hello hello"); // becarefull, the memory can be free, when the text in the memory is unreliable
   lv_label_set_text(label, "hello hello 123 456 789 012 456 234"); // becarefull, the memory can be free, when the text in the memory is unreliable
    lv_obj_set_size(label, 100, 50);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
   //method 3
   //lv_label_set_text_fmt(label, "value:%d", 50);
//   lv_obj_set_style_bg_color(label, lv_color_hex(0x34325), LV_STATE_DEFAULT);  // set background color
//   lv_obj_set_style_bg_opa(label, 100, LV_STATE_DEFAULT);  // set the transparecy
//
//   lv_obj_set_style_text_font(label, &lv_font_montserrat_30, LV_STATE_DEFAULT);
//   lv_obj_set_style_text_color(label, lv_color_hex(0x334365), LV_STATE_DEFAULT);


}

//different for different text
void my_label2()
{
   lv_obj_t *label = lv_label_create(lv_scr_act());
   //method 1
   //lv_label_set_text(label, "hello \n");

   //method 2
   //lv_label_set_text_static(label, "hello"); // becarefull, the memory can be free, when the text in the memory is unreliable

   //method 3
//   lv_label_set_text_fmt(label, "value:%d", 50);
//   lv_obj_set_style_bg_color(label, lv_color_hex(0x34325), LV_STATE_DEFAULT);  // set background color
//   lv_obj_set_style_bg_opa(label, 100, LV_STATE_DEFAULT);  // set the transparecy
//
//   lv_obj_set_style_text_font(label, &lv_font_montserrat_30, LV_STATE_DEFAULT);
//   lv_obj_set_style_text_color(label, lv_color_hex(0x334365), LV_STATE_DEFAULT);

   //set color of some characters
   lv_label_set_recolor(label, true);
   lv_label_set_text(label,"hello #ff0000 lvgl#");
}



