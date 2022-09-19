#include "led.h"
#include "delay.h"
#include "key.h"
#include "beep.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "touch.h"
#include "timer.h"
#include "sram.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_1.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"
#include "gui_app.h"


u16 arrValue = 199;
u16 pscValue = 71;
u16 Pulse = 62;
int main(void)
{	 
  vu8 key=0;
  int led0pwmval=70;
  u8 dir=1;	
	delay_init();	    	 		//��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     		//LED�˿ڳ�ʼ��
	KEY_Init();	 						//������ʼ��
	BEEP_Init();						//��������ʼ��
	TIM5_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	FSMC_SRAM_Init();				//�ⲿ1MB��sram��ʼ��
	LCD_Init();							//LCD��ʼ��			
 	tp_dev.init();					//��������ʼ��
	TIM2_Int_Init(arrValue,pscValue,ENABLE); 
  TIM3_PWM_Init(1399,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM4_EncoderMode_Config(Pulse);
	
	lv_init();							//lvglϵͳ��ʼ��
	lv_port_disp_init();		//lvgl��ʾ�ӿڳ�ʼ��
	lv_port_indev_init();		//lvgl����ӿڳ�ʼ��
	
	
	gui_app_start(); 	//��������
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		 key=KEY_Scan(0);	//�õ���ֵ
      if(key)
        {
          switch(key)
            {
            case KEY2_PRES: // decelerate
							led0pwmval+= 10;
						  if(led0pwmval >=1399) led0pwmval =1399;
						  //else if (74 <=led0pwmval && led0pwmval<= 83) led0pwmval = 84;
						  printf("led0pwmval %d \r\n", led0pwmval);
              break;

            case KEY1_PRES:  // decelerate
							led0pwmval +=20;
						  if(led0pwmval >=1399) led0pwmval =1399;
						  printf("led0pwmval %d \r\n", led0pwmval);
              break;

            case KEY0_PRES:  // celerate
							led0pwmval -=10;
						  if(led0pwmval <=10) led0pwmval =10;
						  printf("led0pwmval %d \r\n", led0pwmval);
              break;
						 
						case WKUP_PRES:  // mid high
							led0pwmval -=20;
						  if(led0pwmval <=10) led0pwmval =10;
						 printf("led0pwmval %d \r\n", led0pwmval);
              break;
            }
        }
				TIM_SetCompare2(TIM3,led0pwmval);
	}
}


