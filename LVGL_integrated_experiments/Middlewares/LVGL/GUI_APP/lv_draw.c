/**
 ****************************************************************************************************
 * @file        lv_draw.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-03-23
 * @brief       LVGL ���� ʵ��
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
 
#include "LVGL/GUI_APP/lv_draw.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TOUCH/touch.h"
#include "lvgl.h"
#include <stdio.h>


lv_obj_t * lv_touch_cont;
lv_timer_t *lv_touchtask;
uint8_t t = 0;
uint16_t lastpos[10][2];        /* ���һ�ε����� */
uint8_t maxp = 5;
/* 10�����ص����ɫ(���ݴ�������) */
const uint16_t POINT_COLOR_TBL[4] = {RED,GREEN,BLUE,GRAY};
/* ���廭�ʵĴ�С */
uint8_t lv_pensise = 2;
/* ���廭����ɫ������ */
uint8_t lv_pencolor = 0;

static lv_point_t line_points1[] = { {0, 0}, {0, 70}, {0, 120}, {0, 180}, {0, 240}, {0, 480} };

/* ����ʵĴ�С�ؼ��� */
lv_obj_t *lv_pen_size_2;
lv_obj_t *lv_pen_size_5;
lv_obj_t *lv_pen_size_10;
lv_obj_t *lv_pen_size_15;
/* �������ɫ�Ĵ�С�ؼ��� */
lv_obj_t *lv_pen_color_red;
lv_obj_t *lv_pen_color_green;
lv_obj_t *lv_pen_color_blue;
lv_obj_t *lv_pen_color_grey;

/**
 * @brief       ����ص�����
 * @param       event: �¼�
 * @retval      ��
 */
static void lv_rst_event(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);

    if(code == LV_EVENT_CLICKED)
    {
        lv_obj_set_style_bg_color(lv_touch_cont, lv_color_make(255,255,255),LV_STATE_DEFAULT);
    }
}

/**
 * @brief       ������
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       size : ������ϸ�̶�
 * @param       color: �ߵ���ɫ
 * @retval      ��
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)return;

    delta_x = x2 - x1;  /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;       /* ���õ������� */
    else if (delta_x == 0)incx = 0; /* ��ֱ�� */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;   /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x; /* ѡȡ�������������� */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )    /* ������� */
    {
        lcd_fill_circle(row, col, size, color); /* ���� */
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief       ���ʴ�С�ص�����
 * @param       event: �¼�
 * @retval      ��
 */
static void lv_pen_size_event(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);

    if(code == LV_EVENT_CLICKED)
    {
        if (obj == lv_pen_size_2)
        {
            lv_pensise = 2;
            lv_obj_set_style_bg_color(lv_pen_size_5,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_10,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_15,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_2,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
        }
        else if (obj == lv_pen_size_5)
        {
            lv_pensise = 5;
            lv_obj_set_style_bg_color(lv_pen_size_2,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_10,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_15,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_5,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
        }
        else if (obj == lv_pen_size_10)
        {
            lv_pensise = 10;
            lv_obj_set_style_bg_color(lv_pen_size_5,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_2,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_15,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_10,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
        }
        else if (obj == lv_pen_size_15)
        {
            lv_pensise = 15;
            lv_obj_set_style_bg_color(lv_pen_size_5,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_10,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_2,lv_color_make(255,255,255),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(lv_pen_size_15,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
        }
        
    }
}

/**
 * @brief       ��ɫ���ûص�����
 * @param       event: �¼�
 * @retval      ��
 */
static void lv_pen_color_event(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);
    
    if(code == LV_EVENT_CLICKED)
    {
        if (obj == lv_pen_color_red)
        {
            lv_obj_set_style_border_color(lv_pen_color_green,lv_palette_main(LV_PALETTE_GREEN),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_green,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_blue,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_blue,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_grey,lv_palette_main(LV_PALETTE_GREY),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_grey,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_red,lv_color_make(0,0,0),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_red,2,LV_STATE_DEFAULT);
            lv_pencolor = 0;
        }
        else if (obj == lv_pen_color_green)
        {
            lv_obj_set_style_border_color(lv_pen_color_red,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_red,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_blue,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_blue,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_grey,lv_palette_main(LV_PALETTE_GREY),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_grey,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_green,lv_color_make(0,0,0),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_green,2,LV_STATE_DEFAULT);
            lv_pencolor = 1;
        }
        else if (obj == lv_pen_color_blue)
        {
            lv_obj_set_style_border_color(lv_pen_color_green,lv_palette_main(LV_PALETTE_GREEN),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_green,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_red,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_red,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_grey,lv_palette_main(LV_PALETTE_GREY),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_grey,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_blue,lv_color_make(0,0,0),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_blue,2,LV_STATE_DEFAULT);
            lv_pencolor = 2;
        }
        else if (obj == lv_pen_color_grey)
        {
            lv_obj_set_style_border_color(lv_pen_color_green,lv_palette_main(LV_PALETTE_GREEN),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_green,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_blue,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_blue,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_red,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_red,0,LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(lv_pen_color_grey,lv_color_make(0,0,0),LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(lv_pen_color_grey,2,LV_STATE_DEFAULT);
            lv_pencolor = 3;
        }
        
        lv_obj_set_style_shadow_width(obj,0,LV_STATE_DEFAULT);
    }
}

/**
 * @brief       ����������İ����ص�����
 * @param       event: �¼�
 * @retval      ��
 */
static void lv_back_event(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t * obj = lv_event_get_target(event);
    
    if (code == LV_EVENT_RELEASED)
    {
        lv_obj_del(lv_touch_cont);
        lv_timer_del(lv_touchtask);
        lv_mainstart();
    }
}
/**
 * @brief       ����������İ���
 * @param       parent:����
 * @retval      ��
 */
void lv_btn_back(lv_obj_t * parent)
{
    /* ����ʽ��ʼ��ΪĬ��״̬ */
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 3);

    lv_style_set_bg_opa(&style, LV_OPA_100);
    lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_color(&style, lv_palette_darken(LV_PALETTE_BLUE, 2));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&style, LV_OPA_40);
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_shadow_width(&style, 8);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_ofs_y(&style, 8);

    lv_style_set_outline_opa(&style, LV_OPA_COVER);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_pad_all(&style, 10);

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

    lv_obj_t * btn1 = lv_btn_create(parent);
    lv_obj_remove_style_all(btn1);                          /* �Ƴ������������ʽ */
    lv_obj_add_style(btn1, &style, 0);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(btn1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(btn1,LV_ALIGN_BOTTOM_RIGHT,15,0);
    lv_obj_add_event_cb(btn1,lv_back_event,LV_EVENT_ALL,NULL);
    
    lv_obj_t * label = lv_label_create(btn1);
    lv_label_set_text(label, "Back");
    lv_obj_center(label);
}

/**
 * @brief       �����Ļ�������Ͻ���ʾ"RST"
 * @param       parent:����
 * @retval      ��
 */
void lv_load_draw_dialog(lv_obj_t *parent)
{
    lcd_clear(WHITE);       /* ���� */
    lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); /* ��ʾ�������� */
  
    #define TOP_OFFSET  5
    lv_obj_t * RST_btn = lv_label_create(parent);
    lv_obj_move_foreground(RST_btn);
    lv_label_set_text(RST_btn,"RST");
    lv_obj_set_style_bg_color(RST_btn,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(RST_btn,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
    lv_obj_align_to(RST_btn,NULL,LV_ALIGN_TOP_RIGHT,-5,TOP_OFFSET);
    lv_obj_add_flag(RST_btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(RST_btn,lv_rst_event,LV_EVENT_CLICKED,NULL);
    
    lv_obj_t * lv_pen_size = lv_label_create(parent);
    lv_label_set_text(lv_pen_size,"pen size");
    lv_obj_align_to(lv_pen_size,RST_btn,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    
    lv_pen_size_2 = lv_led_create(parent);
    lv_obj_set_style_bg_color(lv_pen_size_2,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
    lv_obj_align_to(lv_pen_size_2,lv_pen_size,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_t *lv_pen_size_2_label = lv_label_create(parent);
    lv_label_set_text(lv_pen_size_2_label,"2");
    lv_obj_align_to(lv_pen_size_2_label,lv_pen_size_2,LV_ALIGN_LEFT_MID,-15,0);
    
    
    lv_pen_size_5 = lv_led_create(parent);
    lv_obj_align_to(lv_pen_size_5,lv_pen_size_2,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_t *lv_pen_size_5_label = lv_label_create(parent);
    lv_label_set_text(lv_pen_size_5_label,"5");
    lv_obj_align_to(lv_pen_size_5_label,lv_pen_size_5,LV_ALIGN_LEFT_MID,-15,0);
    
    lv_pen_size_10 = lv_led_create(parent);
    lv_obj_align_to(lv_pen_size_10,lv_pen_size_5,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_t *lv_pen_size_10_label = lv_label_create(parent);
    lv_label_set_text(lv_pen_size_10_label,"10");
    lv_obj_align_to(lv_pen_size_10_label,lv_pen_size_10,LV_ALIGN_LEFT_MID,-15,0);
    
    lv_pen_size_15 = lv_led_create(parent);
    lv_obj_align_to(lv_pen_size_15,lv_pen_size_10,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    lv_obj_t *lv_pen_size_15_label = lv_label_create(parent);
    lv_label_set_text(lv_pen_size_15_label,"15");
    lv_obj_align_to(lv_pen_size_15_label,lv_pen_size_15,LV_ALIGN_LEFT_MID,-15,0);
    
    lv_obj_add_flag(lv_pen_size_2,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_size_5,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_size_10,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_size_15,LV_OBJ_FLAG_CLICKABLE);
    
    
    lv_obj_add_event_cb(lv_pen_size_2,lv_pen_size_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_size_5,lv_pen_size_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_size_10,lv_pen_size_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_size_15,lv_pen_size_event,LV_EVENT_ALL,NULL);
    
    
    
    lv_obj_t * lv_pen_color = lv_label_create(parent);
    lv_label_set_text(lv_pen_color,"color");
    lv_obj_align_to(lv_pen_color,lv_pen_size,LV_ALIGN_OUT_BOTTOM_MID,0,150);
    
    lv_pen_color_red = lv_led_create(parent);
    lv_obj_set_style_shadow_width(lv_pen_color_red,0,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_pen_color_red,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(lv_pen_color_red,lv_color_make(0,0,0),LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(lv_pen_color_red,2,LV_STATE_DEFAULT);
    lv_obj_align_to(lv_pen_color_red,lv_pen_color,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    
    lv_pen_color_green = lv_led_create(parent);
    lv_obj_set_style_shadow_width(lv_pen_color_green,0,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_pen_color_green,lv_palette_main(LV_PALETTE_GREEN),LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(lv_pen_color_green,lv_palette_main(LV_PALETTE_GREEN),LV_STATE_DEFAULT);
    lv_obj_align_to(lv_pen_color_green,lv_pen_color_red,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    
    lv_pen_color_blue = lv_led_create(parent);
    lv_obj_set_style_shadow_width(lv_pen_color_blue,0,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_pen_color_blue,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(lv_pen_color_blue,lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
    lv_obj_align_to(lv_pen_color_blue,lv_pen_color_green,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    
    lv_pen_color_grey = lv_led_create(parent);
    lv_obj_set_style_shadow_width(lv_pen_color_grey,0,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_pen_color_grey,lv_palette_main(LV_PALETTE_GREY),LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(lv_pen_color_grey,lv_palette_main(LV_PALETTE_GREY),LV_STATE_DEFAULT);
    lv_obj_align_to(lv_pen_color_grey,lv_pen_color_blue,LV_ALIGN_OUT_BOTTOM_MID,0,10);
    
    
    lv_obj_add_flag(lv_pen_color_red,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_color_green,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_color_blue,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(lv_pen_color_grey,LV_OBJ_FLAG_CLICKABLE);
    
    
    lv_obj_add_event_cb(lv_pen_color_red,lv_pen_color_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_color_green,lv_pen_color_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_color_blue,lv_pen_color_event,LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(lv_pen_color_grey,lv_pen_color_event,LV_EVENT_ALL,NULL);
    
    
    lv_obj_t* line1 = lv_line_create(parent);
    lv_line_set_points(line1, line_points1, 6); 
    lv_obj_align_to(line1,lv_pen_size,LV_ALIGN_OUT_LEFT_TOP,0,-50);

    lv_btn_back(parent);/* �������������水�� */
    
}


/**
 * @brief       ���ݴ��������Ժ���
 * @param       ��
 * @retval      ��
 */
void ctp_test(void)
{
    if (lcddev.id == 0X1018)maxp = 10;
  
    tp_dev.scan(0);

    for (t = 0; t < maxp; t++)
    {
        if ((tp_dev.sta) & (1 << t))
        {
            if (tp_dev.x[t] < lv_obj_get_width(lv_touch_cont) - lv_obj_get_width(lv_obj_get_child(lv_touch_cont,1)) - 10 && tp_dev.y[t] < lv_obj_get_height(lv_touch_cont))  /* ��������Ļ��Χ�� */
            {
                if (lastpos[t][0] == 0XFFFF)
                {
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];
                }

                lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], lv_pensise, POINT_COLOR_TBL[lv_pencolor]); /* ���� */
                lastpos[t][0] = tp_dev.x[t];
                lastpos[t][1] = tp_dev.y[t];

            }
        }
        else 
        {
            lastpos[t][0] = 0XFFFF;
        }
    }    
}

/**
 * @brief       ���败�������Ժ���
 * @param       ��
 * @retval      ��
 */
void rtp_test(void)
{
    tp_dev.scan(0);

    if (tp_dev.sta & TP_PRES_DOWN)  /* ������������ */
    {
        if (tp_dev.x[0] < lv_obj_get_width(lv_touch_cont) - lv_obj_get_width(lv_obj_get_child(lv_touch_cont,1)) - 10 && tp_dev.y[0] < lv_obj_get_height(lv_touch_cont))
        {
             tp_draw_big_point(tp_dev.x[0], tp_dev.y[0], POINT_COLOR_TBL[lv_pencolor]);   /* ���� */
        }
    }
}

void lv_touch_task(lv_timer_t *task)
{
    lv_touchtask = task;

    if (tp_dev.touchtype & 0X80)
    {
        ctp_test(); /* ���������� */
    }
    else
    {
        rtp_test(); /* ���������� */
    }
}

/**
 * @brief       ���败�������Ժ���
 * @param       ��
 * @retval      ��
 */
void lv_draw_demo(void)
{
    if (lcddev.dir == 0)    /* ����ģʽ, x������Ҫ�任 */
    {
        lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_90);
    }
    
    lv_touch_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(lv_touch_cont,lv_obj_get_width(lv_scr_act()),lv_obj_get_height(lv_scr_act()));
    lv_obj_clear_flag(lv_touch_cont,LV_OBJ_FLAG_SCROLLABLE);
    lv_load_draw_dialog(lv_touch_cont);
    lv_timer_create(lv_touch_task,10,NULL);
}
