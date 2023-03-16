/********************************** (C) COPYRIGHT *******************************
 * File Name          : SERVO.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/11/22
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
#ifndef __SERVO_H
#define __SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */
#define SERVO_CONTROL_TIM_CLOCK                     100000                                              //������ƶ�ʱ��2ʱ��100KHz
#define SERVO_CONTROL_TIM_PRESCALER                 (SystemCoreClock / SERVO_CONTROL_TIM_CLOCK - 1)     //��ʱ��2��ʱ�ӷ�Ƶϵ��,Ƶ��Ϊ100K,����Ϊ0.01ms
#define SERVO_CONTROL_PWM_PERIOD                    (20 * SERVO_CONTROL_TIM_CLOCK / 1000)               //������Ƶ�PWM���ڹ̶�Ϊ20ms

#define SERVO_DIRECTION_DUTY_CYCLE_MAX_LEFT         80     //���������ռ�ձȼ���(ʵ��ֵ)
#define SERVO_DIRECTION_DUTY_CYCLE_FORWARD          140     //�������ǰ��ռ�ձȼ�����ʵ��ֵ��
#define SERVO_DIRECTION_DUTY_CYCLE_MAX_RIGHT        180      //���������ռ�ձȼ�����ʵ��ֵ��

#define SERVO_DIRECTION_TURN_LEFT                   0       //�����ת��־
#define SERVO_DIRECTION_TURN_RIGHT                  1       //�����ת��־

typedef struct
{
    uint8_t dir;        //���з���
    uint8_t per;        //�仯����
} ControlCommand_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/**
 * @brief   Initializes TIM2 output PWM.
 */
void HAL_TIM2Init(u16 arr, u16 psc, u16 ccp);

/**
 * @brief   Servo PWM Init.
 */
void HAL_ServoInit(void);

/**
 * @brief   Servo PWM Output State.
 */
void HAL_ServoState(uint8_t en);

/**
 * @brief   Servo turn forward.
 */
void HAL_ServoDirectionTurnForward(void);

/**
 * @brief   Change servo direction.
 */
void HAL_ServoDirectionControl(uint8_t dir, uint8_t per);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
