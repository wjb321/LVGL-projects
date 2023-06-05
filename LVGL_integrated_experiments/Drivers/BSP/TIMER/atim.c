/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2020-04-21
 * @brief       �߼���ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211217
 * ��һ�η���
 * V1.1 20211217
 * 1, ����atim_timx_comp_pwm_init����, ʵ������Ƚ�ģʽPWM�������
 * V1.2 20211217
 * 1, ����atim_timx_cplm_pwm_init����
 * 2, ����atim_timx_cplm_pwm_set����
 * V1.3 20211217
 * 1, ����atim_timx_pwmin_chy_process����
 * 2, ����atim_timx_pwmin_chy_init����
 * 3, ����atim_timx_pwmin_chy_restart�Ⱥ���
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"


//TIM_HandleTypeDef g_timx_npwm_chy_handle;     /* ��ʱ��x��� */

///* g_npwm_remain��ʾ��ǰ��ʣ�¶��ٸ�����Ҫ����
// * ÿ����෢��256������
// */
//static uint32_t g_npwm_remain = 0;

///**
// * @brief       �߼���ʱ��TIMX ͨ��Y ���ָ������PWM ��ʼ������
// * @note
// *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
// *              �߼���ʱ��ʱ�� = 72Mhz
// *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
// *              Ft=��ʱ������Ƶ��,��λ:Mhz
// *
// * @param       arr: �Զ���װֵ
// * @param       psc: ʱ��Ԥ��Ƶ��
// * @retval      ��
// */
//void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
//{
//    GPIO_InitTypeDef gpio_init_struct;
//    TIM_OC_InitTypeDef timx_oc_npwm_chy;   /* ��ʱ����� */
//    ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE();  /* TIMX ͨ��IO��ʱ��ʹ�� */
//    ATIM_TIMX_NPWM_CHY_CLK_ENABLE();       /* TIMX ʱ��ʹ�� */

//    g_timx_npwm_chy_handle.Instance = ATIM_TIMX_NPWM;                  /* ��ʱ��x */
//    g_timx_npwm_chy_handle.Init.Prescaler = psc;                       /* ��ʱ����Ƶ */
//    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;      /* ��������ģʽ */
//    g_timx_npwm_chy_handle.Init.Period = arr;                          /* �Զ���װ��ֵ */
//    g_timx_npwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /*ʹ��TIMx_ARR���л��� */
//    g_timx_npwm_chy_handle.Init.RepetitionCounter = 0;                 /* �ظ���������ʼֵ */
//    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);                         /* ��ʼ��PWM */

//    gpio_init_struct.Pin = ATIM_TIMX_NPWM_CHY_GPIO_PIN;                /* ͨ��y��CPIO�� */
//    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                           /* ����������� */
//    gpio_init_struct.Pull = GPIO_PULLUP;                               /* ���� */
//    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                     /* ���� */
//    HAL_GPIO_Init(ATIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);

//    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;                         /* ģʽѡ��PWM 1*/
//    timx_oc_npwm_chy.Pulse = arr / 2;                                  /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
//                                                                       /* ����Ĭ�����ñȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */
//    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                 /* ����Ƚϼ���Ϊ�� */
//    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &timx_oc_npwm_chy, ATIM_TIMX_NPWM_CHY); /* ����TIMxͨ��y */

//    HAL_NVIC_SetPriority(ATIM_TIMX_NPWM_IRQn, 1, 3);                   /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
//    HAL_NVIC_EnableIRQ(ATIM_TIMX_NPWM_IRQn);                           /* ����ITMx�ж� */

//    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);       /* ��������ж� */
//    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, ATIM_TIMX_NPWM_CHY);    /* ������ӦPWMͨ�� */
//}

///**
// * @brief       �߼���ʱ��TIMX NPWM����PWM����
// * @param       rcr: PWM�ĸ���, 1~2^32�η���
// * @retval      ��
// */
//void atim_timx_npwm_chy_set(uint32_t npwm)
//{
//    if (npwm == 0) return;

//    g_npwm_remain = npwm;                                                   /* ����������� */
//    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
//    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
//}

///**
// * @brief       �߼���ʱ��TIMX NPWM�жϷ�����
// * @param       ��
// * @retval      ��
// */
//void ATIM_TIMX_NPWM_IRQHandler(void)
//{
//    uint16_t npwm = 0;

//    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ */
//    if(__HAL_TIM_GET_FLAG(&g_timx_npwm_chy_handle, TIM_FLAG_UPDATE) != RESET)
//    {
//        if (g_npwm_remain > 256)            /* ���д���256��������Ҫ���� */
//        {
//            g_npwm_remain = g_npwm_remain - 256;
//            npwm = 256;
//        }
//        else if (g_npwm_remain % 256)       /* ����λ��������256��������Ҫ���� */
//        {
//            npwm = g_npwm_remain % 256;
//            g_npwm_remain = 0;              /* û�������� */
//        }

//        if (npwm) /* ������Ҫ���� */
//        {
//            ATIM_TIMX_NPWM->RCR = npwm - 1;                                         /* �����ظ������Ĵ���ֵΪnpwm-1, ��npwm������ */
//            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
//            __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
//        }
//        else
//        {
//            ATIM_TIMX_NPWM->CR1 &= ~(1 << 0); /* �رն�ʱ��TIMX��ʹ��HAL Disable�����PWMͨ����Ϣ���˴����� */
//        }

//        __HAL_TIM_CLEAR_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
//    }
//}


/*******************************�����Ǹ߼���ʱ������Ƚ�ģʽʵ�����**************************************/

TIM_HandleTypeDef g_timx_comp_pwm_handle;       /* ��ʱ��x��� */

/**
 * @brief       �߼���ʱ��TIMX ����Ƚ�ģʽ ��ʼ��������ʹ������Ƚ�ģʽ��
 * @note
 *              ���ø߼���ʱ��TIMX 4·����Ƚ�ģʽPWM���,ʵ��50%ռ�ձ�,��ͬ��λ����
 *              ע��,����������Ƚ�ģʽ,ÿ2���������ڲ������һ��PWM���,������Ƶ�ʼ���
 *              ����,���ǻ����Կ����ж����ж������޸�CCRx,�Ӷ�ʵ�ֲ�ͬƵ��/��ͬ��λ�Ŀ���
 *              �������ǲ��Ƽ���ôʹ��,��Ϊ����ܵ��·ǳ�Ƶ�����ж�,�Ӷ�ռ�ô���CPU��Դ
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_comp_pwm = {0};

    g_timx_comp_pwm_handle.Instance = ATIM_TIMX_COMP;                   /* ��ʱ��8 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc  ;                      /* ��ʱ����Ƶ */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ��������ģʽ */
    g_timx_comp_pwm_handle.Init.Period = arr;                           /* �Զ���װ��ֵ */
    g_timx_comp_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);                           /* ����Ƚ�ģʽ��ʼ�� */

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;                        /* �Ƚ����ģʽ��ת���� */
    timx_oc_comp_pwm.Pulse = 250 - 1;                                   /* ��������ȽϼĴ�����ֵ */
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                  /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_1); /* ��ʼ����ʱ��������Ƚ�ͨ��1 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);                 /* ͨ��1 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 500 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_2); /* ��ʼ����ʱ��������Ƚ�ͨ��2 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);                 /* ͨ��2 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 750 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_3); /* ��ʼ����ʱ��������Ƚ�ͨ��3 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);                 /* ͨ��3 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 1000 - 1;
    timx_oc_comp_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_4); /* ��ʼ����ʱ��������Ƚ�ͨ��4 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);                 /* ͨ��4 Ԥװ��ʹ�� */

    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_OC_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ATIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct;

        ATIM_TIMX_COMP_CLK_ENABLE();

        ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE();

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH2_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH3_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH4_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH4_GPIO_PORT, &gpio_init_struct);
    }
}


/*******************************�����Ǹ߼���ʱ�������������������ʵ�����**************************************/

TIM_HandleTypeDef g_timx_cplm_pwm_handle;                              /* ��ʱ��x��� */
TIM_BreakDeadTimeConfigTypeDef g_sbreak_dead_time_config = {0};        /* ����ʱ������ */

/**
 * @brief       �߼���ʱ��TIMX ������� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ���ø߼���ʱ��TIMX �������, һ·OCy һ·OCyN, ���ҿ�����������ʱ��
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIMX_CPLM_CLK_ENABLE();            /* TIMx ʱ��ʹ�� */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE();  /* ͨ��Xɲ�������ӦIO��ʱ��ʹ�� */
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_BKIN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);
    
    ATIM_TIMX_CPLM_CHYN_GPIO_REMAP();     /* ӳ�䶨ʱ��IO,PE���Ǳ��������ö�ʱ����Ĭ��IO����Ҫ���� */

    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;                       /* ��ʱ��x */
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;                            /* ��ʱ��Ԥ��Ƶϵ�� */
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* ��������ģʽ */
    g_timx_cplm_pwm_handle.Init.Period = arr;                               /* �Զ���װ��ֵ */
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 18Mhz */
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle);

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                               /* PWMģʽ1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                        /* OCy �͵�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                      /* OCyN �͵�ƽ��Ч */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                    /* ��MOE=0��OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                  /* ��MOE=0��OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX_CPLM_CHY);

    /* �����������������������ж� */
    g_sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_DISABLE;           /* ����ģʽ�Ĺر����״̬ */
    g_sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;          /* ����ģʽ�Ĺر����״̬ */
    g_sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                /* ���üĴ��������� */
    g_sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE;                /* ʹ��ɲ������ */
    g_sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       /* ɲ��������Ч�źż���Ϊ�� */
    g_sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* ʹ��AOEλ������ɲ���������Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);         /* ʹ��OCy��� */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);      /* ʹ��OCyN��� */
}

/**
 * @brief       ��ʱ��TIMX ��������Ƚ�ֵ & ����ʱ��
 * @param       ccr: ����Ƚ�ֵ
 * @param       dtg: ����ʱ��
 *   @arg       dtg[7:5]=0xxʱ, ����ʱ�� = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10xʱ, ����ʱ�� = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110ʱ, ����ʱ�� = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111ʱ, ����ʱ�� = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 18M = 55.56ns
 * @retval      ��
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    g_sbreak_dead_time_config.DeadTime = dtg;       /* ����ʱ������ */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);  /*��������ʱ��*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);  /* MOE=1,ʹ������� */
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;                  /* ���ñȽϼĴ��� */
}


/*******************************�����Ǹ߼���ʱ��PWM����ģʽ����**************************************/

TIM_HandleTypeDef g_timx_pwmin_chy_handle;   /* ��ʱ��x��� */

/* PWM����״̬(g_timxchy_cap_sta)
 * 0,û�гɹ�����.
 * 1,�Ѿ��ɹ�������
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM����״̬ */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM�����Ƶϵ�� */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM�ĸߵ�ƽ���� */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM�����ڿ�� */

/**
 * @brief       ��ʱ��TIMX ͨ��Y PWM����ģʽ ��ʼ������
 * @note
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 *              ��������ʼ����ʱ��: ʹ��psc=0, arr�̶�Ϊ65535. �õ�����ʱ��Ƶ��Ϊ72Mhz,����Լ13.8ns
 *
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    ATIM_TIMX_PWMIN_CHY_CLK_ENABLE();
    ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    AFIO_REMAP_PARTIAL(AFIO_EVCR_PORT_PC, AFIO_EVCR_PIN_PX6);       /* ����TIM8_CH1/PC6 */

    gpio_init_struct.Pin = ATIM_TIMX_PWMIN_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    g_timx_pwmin_chy_handle.Instance = ATIM_TIMX_PWMIN;             /* ��ʱ��8 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;                     /* ��ʱ��Ԥ��Ƶϵ�� */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* ��������ģʽ */
    g_timx_pwmin_chy_handle.Init.Period = 65535;                    /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);
    
    /* ��ģʽ���ã�IT1�������� */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET;                   /* ��ģʽ����λģʽ */
    slave_config.InputTrigger = TIM_TS_TI1FP1;                      /* ��ʱ�����봥��Դ��TI1FP1 */
    slave_config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* �����ؼ�� */
    slave_config.TriggerFilter = 0;                                 /* ���˲� */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config);

    /* IC1���������ش���TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* �����ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;       /* ѡ�������IC1ӳ�䵽TI1 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1;                 /* ����Ƶ */
    tim_ic_pwmin_chy.ICFilter = 0;                                 /* ���˲� */
    HAL_TIM_IC_ConfigChannel( &g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1 );
    
    /* IC2���������ش���TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING; /* �½��ؼ�� */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;      /* ѡ�������IC2ӳ�䵽TI1 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle,&tim_ic_pwmin_chy,TIM_CHANNEL_2);
    
    HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_IRQn, 1, 3);               /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ( ATIM_TIMX_PWMIN_IRQn );                     /* ����TIMx�ж� */
    
    /* TIM1/TIM8�ж��������벶���жϷ����� */
    if ( ATIM_TIMX_PWMIN == TIM1 || ATIM_TIMX_PWMIN == TIM8)
    {
        HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_CC_IRQn, 1, 3);        /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_CC_IRQn);                /* ����TIMx�ж� */
    }

    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2);
}

/**
 * @brief       ��ʱ��TIMX PWM����ģʽ ������������
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                                             /* �ر��ж� */

    g_timxchy_pwmin_sta = 0;                                        /* ����״̬,���¿�ʼ��� */
    g_timxchy_pwmin_psc = 0;                                        /* ��Ƶϵ������ */

    __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, 0);           /* �����ļ���Ƶ�ʲɼ�,�Եõ���õľ��� */
    __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* ���������� */
    
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);      /* ʹ��ͨ��1�����ж� */
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);   /* ʹ������ж� */
    __HAL_TIM_ENABLE(&g_timx_pwmin_chy_handle);                     /* ʹ�ܶ�ʱ��TIMX */

    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);   /* ���㲶��/�Ƚ�1�жϱ�־ */
    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);   /* ���㲶��/�Ƚ�2�жϱ�־ */
    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);/* ��������жϱ�־ */

    sys_intx_enable();                                              /* ���ж� */
}

/**
 * @brief       ��ʱ��TIMX ͨ��Y PWM����ģʽ �жϴ�����
 * @note
 *              ��ΪTIM1/TIM8���ж���жϷ�����,��TIM2~5/TIM12/TIM15����ͨ��ʱ��ֻ��1���жϷ���
 *              ����,Ϊ�˸��õļ���,���Ƕ��жϴ���ͳһ�ŵ�atim_timx_pwin_chy_process����������д���
 *
 * @param       ��
 * @retval      ��
 */
static void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;               /* ����PWMIN�������־ */

    if (g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);   /* ���㲶��/�Ƚ�1�жϱ�־ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);   /* ���㲶��/�Ƚ�2�жϱ�־ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);/* ��������жϱ�־ */
        __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);             /* ���������� */
        return ;
    }

    if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE))         /* ����������ж� */
    { 
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);       /* �������жϱ�� */

        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1) == 0)   /* û�з������ڲ����ж�,�Ҳ���δ��� */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0)   /* ��0 �� 1 */
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65535)               /* �Ѿ������,������������״̬ */
                {
                    g_timxchy_pwmin_psc = 0;                    /* ���»ָ�����Ƶ */
                }
                else if (g_timxchy_pwmin_psc > 32767)           /* ���ܱ����� */
                {
                    g_timxchy_pwmin_psc = 65535;                /* ֱ�ӵ�������Ƶϵ�� */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc; /* ���� */
                }
            }

            __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, g_timxchy_pwmin_psc); /* ���ö�ʱ��Ԥ��Ƶϵ�� */
            __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0);                     /* ���������� */
            __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);           /* ���㲶��/�Ƚ�1�жϱ�־ */
            __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);           /* ���㲶��/�Ƚ�2�жϱ�־ */
            __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);        /* ��������жϱ�־ */
            return ;
        }
    }

    if (sflag == 0)   /* ��һ�βɼ��������ж� */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* ��⵽�˵�һ�����ڲ����ж� */
        { 
            sflag = 1;  /* ��ǵ�һ�������Ѿ�����, �ڶ������ڲ�����Կ�ʼ�� */
        }
        
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);           /* ���㲶��/�Ƚ�1�жϱ�־ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);           /* ���㲶��/�Ƚ�2�жϱ�־ */
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);        /* ��������жϱ�־ */
        return ;                    /* ��ɴ˴β��� */
    }

    if (g_timxchy_pwmin_sta == 0)   /* ��û�гɹ����� */
    {
        if (__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1))         /* ��⵽�����ڲ����ж� */
        {
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2) + 1; /* �߶�ƽ������ֵ */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1) + 1; /* ���ڲ���ֵ */

            if (g_timxchy_pwmin_hval < g_timxchy_pwmin_cval)    /* �ߵ�ƽ����ض�С�����ڳ��� */
            {
                g_timxchy_pwmin_sta = 1;                        /* ��ǲ���ɹ� */

                g_timxchy_pwmin_psc = ATIM_TIMX_PWMIN->PSC;     /* ��ȡPWM�����Ƶϵ�� */
                
                if (g_timxchy_pwmin_psc == 0)                   /* ��Ƶϵ��Ϊ0��ʱ��, ������ȡ���� */
                {
                    g_timxchy_pwmin_hval++;                     /* ����ϵ��Ϊ1, ��1 */
                    g_timxchy_pwmin_cval++;                     /* ����ϵ��Ϊ1, ��1 */
                }

                sflag = 0;
                /* ÿ�β���PWM����ɹ���, ֹͣ����,����Ƶ���ж�Ӱ��ϵͳ������������ */
                ATIM_TIMX_PWMIN->CR1  &= ~(1 << 0);                             /* �رն�ʱ��TIMX */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);     /* ʹ��ͨ��1�����ж� */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC2);     /* ʹ��ͨ��2�����ж� */
                __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);  /* ʹ������ж� */

                __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);   /* ���㲶��/�Ƚ�1�жϱ�־ */
                __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);   /* ���㲶��/�Ƚ�2�жϱ�־ */
                __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);/* ��������жϱ�־ */
            }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        }
    }
    
    /* �������/�Ƚ�1�жϱ�־\����/�Ƚ�2�жϱ�־/�����жϱ�־ */
    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1);
    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC2);
    __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE);
}

/**
 * @brief       ��ʱ��TIMX ����/��� �жϷ�����
 *   @note      TIM1/TIM8��������������ڸ���/����жϷ���,����������һ������!
 *              ������ͨ��ʱ�������/���/����,��������������洦��!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}

/**
 * @brief       ��ʱ��TIMX ���벶�� �жϷ�����
 *   @note      ��TIM1/TIM8���������,������ͨ��ʱ��û������жϷ�����!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_CC_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}








