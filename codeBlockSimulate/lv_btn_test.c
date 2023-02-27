
#include "lv_btn_test.h"
#include "lvgl.h"
#include <stdio.h>


static void event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        printf("value is changed \n");
    }

}

void lv_btn_start()
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());

    lv_obj_set_size(btn, 100, 50);
    lv_obj_set_align(btn, LV_ALIGN_CENTER);
//    lv_obj_set_style_bg_color(btn, lv_color_hex(0x34ef33), LV_STATE_PRESSED);

    //开启状态切换，如果屏蔽掉，下面事件的回调就不能再工作了
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);//按下去颜色会发生变化
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_VALUE_CHANGED, NULL);//定义回调函数
}



static const lv_font_t* font;       /* 定义字体 */

static lv_obj_t *label_speed;       /* 速度提示文本 */
static lv_obj_t *btn_speed_up;      /* 加速按钮 */
static lv_obj_t *btn_speed_down;    /* 减速按钮 */
static lv_obj_t *btn_stop;          /* 急停按钮 */

static int32_t speed_val = 0;       /* 速度值 */

/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())


/**
 * @brief  按钮回调
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
static void btn_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* 获取触发源 */

    if(target == btn_speed_up)                      /* 加速按钮 */
    {
        speed_val += 30;
    }
    else if(target == btn_speed_down)               /* 减速按钮 */
    {
        speed_val -= 30;
    }
    else if(target == btn_stop)                     /* 急停按钮 */
    {
        speed_val = 0;
    }
    lv_label_set_text_fmt(label_speed, "Speed : %d RPM", speed_val);    /* 更新速度值 */
}


/**
 * @brief  速度值提示标签
 * @param  无
 * @return 无
 */
static void lv_example_label(void)
{
    /* 根据活动屏幕宽度选择字体 */
    if (scr_act_width() <= 320)
    {
        font = &lv_font_montserrat_10;
    }
    else if (scr_act_width() <= 480)
    {
        font = &lv_font_montserrat_14;
    }
    else
    {
        font = &lv_font_montserrat_20;
    }

    label_speed = lv_label_create(lv_scr_act());                                    /* 创建速度显示标签 */
    lv_obj_set_style_text_font(label_speed, font, LV_PART_MAIN);                    /* 设置字体 */
    lv_label_set_text(label_speed, "Speed : 0 RPM");                                /* 设置文本 */
    lv_obj_align(label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);           /* 设置标签位置 */
}
/**
 * @brief  加速按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_up(void)
{
    btn_speed_up = lv_btn_create(lv_scr_act());                                     /* 创建加速按钮 */
    lv_obj_set_size(btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);       /* 设置按钮大小 */
    lv_obj_align(btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);           /* 设置按钮位置 */
    lv_obj_add_event_cb(btn_speed_up, btn_event_cb, LV_EVENT_CLICKED, NULL);        /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_speed_up);                                /* 创建加速按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Speed +");                                            /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

/**
 * @brief  减速按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_down(void)
{
    btn_speed_down = lv_btn_create(lv_scr_act());                                   /* 创建加速按钮 */
    lv_obj_set_size(btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);     /* 设置按钮大小 */
    lv_obj_align(btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            /* 设置按钮位置 */
    lv_obj_add_event_cb(btn_speed_down, btn_event_cb, LV_EVENT_CLICKED, NULL);      /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_speed_down);                              /* 创建减速按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Speed -");                                            /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

/**
 * @brief  急停按钮
 * @param  无
 * @return 无
 */
static void lv_example_btn_stop(void)
{
    btn_stop = lv_btn_create(lv_scr_act());                                         /* 创建急停按钮 */
    lv_obj_set_size(btn_stop, scr_act_width() / 4, scr_act_height() / 6);           /* 设置按钮大小 */
    lv_obj_align(btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                /* 设置按钮位置 */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  /* 设置按钮背景颜色（默认） */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  /* 设置按钮背景颜色（按下） */
    lv_obj_add_event_cb(btn_stop, btn_event_cb, LV_EVENT_CLICKED, NULL);            /* 设置按钮事件 */

    lv_obj_t* label = lv_label_create(btn_stop);                                    /* 创建急停按钮标签 */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* 设置字体 */
    lv_label_set_text(label, "Stop");                                               /* 设置标签文本 */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* 设置标签位置 */
}

/**
 * @brief  LVGL演示
 * @param  无
 * @return 无
 */
void lv_btn_test2(void)
{
    lv_example_label();             /* 速度提示标签 */
    lv_example_btn_up();            /* 加速按钮 */
    lv_example_btn_down();          /* 减速按钮 */
    lv_example_btn_stop();          /* 急停按钮 */
}
