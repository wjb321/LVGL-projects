25.07

1. sram configuration which speeds up freshing
2. screen direction(horizontal screen and vertical screen)change: 
   a. 
   #define LV_HOR_RES_MAX          (800)  //320  (480) 
   #define LV_VER_RES_MAX          (480)  //480  (800) 
   
   b. LCD_Display_Dir(1);         //vertical screen 
      LCD_Display_Dir(0);         //horizontal screen
	  
   only change b can switch between horizontal screen and vetical screen, becasue 
       disp_drv.hor_res = lcddev.width;
       disp_drv.ver_res = lcddev.height;
   cover a's definition. while to be one the safe side, should remeber to swap the value of the direction
   in the macro definition in a.
   
27.07

task timeliness is directly defined by lv_task_handler, which is located in while function, make sure inside
while function there is no big delay function. 
https://akaedu.github.io/book/ch23s08.html

union and struct: 
https://blog.csdn.net/dreamback1987/article/details/8504943

function pointer and pointer function:

typedef:http://c.biancheng.net/view/298.html