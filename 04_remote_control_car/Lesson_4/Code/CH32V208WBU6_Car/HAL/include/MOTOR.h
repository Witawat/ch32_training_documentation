/********************************** (C) COPYRIGHT *******************************
 * File Name          : MOTOR.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/11/22
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * CONSTANTS
 */
#define MOTOR_CONTROL_TIM_CLOCK                     2500000                                             //������ƶ�ʱ��1ʱ��2500KHz
#define MOTOR_CONTROL_TIM_PRESCALER                 (SystemCoreClock / MOTOR_CONTROL_TIM_CLOCK - 1)     //��ʱ��1��ʱ�ӷ�Ƶϵ��,Ƶ��Ϊ2500K,����Ϊ0.04us
#define MOTOR_CONTROL_PWM_PERIOD                    (40 * MOTOR_CONTROL_TIM_CLOCK / 1000000)            //������Ƶ�PWM���ڹ̶�Ϊ40us

#define MOTOR_SPEED_DUTY_CYCLE_FASTEST              100     //���ת�����ռ�ձȼ���
#define MOTOR_SPEED_DUTY_CYCLE_SLOWEST              0       //���ת������ռ�ձȼ���

#define MOTOR_ROTATE_FORWARD                        0       //С��ǰ��
#define MOTOR_ROTATE_BACKWARD                       1       //С������
#define MOTOR_ROTATE_STOP                           2       //С��ֹͣ

/*********************************************************************
 * TYPEDEFS
 */
typedef struct
{
    uint8_t dir;            //С�����з���
    uint8_t speed;          //С�������ٶȣ���ռ�ձȱ���
} MotorStatus_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

/**
 * @brief   Initializes motor status structure.
 */
void HAL_MotorStatusInit(void);

/**
 * @brief   Initializes TIM1 output PWM.
 */
void HAL_TIM1Init(u16 arr, u16 psc, u16 ccp);

/**
 * @brief   Initializes Motor control IO.
 */
void HAL_MotorInit(void);

/**
 * @brief   Motor stop working.
 */
void HAL_MotorStop(void);

/**
 * @brief   Control motor working mode.
 */
void HAL_MotorSpeedControl(uint8_t dir, uint8_t per);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
