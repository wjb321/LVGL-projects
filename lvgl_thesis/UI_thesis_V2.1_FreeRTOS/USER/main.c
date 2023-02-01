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
#include "chart_win.h"

int arrValue = 199;
int pscValue = 71;
//extern int arrValue ;
//extern int pscValue;
int Pulse = 62;
int led0pwmval=70;
int TIM1_Enable = ENABLE;
int SpeedDecrease = 0;
int speedflag = 0;
extern lv_coord_t series1_y[POINT_COUNT];
int update_chart_Array = 0;
extern int updateChart;
int ConstantOrChangeSpeed = 0;
int PWMChangeFlag = 0;
int CeleOrDeceFlag = 0;
int InitPWMFlag = 0;
int main(void)
{	 
  vu8 key=0;
  u8 dir=1;	
	delay_init();	    	 		//��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     		//LED�˿ڳ�ʼ��
	KEY_Init();	 						//������ʼ��
	BEEP_Init();						//��������ʼ��
	FSMC_SRAM_Init();				//�ⲿ1MB��sram��ʼ��
	LCD_Init();							//LCD��ʼ��			
 	tp_dev.init();					//��������ʼ��
	
	TIM1_Int_Init(2999, 7199, TIM1_Enable);
	TIM2_Int_Init(arrValue,pscValue,ENABLE); 
  TIM3_PWM_Init(1399,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM5_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	TIM6_Int_Init(9999,7199, ENABLE);
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
            case KEY2_PRES:  //change the current trend:celeration-> deceleration or vice versa
							  CeleOrDeceFlag = !CeleOrDeceFlag;
              break;

            case KEY1_PRES:  // initial the speed either highest or lowest
     						 InitPWMFlag =!InitPWMFlag;
								if(InitPWMFlag) led0pwmval = 30;
							  else led0pwmval = 1079;
              break;

            case KEY0_PRES:  // deceleration or celeration rate change
							 PWMChangeFlag =!PWMChangeFlag;
								if(PWMChangeFlag) SpeedDecrease = 20;
							  else SpeedDecrease = 40;
              break;
						 
						case WKUP_PRES:  // keep current speed or keep the previous trend or or off timer
							ConstantOrChangeSpeed = !ConstantOrChangeSpeed;
							TIM_Cmd(TIM1, ConstantOrChangeSpeed); 
              break;
            }
        }
		 
     if(speedflag == 1)  // timer1 for peridically decreasing or increasing the speed
		 { 	
			 
       if(led0pwmval <= 30) CeleOrDeceFlag = 0;
			 else if(led0pwmval >= 1079) CeleOrDeceFlag = 1;
			 switch(CeleOrDeceFlag)
			 {
				 case 0:
					 led0pwmval +=SpeedDecrease;
					 break;
				 case 1:
					 led0pwmval -=SpeedDecrease;
					 break;
				 default:
					 break;
			 }
			 speedflag = 0;
		 }
		TIM_SetCompare2(TIM3,led0pwmval);		
	}
}


