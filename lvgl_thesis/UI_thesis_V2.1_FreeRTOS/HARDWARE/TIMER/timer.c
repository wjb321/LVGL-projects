#include "timer.h"
#include "lvgl.h"
#include "timer.h"
#include "led.h"
#include "usart.h"
#include "math.h"
#include "chart_win.h"


#define ENCODER_RESOLUTION 11   /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 34 /*����ļ��ٱ�*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define CNT_INIT 0

extern int Pulse;


/*updating the chart*/

//vu32 NumHighFreq ;
int NumHighFreq =0; // for high frequency method
int rounds = 0;

/*7.7.22 add for testing*/

int read_encoder(void) //static
{
    int encoderNum = 0;
    encoderNum = (int)((int16_t)(TIM4->CNT)); /*CNTΪuint32, תΪint16*/
    TIM_SetCounter(TIM4, CNT_INIT);/*CNT���ֵ*/
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


//#define POINT_COUNT   	10  //�����������е����ݵ����
extern lv_coord_t series1_y[POINT_COUNT] ;
float TSpeed(int arr, int psc )
{ 
	//static int counter = 0;
	float Tv = 0;
	int freq = 0;
	freq = (int)(1.0/(((arr+1)*(psc +1)) / (72.0* pow(10.0,6.0))));
	Tv = 60.0* freq * Pulse/ (TOTAL_RESOLUTION * NumHighFreq *1.0);
  //	printf("freq is %d \r\n", freq);
  	printf("NumHighFreq is %d \r\n", NumHighFreq);
	
	//series1_y[counter] = Tv;
	//counter ++;
	//if(counter >10) counter = 0;
	printf("%.3f \n", Tv );
	NumHighFreq = 0;
	return Tv;
	//printf("Tv is %.3f \r\n", Tv);
}

   	  
void TIM1_Int_Init(int arr, int psc, int timer_enable_flag)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

  //��ʱ��TIM1��ʼ��
  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//TIM1_IRQn;  //TIM1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
  TIM_Cmd(TIM1, timer_enable_flag);  //ʹ��TIMx
}


void TIM2_Int_Init(u16 arr,u16 psc, u8 timer_enable_flag)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

  //��ʱ��TIM2��ʼ��
  TIM_TimeBaseStructure1.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure1.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure1); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


  TIM_Cmd(TIM2, timer_enable_flag );  //ʹ��TIMx
}


void TIM6_Int_Init(u16 arr,u16 psc, u8 timer_enable_flag)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

  //��ʱ��TIM6��ʼ��
  TIM_TimeBaseStructure1.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure1.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure1); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


  TIM_Cmd(TIM6, timer_enable_flag );  //ʹ��TIMx
}


void TIM2_IRQHandler(void)   //TIM3�ж�
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
  			NumHighFreq ++; //Tmethod use this vlaue
			 //Mspeed(arrValue, pscValue);
    }
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//		{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 

//		}
//}




//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��  // | RCC_APB2Periph_AFIO
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}

void TIM4_EncoderMode_Config(int Period)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;   
		NVIC_InitTypeDef NVIC_InitStructure;	

    //PB6 ch1  A,PB7 ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

    GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA6 PA7��������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                           

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = Period;//7;//7;  //�趨��������װֵ   TIMx_ARR = 1024*4-1 ����360�ߵ�
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM4ʱ��Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
    TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM_ICInitStructure.TIM_ICFilter =0xf;// 6;// 0xf//ѡ������Ƚ��˲��� 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3


		//����ж�����
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����TIM4����ж�
 
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
 
  TIM_SetCounter(TIM4,0); //TIM4->CNT=0
  TIM_Cmd(TIM4, ENABLE); 
}


extern int led0pwmval;
extern int arrValue;
extern int pscValue;

extern int SpeedDecrease;
extern lv_obj_t * chart1;
extern int speedflag;
void TIM1_UP_IRQHandler(void)   //TIM1�ж� TIM1_BRK_IRQHandler  TIM1_UP_IRQHandler
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM1�����жϷ������
    {
       LED0 = !LED0;
			speedflag = 1;
    }
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
}


float TMethodSpeed = 0;
int updateChart = 0;
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{    
			  //rounds ++;
		TMethodSpeed = TSpeed( arrValue, pscValue );
		series1_y[updateChart++]= TMethodSpeed;
			if(updateChart > POINT_COUNT -1 ) updateChart = 0;
		   
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
}



void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM5��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM5�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx					 
}


//��ʱ��3�жϷ������
void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM5->SR&TIM_IT_Update)//����ж�
	{
		lv_tick_inc(1);//lvgl��1ms����
	}				   
	TIM5->SR = (uint16_t)~TIM_IT_Update;
}

extern float TMethodSpeed ;
extern lv_obj_t * chart1;
extern lv_chart_series_t * series1;

void TIM6_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{    
			  //rounds ++;
		 LED1=!LED1;
		lv_chart_refresh(chart1);
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
}










