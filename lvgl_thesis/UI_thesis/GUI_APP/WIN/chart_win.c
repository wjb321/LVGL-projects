#include "chart_win.h"
#include "gui_app.h"
#include <stdio.h>

extern float TMethodSpeed ;
#define POINT_COUNT   	10  //�����������е����ݵ����
#define SLIDER_MAX_VAL	100	//��������ֵ
lv_coord_t series1_y[POINT_COUNT] = {30,60,40,60,20,60,50,80,60,80};
static lv_obj_t * chart1;
static lv_chart_series_t * series1;
static lv_obj_t * slider1;
static lv_obj_t *  sw1;
static lv_obj_t * cb1;

//void speed_update()
//{
//	static int counter = 0;
//	series1_y[counter] = TMethodSpeed;
//	counter ++;
//	if(counter >10) counter = 0;
//	
//}


//static lv_task_t * task = NULL;

////�¼��ص�����
//static void event_handler(lv_obj_t * obj,lv_event_t event)
//{
//	if(event==USER_EVENT_BACK)//���巵���¼�,������Ͻǵ�"< Back"��ť�ᴥ�����¼�
//	{
//		if(task)//ɾ��������
//		{
//			lv_task_del(task);
//			task = NULL;
//		}
//	}
//}

////����ص�����
//void task_cb(lv_task_t * task)
//{
//	static uint8_t is_add_dir = 1;//�Ƿ����ٶ����ӵķ���
//	char buff[40];

////	if(is_add_dir)
////	{
////			speed_val += 5;
////			if(speed_val>=100)
////				is_add_dir = 0;
////	}else
////	{
////			speed_val -= 5;
////			if(speed_val<=0)
////				is_add_dir = 1;
////	}
//	
//	//����ָ�����ֵ
//	lv_gauge_set_value(gauge1,0,TMethodSpeed);
//	//�Ѵ��ٶ���ʾ�ڱ�ǩ��,Ȼ����ݲ�ͬ����ֵ��ʾ����ͬ����ɫ
//	//��ɫ����ȫ,��ɫ������,��ɫ����Σ��  
//	sprintf(buff,"#%s %d r/m#",TMethodSpeed<100?"00FF00":(TMethodSpeed<150?"FFFF00":"FF0000"),TMethodSpeed); 
//	lv_label_set_text(label1,buff);
//	//���ÿ̶�ָʾ������ֵ
//	lv_lmeter_set_value(lmeter1,TMethodSpeed);
//}

//�¼��ص�����
static void event_handler(lv_obj_t * obj,lv_event_t event)
{
	float val;
	uint8_t i;
	bool sta;
	
	if(event==LV_EVENT_VALUE_CHANGED)
	{
		if(obj==slider1)//����,�����ı������ߵ�����
		{
			val = lv_slider_get_value(obj);//��ȡ����ĵ�ǰֵ
			val /= SLIDER_MAX_VAL;//�õ�����ϵ��
			for(i=0;i<POINT_COUNT;i++)
				series1->points[i] = (lv_coord_t)(val*series1_y[i]);//��������ϵ��,�޸����ݵ��ֵ
			lv_chart_refresh(chart1);//����ͼ��
		}else if(obj==sw1)//����,����ѡ��ͼ�������
		{
			sta = lv_sw_get_state(obj);
			if(sta)
				lv_chart_set_type(chart1,LV_CHART_TYPE_POINT|LV_CHART_TYPE_LINE);//����Ϊɢ������ߵ����
			else
				lv_chart_set_type(chart1,LV_CHART_TYPE_COLUMN);//��״ͼ 
		}else if(obj==cb1)//��ѡ��,Ҳ������ѡ��ͼ�������
		{
			sta = lv_cb_is_checked(obj);
			if(sta)
				lv_chart_set_type(chart1,LV_CHART_TYPE_AREA);//���ͼ
			else
				lv_chart_set_type(chart1,LV_CHART_TYPE_POINT|LV_CHART_TYPE_LINE);//����Ϊɢ������ߵ����
		}
	}
}

//��������
void chart_win_create(lv_obj_t * parent)
{
	lv_obj_t * win;
	win = general_win_create(parent,"Chart");
	
	//����ͼ��
	chart1 = lv_chart_create(win,NULL);
  lv_obj_set_size(chart1,360,400);//���ô�С
  lv_obj_align(chart1,NULL,LV_ALIGN_IN_TOP_MID,17,40);//���ö��뷽ʽ
  lv_chart_set_type(chart1,LV_CHART_TYPE_POINT|LV_CHART_TYPE_LINE);//����Ϊɢ������ߵ����
  lv_chart_set_series_opa(chart1,LV_OPA_80);//���������ߵ�͸����
  lv_chart_set_series_width(chart1,4);//���������ߵĿ��
  lv_chart_set_series_darking(chart1,LV_OPA_80);//���������ߵĺ���ӰЧ��
  lv_chart_set_point_count(chart1,POINT_COUNT);//�������ݵ�ĸ���
  lv_chart_set_div_line_count(chart1,4,4);//����ˮƽ�ʹ�ֱ�ָ���
  lv_chart_set_range(chart1,0,100);//����y�����ֵ��Χ
  lv_chart_set_y_tick_length(chart1,10,3);
  lv_chart_set_y_tick_texts(chart1,"100\n80\n60\n40\n20\n0",2,LV_CHART_AXIS_DRAW_LAST_TICK);
  lv_chart_set_x_tick_length(chart1,10,3);
  lv_chart_set_x_tick_texts(chart1,"0\n2\n4\n6\n8\n10",2,LV_CHART_AXIS_DRAW_LAST_TICK);
  lv_chart_set_margin(chart1,40);
  //��ͼ�������������
  series1 = lv_chart_add_series(chart1,LV_COLOR_RED);//ָ��Ϊ��ɫ
  lv_chart_set_points(chart1,series1,(lv_coord_t*)series1_y);//��ʼ�����ݵ��ֵ
	
	//��������
	slider1 = lv_slider_create(win,NULL);
	lv_obj_set_size(slider1,200,20);//���ô�С
	lv_slider_set_range(slider1,0,SLIDER_MAX_VAL);//���÷�Χ
	lv_slider_set_value(slider1,SLIDER_MAX_VAL,LV_ANIM_OFF);
	lv_obj_align(slider1,chart1,LV_ALIGN_OUT_BOTTOM_MID,-17,45);//���ö��뷽ʽ
	lv_obj_set_event_cb(slider1,event_handler);//�����¼��ص�����
	
	//������ѡ��
	cb1 = lv_cb_create(win, NULL);
	lv_cb_set_text(cb1,"Area type");//�����ı�
	lv_cb_set_checked(cb1,false);//���ø�ѡ��Ĭ��û�б�ѡ�� 
	lv_obj_align(cb1,slider1,LV_ALIGN_OUT_BOTTOM_MID,0,15);//���ö��뷽ʽ
	lv_obj_set_event_cb(cb1,event_handler);//�����¼��ص�����
	
	//��������
	sw1 = lv_sw_create(win,NULL);
	lv_obj_set_size(sw1,80,40);//���ô�С
	lv_obj_align(sw1,cb1,LV_ALIGN_OUT_BOTTOM_MID,0,15);//���ö��뷽ʽ
	lv_sw_on(sw1,LV_ANIM_ON);//Ĭ��Ϊ��״̬
	lv_obj_set_event_cb(sw1,event_handler);//�����¼��ص�����
	
	
	
	
	 //����һ��������ģ���ٶ�ָ��ı仯
//		if(task==NULL)
//		task = lv_task_create(task_cb,600,LV_TASK_PRIO_MID,NULL);
	
}















