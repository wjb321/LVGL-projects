#include "timer.h"
#include "lvgl.h"
#include "QuickSort.h"
#include "timer.h"
#include "led.h"
#include "usart.h"
#include "math.h"

#define ENCODER_RESOLUTION 11   /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4       /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO 34 /*电机的减速比*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define CNT_INIT 0
extern u16 arrValue ;
extern u16 pscValue;
extern vu32 TMethodSpeed ;
extern int Pulse;
//vu32 NumHighFreq ;
vu32 NumHighFreq =0; // for high frequency method
int rounds = 0;

/*7.7.22 add for testing*/

int read_encoder(void) //static
{
    int encoderNum = 0;
    encoderNum = (int)((int16_t)(TIM4->CNT)); /*CNT为uint32, 转为int16*/
    TIM_SetCounter(TIM4, CNT_INIT);/*CNT设初值*/
	  printf("encoder %d \r\n",encoderNum);
    return encoderNum;
}

float Mspeed(int arr, int psc)  //psc here is 7199, so it is fine
{
	int encoderNum = 0;
  float Mv = 0;
	encoderNum = read_encoder() + 1496* rounds;
	Mv = 60* encoderNum/(TOTAL_RESOLUTION * (arr/pow(10.0,4.0))) ;
	//printf("rounds %d\n",rounds);
  //printf("encoderNum %d\n",encoderNum);
	//printf("%d\n",TIM4->CNT);
	//printf("%.3f\n",(TOTAL_RESOLUTION * (arr/pow(10.0,4.0))));
	printf("MvW %.3f\n",Mv);
	rounds = 0;
	return Mv;
	//printf("%.3f\n",Mv);
}


//#define POINT_COUNT   	10  //数据线所具有的数据点个数
//extern lv_coord_t series1_y[POINT_COUNT] ;
float TSpeed(int arr, int psc , int numfreq)
{ 
	static int counter = 0;
	float Tv = 0;
	int freq = 0;
	freq = (int)(1.0/(((arr+1)*(psc +1)) / (72.0* pow(10.0,6.0))));
	Tv = 60* freq * Pulse/ (TOTAL_RESOLUTION * numfreq *1.0);
  //	printf("freq is %d \r\n", freq);
  printf("NumHighFreq is %d \r\n", numfreq);
	
	//series1_y[counter] = Tv;
	//counter ++;
	if(counter >10) counter = 0;
	printf("%.3f \n", Tv );
	//numfreq = 0;
	return Tv;
	//printf("Tv is %.3f \r\n", Tv);
}

   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Int_Init(u16 arr,u16 psc, u8 timer_enable_flag)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

  //定时器TIM2初始化
  TIM_TimeBaseStructure1.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure1.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure1); //根据指定的参数初始化TIMx的时间基数单位

  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

  //中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


  TIM_Cmd(TIM2, timer_enable_flag );  //使能TIMx
}



void TIM2_IRQHandler(void)   //TIM3中断
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志

    
          NumHighFreq ++; //Tmethod use this vlaue
			 //Mspeed(arrValue, pscValue);
    }
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		LED1=!LED1;
		}
}




//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟  // | RCC_APB2Periph_AFIO
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}

void TIM4_EncoderMode_Config(int Period)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;   
		NVIC_InitTypeDef NVIC_InitStructure;	

    //PB6 ch1  A,PB7 ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能TIM4时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟

    GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA6 PA7浮空输入  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                           

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = Period;//7;//7;  //设定计数器重装值   TIMx_ARR = 1024*4-1 这是360线的
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4时钟预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
    TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
    TIM_ICInitStructure.TIM_ICFilter =0xf;// 6;// 0xf//选择输入比较滤波器 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3


		//溢出中断设置
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许TIM4溢出中断
 
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
 
  TIM_SetCounter(TIM4,0); //TIM4->CNT=0
  TIM_Cmd(TIM4, ENABLE); 
}

vu32 TMethodSpeed = 0;
int HighFreqTimes = 0;
int highNumberArray[5] = {0};
int highNumberArrayLen = sizeof(highNumberArray) / sizeof(int);
void TIM4_IRQHandler(void)   //TIM3中断
{ 
	int medianfrequency = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{    
			  //rounds ++;
			  
			  highNumberArray[HighFreqTimes] = NumHighFreq;
			  NumHighFreq = 0;
			  HighFreqTimes ++;
		    if(HighFreqTimes == 5)
				{
					HighFreqTimes = 0;
					medianfrequency = quickSort(highNumberArray, 0, highNumberArrayLen-1);
					printf("medianfrequency %d , %d\r\n",medianfrequency , highNumberArrayLen);
					TMethodSpeed = TSpeed( arrValue, pscValue, medianfrequency );
				  
				}
				
			 //TIM_SetCounter(TIM4, CNT_INIT);/*CNT设初值*/
       //printf("TIM4 %d \r\n", TIM4->CNT ); 
      			
			
		  
			//LED0 = !LED0 ;
		   
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
}
//void TIM5_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
//	
//	//定时器TIM5初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断

//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


//	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					 
//}


////定时器3中断服务程序
//void TIM5_IRQHandler(void)
//{ 		    		  			    
//	if(TIM5->SR&TIM_IT_Update)//溢出中断
//	{
//		lv_tick_inc(1);//lvgl的1ms心跳
//	}				   
//	TIM5->SR = (uint16_t)~TIM_IT_Update;
//}










