
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

    //����״̬�л���������ε��������¼��Ļص��Ͳ����ٹ�����
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);//����ȥ��ɫ�ᷢ���仯
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_VALUE_CHANGED, NULL);//����ص�����
}



static const lv_font_t* font;       /* �������� */

static lv_obj_t *label_speed;       /* �ٶ���ʾ�ı� */
static lv_obj_t *btn_speed_up;      /* ���ٰ�ť */
static lv_obj_t *btn_speed_down;    /* ���ٰ�ť */
static lv_obj_t *btn_stop;          /* ��ͣ��ť */

static int32_t speed_val = 0;       /* �ٶ�ֵ */

/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())


/**
 * @brief  ��ť�ص�
 * @param  *e ���¼���ز����ļ��ϣ��������˸��¼�����������
 * @return ��
 */
static void btn_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* ��ȡ����Դ */

    if(target == btn_speed_up)                      /* ���ٰ�ť */
    {
        speed_val += 30;
    }
    else if(target == btn_speed_down)               /* ���ٰ�ť */
    {
        speed_val -= 30;
    }
    else if(target == btn_stop)                     /* ��ͣ��ť */
    {
        speed_val = 0;
    }
    lv_label_set_text_fmt(label_speed, "Speed : %d RPM", speed_val);    /* �����ٶ�ֵ */
}


/**
 * @brief  �ٶ�ֵ��ʾ��ǩ
 * @param  ��
 * @return ��
 */
static void lv_example_label(void)
{
    /* ���ݻ��Ļ���ѡ������ */
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

    label_speed = lv_label_create(lv_scr_act());                                    /* �����ٶ���ʾ��ǩ */
    lv_obj_set_style_text_font(label_speed, font, LV_PART_MAIN);                    /* �������� */
    lv_label_set_text(label_speed, "Speed : 0 RPM");                                /* �����ı� */
    lv_obj_align(label_speed, LV_ALIGN_CENTER, 0, -scr_act_height() / 3);           /* ���ñ�ǩλ�� */
}
/**
 * @brief  ���ٰ�ť
 * @param  ��
 * @return ��
 */
static void lv_example_btn_up(void)
{
    btn_speed_up = lv_btn_create(lv_scr_act());                                     /* �������ٰ�ť */
    lv_obj_set_size(btn_speed_up, scr_act_width() / 4, scr_act_height() / 6);       /* ���ð�ť��С */
    lv_obj_align(btn_speed_up, LV_ALIGN_CENTER, -scr_act_width() / 3, 0);           /* ���ð�ťλ�� */
    lv_obj_add_event_cb(btn_speed_up, btn_event_cb, LV_EVENT_CLICKED, NULL);        /* ���ð�ť�¼� */

    lv_obj_t* label = lv_label_create(btn_speed_up);                                /* �������ٰ�ť��ǩ */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label, "Speed +");                                            /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* ���ñ�ǩλ�� */
}

/**
 * @brief  ���ٰ�ť
 * @param  ��
 * @return ��
 */
static void lv_example_btn_down(void)
{
    btn_speed_down = lv_btn_create(lv_scr_act());                                   /* �������ٰ�ť */
    lv_obj_set_size(btn_speed_down, scr_act_width() / 4, scr_act_height() / 6);     /* ���ð�ť��С */
    lv_obj_align(btn_speed_down, LV_ALIGN_CENTER, 0, 0);                            /* ���ð�ťλ�� */
    lv_obj_add_event_cb(btn_speed_down, btn_event_cb, LV_EVENT_CLICKED, NULL);      /* ���ð�ť�¼� */

    lv_obj_t* label = lv_label_create(btn_speed_down);                              /* �������ٰ�ť��ǩ */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label, "Speed -");                                            /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* ���ñ�ǩλ�� */
}

/**
 * @brief  ��ͣ��ť
 * @param  ��
 * @return ��
 */
static void lv_example_btn_stop(void)
{
    btn_stop = lv_btn_create(lv_scr_act());                                         /* ������ͣ��ť */
    lv_obj_set_size(btn_stop, scr_act_width() / 4, scr_act_height() / 6);           /* ���ð�ť��С */
    lv_obj_align(btn_stop, LV_ALIGN_CENTER, scr_act_width() / 3, 0);                /* ���ð�ťλ�� */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  /* ���ð�ť������ɫ��Ĭ�ϣ� */
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);  /* ���ð�ť������ɫ�����£� */
    lv_obj_add_event_cb(btn_stop, btn_event_cb, LV_EVENT_CLICKED, NULL);            /* ���ð�ť�¼� */

    lv_obj_t* label = lv_label_create(btn_stop);                                    /* ������ͣ��ť��ǩ */
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label, "Stop");                                               /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label,LV_ALIGN_CENTER);                                        /* ���ñ�ǩλ�� */
}

/**
 * @brief  LVGL��ʾ
 * @param  ��
 * @return ��
 */
void lv_btn_test2(void)
{
    lv_example_label();             /* �ٶ���ʾ��ǩ */
    lv_example_btn_up();            /* ���ٰ�ť */
    lv_example_btn_down();          /* ���ٰ�ť */
    lv_example_btn_stop();          /* ��ͣ��ť */
}
