#include "lvgl_ttcan.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
//#include "lv_demo_stress.h"

#include "timer.h"
#include "led.h"

#if SYSTEM_SUPPORT_OS 
#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "lv_demo_keypad_encoder.h"

#define START_TASK_PRIO     1           
#define START_STK_SIZE      128        
TaskHandle_t StartTask_Handler;       
void start_task(void *pvParameters);    


#define LV_DEMO_TASK_PRIO   3         
#define LV_DEMO_STK_SIZE   1024       
TaskHandle_t LV_DEMOTask_Handler;      
void lv_demo_task(void *pvParameters); 


#define LED_TASK_PRIO       4         
#define LED_STK_SIZE        128        
TaskHandle_t LEDTask_Handler;         
void led_task(void *pvParameters);     


void lvgl_ttcan(void)
{

  lv_init();					
  lv_port_disp_init();
  lv_port_indev_init();	
	
  xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */


    vTaskStartScheduler();                           
}

void start_task(void *pvParameters)
{ 
   taskENTER_CRITICAL();          

    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);


    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            
    vTaskDelete(StartTask_Handler);
}


void lv_demo_task(void *pvParameters)
{  
    //lv_demo_stress(); 
    lv_demo_keypad_encoder();
    while(1)
			{  
        lv_timer_handler(); 
        vTaskDelay(5);

    }
}


void led_task(void *pvParameters)
{
    while(1)
    {  
			  LED1 = !LED1;
        vTaskDelay(1000);
    }
}

#endif

