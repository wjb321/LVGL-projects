# LVGL-projects  
will do some projects based on LVGL:  
basic for testing the lvgl lib in order to be used in the future for MCU as an UI  
In this [project](LVGL-projects/lvgl_thesis/UI_thesis_V2.2_lvgl8.2_FreeRTOS9)

> sys.h:  

configure:  
```
#define SYSTEM_SUPPORT_OS		1  		
```  
to configure if need run freertos or not, this will change the configurations in  
>lv_conf.h  
timer.h  
port.c  
freeRTOSCfig.h
sys.c  
delay.c  
usart.c  

where there are some parts for supporting **OS** or not
