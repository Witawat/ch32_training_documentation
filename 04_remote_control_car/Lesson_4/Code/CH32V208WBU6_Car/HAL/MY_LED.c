/********************************** (C) COPYRIGHT *******************************
 * File Name          : BEEP.c
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
 * @fn      HAL_MyLEDInit
 *
 * @brief   Initializes GPIOC that control LED.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_MyLEDInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    //GPIO Init
    GPIO_InitStructure.GPIO_Pin = MY_LED_POWER_STATE_PIN|MY_LED_LINK_STATE_PIN|MY_LED_SLEEP_STATE_PIN; //POWER_STATE_PIN-PA0,LINK_STATE_PIN-PA1��SLEEP_STATE_PIN-PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                        //�������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, MY_LED_POWER_STATE_PIN);                          //Ĭ�ϵ�ƽΪ�͵�ƽ����ԴָʾLED����
    GPIO_SetBits(GPIOC, MY_LED_LINK_STATE_PIN|MY_LED_SLEEP_STATE_PIN);      //Ĭ�ϵ�ƽΪ�ߵ�ƽ������ָʾLED������ָʾLEDϨ��
}

/**************************************************************************************************
 * @fn      HAL_MyLEDState
 *
 * @brief   Control LED turn on/off.
 *
 * @param   pin - GPIO pin that control LED.
 *          en - ENABLE/DISABLE
 *
 * @return  none
 **************************************************************************************************/
void HAL_MyLEDState(uint16_t pin, uint8_t en)
{
    if(en == ENABLE)
    {
        GPIO_ResetBits(GPIOC, pin);         //����LED
    }
    else
    {
        GPIO_SetBits(GPIOC, pin);           //Ϩ��LED
    }
}

/******************************** endfile @ my_led ******************************/
