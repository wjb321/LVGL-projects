#include "can_config.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
/*Mater Node*/
uint8_t MasterNumBC =0;
uint64_t MasterTimeTx =0;
uint8_t MASTER_DATA0,MASTER_DATA1,MASTER_DATA2,MASTER_DATA3,MASTER_DATA4,MASTER_DATA5,MASTER_DATA6,MASTER_DATA7;
/*Slave Node*/
uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;
void CanInit(void);
void CanFilter_0_Configure(void);
void GPIO_Configuration(void);
/*******************************************************************************
* Function Name  : CAN_mode_Configuration
* Description    : contains CAN mode and filter congiguration
* Input          : None
* Output         : None
* Return         : None
* Attention		   : 1) for testing, better use loopback mode, after testing then use normal mode
*******************************************************************************/

void CAN_ABSCONTROL_Configuration(void)
{
  CanInit();
 //CanFilter_0_Configure();
  CanFilter_ABS_Configure();
  NVIC0_Configuration();
  NVIC1_Configuration();
  //GPIO_Configuration(); // configure the can IO
}



void CanInit()
{
  CAN_InitTypeDef        CAN_InitStructure;  // CAN mode struct
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // enable can1 clock.
  /* CAN register init */
  CAN_DeInit(CAN1);// reset all registers of the peripheral CAN to their default values
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; /* 时间触发禁止, 时间触发：CAN硬件的内部定时器被激活，并且被用于产生时间戳 */
  CAN_InitStructure.CAN_ABOM = ENABLE; /* 自动离线禁止，自动离线：一旦硬件监控到128次11个隐性位，就自动退出离线状态。在这里要软件设定后才能退出 */
  CAN_InitStructure.CAN_AWUM = ENABLE;  /* 自动唤醒禁止，有报文来的时候自动退出休眠	*/
  CAN_InitStructure.CAN_NART = ENABLE; /* The message is retransmitted, and if it is transmitted in error, it is transmitted until it succeeds. */
  CAN_InitStructure.CAN_RFLM = DISABLE; /* receive FIFO lock, 1-- after locking, newly coming data will be ignored.0--接收到新的报文则覆盖前一报文	*/
  CAN_InitStructure.CAN_TXFP = DISABLE;  /* FIFO priority, 0 means decide by identifier, 1 means decided by request sequence	*/
  //CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack; /*better for testing. when 2 boards communication, then it is mode_normal 	*/
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; /* 模式	*/
  //CAN_InitStructure.CAN_Mode = CAN_Mode_Silent ;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      /* 重新同步跳宽，只有can硬件处于初始化模式时才能访问这个寄存器 */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;      /* PTS + PBS1 */
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;      /* PBS2 */
  CAN_InitStructure.CAN_Prescaler = 6;         /* 波特率预分频数 */
  CAN_Init(CAN1,&CAN_InitStructure);

}


void CanFilter_ABS_Configure()
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure1;  // CAN filter struct
  CAN_FilterInitStructure1.CAN_FilterActivation=ENABLE;
  CAN_FilterInitStructure1.CAN_FilterFIFOAssignment=1;  /* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure1.CAN_FilterNumber=1;     /* 过滤器0 */
  CAN_FilterInitStructure1.CAN_FilterMode=CAN_FilterMode_IdMask;  /* can be IDlist,IDmask, the mask is relatively complex*/
  CAN_FilterInitStructure1.CAN_FilterScale=CAN_FilterScale_32bit; /* 32位 */
  CAN_FilterInitStructure1.CAN_FilterIdHigh= ((((u32)mes4_ID<<21))&0xFFFF0000)>>16;
  CAN_FilterInitStructure1.CAN_FilterIdLow= (((u32)mes4_ID<<21)| CAN_ID_STD |CAN_RTR_DATA)&0xFFFF; //0x123
  CAN_ITConfig(CAN1,CAN_IT_FMP1, ENABLE);   /* 挂号中断, 进入中断后读fifo的报文函数释放报文清中断标志 */
  CAN_FilterInitStructure1.CAN_FilterMaskIdHigh  = 0xFFFF; // 0x123, 0x125, 0x127 can be selected
  CAN_FilterInitStructure1.CAN_FilterMaskIdLow   = 0xFFFF;
  CAN_FilterInit(&CAN_FilterInitStructure1);
}


void NVIC0_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; // mail x0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

void NVIC1_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure1;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure1.NVIC_IRQChannel = CAN1_RX1_IRQn ;   //// mail x1
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure1);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* CAN Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // pin reuse configure| RCC_APB2Periph_AFIO

  //GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); 
  
	/* Configure CAN pin: RX */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);   
//  
//	/* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	     
//				 
	
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 |GPIO_Pin_14;// | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}