#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "touch.h"
#include "timer.h"
#include "sram.h"
#include "led.h"
#include "can_config.h"
#include "lvgl_ttcan.h"
#include "malloc.h"
#include "beep.h"

#if !SYSTEM_SUPPORT_OS
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_keypad_encoder.h"
#endif


static int arrValue    = 199;
static int pscValue    = 71;
int Pulse         = 62;
int TIM1_Enable   = ENABLE;


//extern lv_coord_t series1_y[POINT_COUNT];


int main(void)
{
  delay_init();	    	 		//��ʱ������ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
  LED_Init();			     		//LED�˿ڳ�ʼ��
  BEEP_Init();						//��������ʼ��
  FSMC_SRAM_Init();				//�ⲿ1MB��sram��ʼ��
//  tp_dev.init();
//	my_mem_init(SRAMIN);                /* ��ʼ���ڲ�SRAM�ڴ�� */
//  my_mem_init(SRAMEX);                /* ��ʼ���ⲿSRAM�ڴ�� */
	CAN_ABSCONTROL_Configuration();
	TIM1_Int_Init(99, 7199, TIM1_Enable);
  TIM2_Int_Init(arrValue,pscValue,ENABLE);
  TIM3_PWM_Init(1399,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM4_EncoderMode_Config(Pulse);
	 //TIM6_Int_Init(9999,7199, ENABLE);
	
#if !SYSTEM_SUPPORT_OS
	TIM5_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
  lv_init();					
  lv_port_disp_init();
  lv_port_indev_init();	
	lv_demo_keypad_encoder();
	
    while(1)
    { 
      lv_timer_handler(); 
			delay_ms(5);
		}
#else
  lvgl_ttcan();
#endif
}
// tp_dev.init();		
//      xTaskCreate((TaskFunction_t )start_task,            //������
//                (const char*    )"start_task",          //��������
//                (uint16_t       )START_STK_SIZE,        //�����ջ��С
//                (void*          )NULL,                  //���ݸ��������Ĳ���
//                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
//                (TaskHandle_t*  )&StartTask_Handler);   //������              
//    vTaskStartScheduler();          //�����������


//void start_task(void *pvParameters)
//{  
//	 pvParameters = pvParameters;
//    taskENTER_CRITICAL();           //�����ٽ���
//    //����TASK1����
//    xTaskCreate((TaskFunction_t )LVGL_task,             
//                (const char*    )"LVGL_task",           
//                (uint16_t       )LVGL_STK_SIZE,        
//                (void*          )NULL,                  
//                (UBaseType_t    )LVGL_TASK_PRIO,        
//                (TaskHandle_t*  )&LVGL_Task_Handler);   
//    //����TASK2����
//    xTaskCreate((TaskFunction_t )task2_task,     
//                (const char*    )"task2_task",   
//                (uint16_t       )TASK2_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_TASK_PRIO,
//                (TaskHandle_t*  )&Task2Task_Handler); 
//    taskEXIT_CRITICAL();            //�˳��ٽ���
//	  vTaskDelete(StartTask_Handler); //ɾ����ʼ����
//}


//task2������
//void task2_task(void *pvParameters)
//{
//	while(1)
//	{
//	       LED1=!LED1;
//        vTaskDelay(1000);                          
//	}
//}

//task1������
//void LVGL_task(void *pvParameters)
//{  vu8 key=0;
////	pvParameters = pvParameters;
////	  lv_demo_stress();
////	
//  while(1)
//    { 
////			   lv_timer_handler(); /* LVGL��ʱ�� */
////        vTaskDelay(5);
////      tp_dev.scan(0);
////      lv_task_handler();
////      key=KEY_Scan(0);	//�õ���ֵ
////      if(key)
////        {
////          switch(key)
////            {
////            case KEY2_PRES:  //change the current trend:celeration-> deceleration or vice versa
////              CeleOrDeceFlag = !CeleOrDeceFlag;
////              printf("CeleOrDeceFlag is %d\n",CeleOrDeceFlag);
////              break;

////            case KEY1_PRES:  // initial the speed either highest or lowest
////              InitPWMFlag =!InitPWMFlag;
////              if(InitPWMFlag) led0pwmval = 30;
////              else led0pwmval = 1079;
////              printf("led0pwmval is %d\n",led0pwmval);
////              break;

////            case KEY0_PRES:  // deceleration or celeration rate change
////              PWMChangeFlag =!PWMChangeFlag;
////              if(PWMChangeFlag) SpeedDecrease = 6;
////              else SpeedDecrease = 3;
////              printf("SpeedDecrease is %d\n",SpeedDecrease);
////              break;

////            case WKUP_PRES:  // keep current speed or keep the previous trend or or off timer
////              ConstantOrChangeSpeed = !ConstantOrChangeSpeed;
////              TIM_Cmd(TIM1, ConstantOrChangeSpeed);
////              printf("ConstantOrChangeSpeed is %d\n",ConstantOrChangeSpeed);
////              break;
////            }
////        }
////      switch(CAN_Speedflag)
////        {
////        case 1:
////          if(led0pwmval <= 30) CeleOrDeceFlag = 0;
////          else if(led0pwmval >= 1079) CeleOrDeceFlag = 1;
////          switch(CeleOrDeceFlag)
////            {
////            case 0:
////              led0pwmval +=SpeedDecrease; // decelation
////						  //BEEP = !BEEP;
////						  //printf("led0pwmval is %d\n",led0pwmval);
////              break;
////            case 1:
////              led0pwmval -=SpeedDecrease; // celelation
////              break;
////            default:
////              break;
////            }
////          CAN_Speedflag = 0;
////          break;
////        case 2:
////					 switch(Rx1_DATA0)
////					 {
////						 case 1:
////							 SpeedDecrease = 6;
////						   led0pwmval -= SpeedDecrease;
////						   BEEP = !BEEP;
////							 //printf("blocked\n");
////							 break;
////						 case 2:
////							 printf("celeration\n");
////							 break;
////						 case 3:
////							 printf("celeration\n");
////							 break;
////						 case 4:
////							 printf("constant\n");
////							 break;
////						 default:
////							 break;
////					 }
////					 CAN_Speedflag = 0;
////          break;
////        default:
////          break;
////        }
////      TIM_SetCompare2(TIM3,led0pwmval);
////		//		printf("hello the world'\r\n");
////					//delay_ms(500);
////		//lv_timer_handler();
////  		   LED0=!LED0;
////        vTaskDelay(1000);
//   }

//}



