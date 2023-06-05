/**
 ****************************************************************************************************
 * @file        lv_shelf.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-23
 * @brief       ���ز���ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "LVGL/GUI_APP/lv_shelf.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/BEEP/beep.h"


/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define lv_scr_act_width() lv_obj_get_width(lv_scr_act())
#define lv_scr_act_height() lv_obj_get_height(lv_scr_act())
uint32_t lv_key_statue = 0;

lv_obj_t *lv_key_led[3];
lv_obj_t *lv_key_label[3];
lv_obj_t *lv_shelf_cont;
lv_timer_t *lv_shelf_timer;

typedef struct
{
    const char *lv_key_name;
}lv_ket_label_t;

static const lv_ket_label_t lv_key_info[] =
{
    "KEY0",
    "KEY1",
    "KEY_UP",
};

/**
  * @brief  ��ȡ����ֵ
  * @param  task:����
  * @retval ��
  */
void lv_shelf_vlaue(lv_timer_t *task)
{
    uint8_t key;
    
    key = key_scan(0);

    switch (key)
    {
        case KEY0_PRES:
        case KEY1_PRES:
          lv_key_statue = !lv_key_statue;
          (lv_key_statue)?lv_led_on(lv_key_led[key - 1]):lv_led_off(lv_key_led[key - 1]);
          break;
        default:
          break;
    }
    
    lv_shelf_timer = task;
}

/**
  * @brief  ���ذ����ص�����
  * @param  event:�¼�
  * @retval ��
  */
void lv_shelf_back_btn_event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);
  
    if(code == LV_EVENT_PRESSED)
    {
        lv_label_set_text(obj,"#444444 "BACK_BTN_TITLE"#");
        lv_label_set_recolor(obj,true);
    }
    else if(code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        lv_obj_del(lv_shelf_cont);
        lv_timer_del(lv_shelf_timer);
        lv_mainstart();/* �򿪵�¼���� */
    }
}

/**
  * @brief  �����������ص�����
  * @param  event:�¼�
  * @retval ��
  */
void lv_beep_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);
    
    if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        BEEP_TOGGLE();
    }
}

/**
 * @brief  lv_shelf_switch
 * @param  ��
 * @return ��
 */
static void lv_shelf_switch(void)
{
    lv_general_dev.lv_general_win_create = NULL;
    lv_general_dev.lv_back_event = NULL;
    lv_general_dev.parent = NULL;
  
    lv_shelf_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(lv_shelf_cont, lv_color_make(1, 27, 54), LV_STATE_DEFAULT);
    lv_obj_set_size(lv_shelf_cont,lv_obj_get_width(lv_scr_act()),lv_obj_get_height(lv_scr_act()));
    lv_obj_set_style_radius(lv_shelf_cont, 0, LV_PART_MAIN);
    lv_obj_clear_flag(lv_shelf_cont, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_general_dev.lv_back_event = lv_shelf_back_btn_event_handler;
    lv_general_dev.parent = lv_shelf_cont;
    lv_general_dev.lv_general_win_create = lv_general_win_create;
    lv_general_dev.lv_general_win_create();
  
    lv_key_led[0] = lv_led_create(lv_shelf_cont);                                        /* ���岢����LED */
    lv_obj_set_size(lv_key_led[0], lv_scr_act_width()/10, lv_scr_act_width()/10);        /* ����LED��С */
    lv_obj_align_to(lv_key_led[0], NULL,LV_ALIGN_CENTER, -100, 0);                       /* ����LEDλ�� */
    lv_led_off(lv_key_led[0]);                                                           /* ����LED״̬ */
  
    lv_key_label[0] = lv_label_create(lv_shelf_cont);
    lv_obj_set_style_text_font(lv_key_label[0],&lv_font_montserrat_14,LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lv_key_label[0],lv_color_make(255,255,255),LV_STATE_DEFAULT);
    lv_label_set_text(lv_key_label[0],lv_key_info[0].lv_key_name);
    lv_obj_align_to(lv_key_label[0],lv_key_led[0],LV_ALIGN_OUT_RIGHT_MID,10,0);
    
    
    for (int i = 1; i < 3 ;i ++)
    {
        lv_key_led[i] = lv_led_create(lv_shelf_cont);                                        /* ���岢����LED */
        lv_obj_set_size(lv_key_led[i], lv_scr_act_width()/10, lv_scr_act_width()/10);        /* ����LED��С */
        lv_obj_align_to(lv_key_led[i], (i == 2)?lv_key_led[i - 2]:lv_key_led[i - 1], (i == 2)?LV_ALIGN_OUT_TOP_MID:LV_ALIGN_OUT_BOTTOM_MID, 0,(i == 2)?-10:10);       /* ����LEDλ�� */
        lv_led_off(lv_key_led[i]);                                                           /* ����LED״̬ */
        
        lv_key_label[i] = lv_label_create(lv_shelf_cont);
        lv_obj_set_style_text_font(lv_key_label[i],&lv_font_montserrat_14,LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(lv_key_label[i],lv_color_make(255,255,255),LV_STATE_DEFAULT);
        lv_label_set_text(lv_key_label[i],lv_key_info[i].lv_key_name);
        lv_obj_align_to(lv_key_label[i],lv_key_led[i],LV_ALIGN_OUT_RIGHT_MID,10,0);
    }

    /* ��ʼ�����µ���ʽ */
    static lv_style_t style_pr;
    lv_style_init(&style_pr);

    /* ����ʱ���һ��������� */
    lv_style_set_outline_width(&style_pr, 30);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

    lv_style_set_translate_y(&style_pr, 5);
    lv_style_set_shadow_ofs_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_BLUE, 2));
    lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_BLUE, 4));

    /* ������ӹ��� */
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, 0};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);

    lv_style_set_transition(&style_pr, &trans);
    
    lv_obj_t *lv_beep = lv_led_create(lv_shelf_cont);
    lv_led_off(lv_beep);
    lv_obj_add_style(lv_beep, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(lv_beep,lv_scr_act_width()/3, lv_scr_act_width()/3);
    lv_obj_align_to(lv_beep,NULL,LV_ALIGN_RIGHT_MID,0,0);
    lv_obj_add_flag(lv_beep,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(lv_beep,lv_beep_cb,LV_EVENT_ALL,NULL);
    
    lv_obj_t * lv_beep_label = lv_label_create(lv_beep);
    lv_obj_set_style_text_font(lv_beep_label,&lv_font_montserrat_14,LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lv_beep_label,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
    lv_label_set_text(lv_beep_label,"BEEP");
    lv_obj_center(lv_beep_label);
    
    
    lv_timer_create(lv_shelf_vlaue,10,NULL);
}

/**
 * @brief  LVGL��ʾ
 * @param  ��
 * @return ��
 */
void lv_shelf_demo(void)
{
    lv_shelf_switch();
}
