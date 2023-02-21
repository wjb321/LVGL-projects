#include "lvgl_ttcan.h"
#include "lvgl.h"
#include "timer.h"
#include "led.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_stress.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"

#define START_TASK_PRIO     1           
#define START_STK_SIZE      128        
TaskHandle_t StartTask_Handler;       
void start_task(void *pvParameters);    


#define LV_DEMO_TASK_PRIO   3         
#define LV_DEMO_STK_SIZE    128       
TaskHandle_t LV_DEMOTask_Handler;      
void lv_demo_task(void *pvParameters); 


#define LED_TASK_PRIO       4         
#define LED_STK_SIZE        128        
TaskHandle_t LEDTask_Handler;         
void led_task(void *pvParameters);     


void lvgl_ttcan(void)
{
//    lv_init();                                          /* lvgl系统初始化 */
//    lv_port_disp_init();                                /* lvgl显示接口初始化,放在lv_init()的后面 */
//    lv_port_indev_init();                               /* lvgl输入接口初始化,放在lv_init()的后面 */

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度 */
}


void start_task(void *pvParameters)
{
    
    taskENTER_CRITICAL();          
    
//	 	while(1)
//	{
//		    LED1 = !LED1;
//	 delay_ms(1000);
//	}


//    xTaskCreate((TaskFunction_t )lv_demo_task,
//                (const char*    )"lv_demo_task",
//                (uint16_t       )LV_DEMO_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )LV_DEMO_TASK_PRIO,
//                (TaskHandle_t*  )&LV_DEMOTask_Handler);


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
    pvParameters = pvParameters;
    
    lv_demo_stress();     
    
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
			 printf("now comes to free rtos tasks \r\n");
			  LED1 = !LED1;
        vTaskDelay(1000);
    }
}