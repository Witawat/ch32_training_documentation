/********************************** (C) COPYRIGHT *******************************
 * File Name          : SERVO.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/11/22
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "HAL.h"

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/


/**************************************************************************************************
 * @fn      HAL_TIM2Init
 *
 * @brief   Initializes TIM2 output PWM.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 **************************************************************************************************/
void HAL_TIM2Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //GPIO Init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                       //PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //TIM2 Init
    TIM_TimeBaseInitStructure.TIM_Period = arr;                     //�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //Ԥ��Ƶϵ��, ��������ʱ��Ƶ��=APB1/(psc+1), APB1=120MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //��Ӧ����δʹ�õ�
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);            //��ʱ��2��ʼ��

    //Channel1 Init
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //���ϼ���ʱ, TIMx_CNT < TIMx_CH1CVR �����Ч��ƽ�����������Ч��ƽ
    TIM_OCInitStructure.TIM_Pulse = ccp;                            //���ô�װ�벶��ȽϼĴ����ıȽ�ֵ TIMx_CH1CVR
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //�ߵ�ƽ��Ч
    TIM_OC1Init( TIM2, &TIM_OCInitStructure );                      //ͨ��1��ʼ��

    TIM_OC1PreloadConfig( TIM2, TIM_OCPreload_Disable );            //�Ƚϲ���Ĵ���1Ԥװ��ʧ��,��д��Ƚϲ���Ĵ����е�ֵ������������
    TIM_ARRPreloadConfig( TIM2, ENABLE );                           //�Զ���װ��ʹ��
    TIM_Cmd( TIM2, ENABLE );                                        //������ʹ��
}

/**************************************************************************************************
 * @fn      HAL_ServoInit
 *
 * @brief   Servo PWM Init.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_ServoInit(void)
{
    HAL_TIM2Init(SERVO_CONTROL_PWM_PERIOD, SERVO_CONTROL_TIM_PRESCALER, 0); //��ʱ�����ڣ�0.01ms,Ĭ��С����������ģʽ,��ʱ����������͵�ƽ
    PRINT("Servo init success\n");
}

/**************************************************************************************************
 * @fn      HAL_ServoState
 *
 * @brief   Servo PWM Output State.
 *
 * @param   en - Enable/Disable
 *
 * @return  none
 **************************************************************************************************/
void HAL_ServoState(uint8_t en)
{
    if(en == ENABLE)
    {
        TIM_SetCompare1(TIM2, SERVO_DIRECTION_DUTY_CYCLE_FORWARD); //�������ǰ
    }
    else
    {
        TIM_SetCompare1(TIM2, 0); //�ͷŶ������ʱ����������͵�ƽ
    }
}

/*********************************************************************
 * @fn      HAL_ServoDirectionTurnForward
 *
 * @brief   Servo turn forward.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_ServoDirectionTurnForward(void)
{
    TIM_SetCompare1(TIM2, SERVO_DIRECTION_DUTY_CYCLE_FORWARD); //�������ǰ
}

/*********************************************************************
 * @fn      HAL_ServoDirectionControl
 *
 * @brief   Change servo direction.
 *
 * @param   dir - turn left or turn right, 0: left, 1: right
 *          per - direction change percentage, 0~100
 *
 * @return  none
 **************************************************************************************************/
void HAL_ServoDirectionControl(uint8_t dir, uint8_t per)
{
    uint16_t duty_cycle;

    if(per > 100) //�ٷֱȴ���100
    {
        return;
    }

    if(dir == SERVO_DIRECTION_TURN_LEFT) //��ת
    {
        duty_cycle = SERVO_DIRECTION_DUTY_CYCLE_FORWARD - (SERVO_DIRECTION_DUTY_CYCLE_FORWARD - SERVO_DIRECTION_DUTY_CYCLE_MAX_LEFT) * per / 100;
    }
    else //��ת
    {
        duty_cycle = SERVO_DIRECTION_DUTY_CYCLE_FORWARD + (SERVO_DIRECTION_DUTY_CYCLE_MAX_RIGHT - SERVO_DIRECTION_DUTY_CYCLE_FORWARD) * per / 100;
    }

    PRINT("Servo dir:%d | per:%d | duty_cycle:%d\n", dir, per, duty_cycle);
    TIM_SetCompare1(TIM2, duty_cycle); //���ö�ʱ2ͨ��1�ıȽ�ֵ
}

/******************************** endfile @ servo ******************************/
