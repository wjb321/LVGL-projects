#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "touch.h"
#include "timer.h"
#include "lvgl.h"
#include "sram.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_1.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"
/************************************************
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/



int main(void)
{	 		    
	delay_init();	    	 		//��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     		//LED�˿ڳ�ʼ��
	KEY_Init();	 						//������ʼ��
	TIM3_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms��������
	FSMC_SRAM_Init();
	LCD_Init();							//LCD��ʼ��, why it did not work properly someone can tell me?
 	tp_dev.init();					//��������ʼ��
	lv_init();							//lvglϵͳ��ʼ��
	lv_port_disp_init();		//lvgl��ʾ�ӿڳ�ʼ��
	lv_port_indev_init();		//lvgl����ӿڳ�ʼ��
	
	//demo_create();					//��ʼ����demo
	//lv_test_theme_1(lv_theme_night_init(210, NULL));
	lv_test_theme_2();
	
	
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		/*
		ensure there are not so many delay function, which wastes lots of time
		*/
	}
}


