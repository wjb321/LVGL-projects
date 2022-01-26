#include "lv_conf.h"
//#include <lv_demo.h>
#include <stdint.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
//#include "example_gif.c"
//#include "tits.h"
#include <time.h>
//#include "lv_port_indev.h"
#include "number_games.c"
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/
#include "example.c"

#include "lv_gif.h"
#include "sd_card.h"
#include "lv_port_fatfs.h"


#define LCD_BL_PIN            2
#define LCD_BL_PWM_CHANNEL    0


void taskOne( void * parameter ){
  while(1){
	lv_tick_inc(5);
	delay(5);
  }
  Serial.println("Ending task 1");
  vTaskDelete( NULL );
}
/*LVGL task two_ main function of LVGL*/
void taskTwo(void * parameter){
	while(1){
		lv_task_handler(); 
		delay(5);
	}
  Serial.println("Ending task 2");
  vTaskDelete( NULL );
}

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

SdCard tf;

/*Change to your screen resolution*/
static const uint32_t screenWidth  = 240;
static const uint32_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( const char * buf )
{
   Serial.printf(buf);
   //Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.endWrite();

   lv_disp_flush_ready( disp );
}



void setBackLight(float duty)
{
  duty = constrain(duty, 0, 1);
  duty = 1 - duty;
  ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
}

void load_png()
{
   lv_obj_t * img_bg = lv_img_create(lv_scr_act());// delete the app_window
   LV_IMG_DECLARE(tits);
     Serial.println( "tits is printed" );
   lv_img_set_src(img_bg,&tits);
   lv_obj_align(img_bg,NULL,LV_ALIGN_CENTER,0);
}

/**
 * Open a GIF image from a file and a variable
 */
void lv_example_gif_1(void)
{
    LV_IMG_DECLARE(example);//img_bulb_gifnumber_game//number_game
    lv_obj_t * img;
    Serial.println( "tu pian xian shi le yi jing" );

    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, &example);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
   /*
    img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, "S:/example.bin");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    */
}
void LCD_init()
{
    //diaplay init
   ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
   ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);
   lv_init();
   //#if LV_USE_LOG != 0
   lv_log_register_print_cb( my_print ); /* register print function for debugging */
    tft.begin();          /* TFT init */
   tft.setRotation(4 ); /* Landscape orientation, flipped */
   
   lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

   /*Initialize the display*/
   static lv_disp_drv_t disp_drv;
   lv_disp_drv_init( &disp_drv );
   /*Change the following line to your display resolution*/
   disp_drv.hor_res = screenWidth;
   disp_drv.ver_res = screenHeight;
   disp_drv.flush_cb = my_disp_flush;
   disp_drv.draw_buf = &draw_buf;
   lv_disp_drv_register( &disp_drv );

}

void setup()
{
   Serial.begin( 115200 ); /* prepare for possible serial debug */
   LCD_init();
   Serial.println( "Hello Arduino! (V8.0.X)" );
   Serial.println( "I am LVGL_Arduino" );
   
  
   setBackLight(0.2);
   //load_png();
   lv_example_gif_1();
    Serial.println( "Setup done" );

    xTaskCreate(taskOne,"TaskOne",10000,NULL,1,NULL);                 
    xTaskCreate(taskTwo,"TaskTwo",10000,NULL,1,NULL);
    delay(300);
}

void loop()
{
   lv_task_handler(); /* let the GUI do its work */
}
