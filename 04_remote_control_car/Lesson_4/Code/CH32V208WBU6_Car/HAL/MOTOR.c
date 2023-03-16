/********************************** (C) COPYRIGHT *******************************
 * File Name          : MOTOR.c
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
MotorStatus_t MotorStatus;      //��¼���״̬

/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/
/*********************************************************************
 * @fn      HAL_MotorStatusInit
 *
 * @brief   Initializes motor status structure.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_MotorStatusInit(void)
{
    MotorStatus.dir = MOTOR_ROTATE_STOP;
    MotorStatus.speed = 0;
}

/*********************************************************************
 * @fn      HAL_TIM1Init
 *
 * @brief   Initializes TIM1 output PWM.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 **************************************************************************************************/
void HAL_TIM1Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );

    //GPIO Init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;          //PA8-IN1, PA9-IN3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    //TIM1 Init
    TIM_TimeBaseInitStructure.TIM_Period = arr;                     //�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //Ԥ��Ƶϵ��,��������ʱ��Ƶ��=APB2/(psc+1), APB2=120MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //��Ӧ����δʹ��
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);            //��ʱ��1��ʼ��

    //Channel1 Init
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //���ϼ���ʱ,TIMx_CNT < TIMx_CH1CVR �����Ч��ƽ�����������Ч��ƽ
    TIM_OCInitStructure.TIM_Pulse = ccp;                            //���ô�װ�벶��ȽϼĴ����ıȽ�ֵ TIMx_CH1CVR
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //������ԣ��ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;    //����״̬ʱ��OC1����͵�ƽ
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );                      //ͨ��1��ʼ��

    //Channel2 Init
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //���ϼ���ʱ��TIMx_CNT < TIMx_CH2CVR �����Ч��ƽ�����������Ч��ƽ
    TIM_OCInitStructure.TIM_Pulse = ccp;                            //���ô�װ�벶��ȽϼĴ����ıȽ�ֵ TIMx_CH2CVR
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //������ԣ��ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;    //����״̬ʱ��OC2����͵�ƽ
    TIM_OC2Init( TIM1, &TIM_OCInitStructure );                      //ͨ��2��ʼ��

    TIM_CtrlPWMOutputs(TIM1, DISABLE );                             //�����ʧ�ܣ�PWM�������Ϊ�͵�ƽ�������ת
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );            //�Ƚϲ���Ĵ���1Ԥװ��ʧ��,д��Ƚϲ���Ĵ����е�ֵ������������
    TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Disable );            //�Ƚϲ���Ĵ���2Ԥװ��ʧ��,д��Ƚϲ���Ĵ����е�ֵ������������
    TIM_ARRPreloadConfig( TIM1, ENABLE );                           //�Զ���װ��ʹ��
    TIM_Cmd( TIM1, ENABLE );                                        //������ʹ��
}

/*********************************************************************
 * @fn      HAL_MotorInit
 *
 * @brief   Initializes Motor control IO.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 **************************************************************************************************/
void HAL_MotorInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;    //PA10-IN2, PA12-IN4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //�����������Ϊͨ��IOʹ��
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_12);           //Ĭ�ϵ�ƽΪ�͵�ƽ

    HAL_TIM1Init(MOTOR_CONTROL_PWM_PERIOD, MOTOR_CONTROL_TIM_PRESCALER, MOTOR_SPEED_DUTY_CYCLE_SLOWEST); //��ʱ����ʼ��

    HAL_MotorStatusInit(); //��ʼ�����״̬
    PRINT("Motor init success\n");
}

/*********************************************************************
 * @fn      HAL_MotorStop
 *
 * @brief   Motor stop working.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_MotorStop(void)
{
    TIM_CtrlPWMOutputs(TIM1, DISABLE );                 //��ʱ��1���ʧ�ܣ�����Idle״̬
    GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_12);   //IO����͵�ƽ
    MotorStatus.dir = MOTOR_ROTATE_STOP;                //���µ��״̬
    MotorStatus.speed = 0;
}

/*********************************************************************
 * @fn      HAL_MotorSpeedControl
 *
 * @brief   Control motor working mode.
 *
 * @param   dir - car move direction.
 *          per - speed change percentage, 0~100.
 *
 * @return  none
 **************************************************************************************************/
void HAL_MotorSpeedControl(uint8_t dir, uint8_t per)
{
    if(per > 100)   //��ֵ����100
    {
        return;
    }

    PRINT("motor dir:%d per:%d\n", dir, per);
    if(dir == MOTOR_ROTATE_FORWARD)
    {
        if(MotorStatus.dir == MOTOR_ROTATE_FORWARD)
        {
            if(per == MotorStatus.speed)
            {
                return;
            }
            else
            {
                TIM_CtrlPWMOutputs(TIM1, DISABLE );                 //��ʱ��1���ʧ�ܣ�����Idle״̬
                TIM_SetCompare1(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��1�Ƚ�ֵ
                TIM_SetCompare2(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��2�Ƚ�ֵ
                TIM_CtrlPWMOutputs(TIM1, ENABLE );                  //��ʱ��1���ʹ��
                MotorStatus.speed = per;                            //���µ��״̬
            }
        }
        else
        {
            HAL_MotorStop();
            TIM_SetCompare1(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��1�Ƚ�ֵ
            TIM_SetCompare2(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��2�Ƚ�ֵ
            TIM_OC1PolarityConfig(TIM1, TIM_OCPolarity_High);   //���ö�ʱ��1ͨ��1������ԣ��ߵ�ƽ��Ч
            TIM_OC2PolarityConfig(TIM1, TIM_OCPolarity_High);   //���ö�ʱ��1ͨ��2������ԣ��ߵ�ƽ��Ч
            TIM_CtrlPWMOutputs(TIM1, ENABLE );                  //��ʱ��1���ʹ��
            GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_12);   //IO����͵�ƽ
            MotorStatus.dir = MOTOR_ROTATE_FORWARD;             //���µ��״̬
            MotorStatus.speed = per;
        }
    }
    else
    {
        if(MotorStatus.dir == MOTOR_ROTATE_BACKWARD)
        {
            if(per == MotorStatus.speed)
            {
                return;
            }
            else
            {
                TIM_CtrlPWMOutputs(TIM1, DISABLE );                 //��ʱ��1���ʧ�ܣ�����Idle״̬
                TIM_SetCompare1(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��1�Ƚ�ֵ
                TIM_SetCompare2(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��2�Ƚ�ֵ
                TIM_CtrlPWMOutputs(TIM1, ENABLE );                  //��ʱ��1���ʹ��
                MotorStatus.speed = per;                            //���µ��״̬
            }
        }
        else
        {
            HAL_MotorStop();
            TIM_SetCompare1(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��1�Ƚ�ֵ
            TIM_SetCompare2(TIM1, (MOTOR_SPEED_DUTY_CYCLE_FASTEST - MOTOR_SPEED_DUTY_CYCLE_SLOWEST) * per / 100); //���ö�ʱ��1ͨ��1�Ƚ�ֵ
            TIM_OC1PolarityConfig(TIM1, TIM_OCPolarity_Low);    //���ö�ʱ��1ͨ��1������ԣ��͵�ƽ��Ч
            TIM_OC2PolarityConfig(TIM1, TIM_OCPolarity_Low);    //���ö�ʱ��1ͨ��2������ԣ��͵�ƽ��Ч
            TIM_CtrlPWMOutputs(TIM1, ENABLE );                  //��ʱ��1���ʹ��
            GPIO_SetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_12);     //IO����ߵ�ƽ
            MotorStatus.dir = MOTOR_ROTATE_BACKWARD;            //���µ��״̬
            MotorStatus.speed = per;
        }
    }
}

/******************************** endfile @ motor ******************************/
