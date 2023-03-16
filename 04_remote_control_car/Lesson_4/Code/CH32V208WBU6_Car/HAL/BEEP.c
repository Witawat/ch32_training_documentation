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
 * @fn      HAL_BeepInit
 *
 * @brief   Initializes GPIOA that control beep.
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void HAL_BeepInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //GPIO Init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                       //PA6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                //�������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);                              //Ĭ�ϵ�ƽΪ�͵�ƽ���������ر�
}

/**************************************************************************************************
 * @fn      HAL_BeepState
 *
 * @brief   Beep voice control.
 *
 * @param   en - ENABLE/DISABLE
 *
 * @return  none
 **************************************************************************************************/
void HAL_BeepState(uint8_t en)
{
    if(en == ENABLE)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_6);        //��������
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);      //����������
    }
}

/******************************** endfile @ beep ******************************/
