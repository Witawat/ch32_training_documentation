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
#include "string.h"

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/
s16 Calibrattion_Val = 0;           //ADCУ׼ֵ
JoystickState_t JoystickState;      //�ֱ�״̬

/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/

/*********************************************************************
 * @fn      HAL_ADC1Init
 *
 * @brief   Initializes ADC peripheral.
 *
 * @return  none
 **************************************************************************************************/
void HAL_ADC1Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef  ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //ADC���ʱ�䲻�ܳ���14M,����ADC��Ƶ����6��60M/6=10M,

    //GPIO Init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3;    //PA1-Channel1-left, PA3-Channel3-right
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           //ģ������
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    ADC_DeInit(ADC1);                                       //��λADC1,������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ

    //ADC1 Init
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      //ADC1�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           //ɨ��ģʽ�ر�
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     //����ת��ģʽ�ر�
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //����ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);                                     //ADC1��ʼ��

    //Enable ADC1
    ADC_Cmd(ADC1, ENABLE);                                  //ʹ��ADC1

    ADC_BufferCmd(ADC1, DISABLE);                           //ʹ��У׼ǰҪ�ر�Buffer
    ADC_ResetCalibration(ADC1);                             //ʹ�ܸ�λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));             //�ȴ���λУ׼����
    ADC_StartCalibration(ADC1);                             //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC1));                  //�ȴ�У׼����
    Calibrattion_Val = Get_CalibrationValue(ADC1);          //��ȡADC1У׼ֵ
    ADC_BufferCmd(ADC1, ENABLE);                            //��Buffer
}

/*********************************************************************
 * @fn      HAL_ADCCalibrattion
 *
 * @brief   Get Conversion Value.
 *
 * @param   val - Sampling value
 *
 * @return  val+Calibrattion_Val - Conversion Value.
 */
u16 HAL_ADCCalibrattion(s16 val)
{
    if((val + Calibrattion_Val) < 0)
        return 0;
    if((Calibrattion_Val + val) > 4095||val==4095)
        return 4095;
    return (val + Calibrattion_Val);
}

/*********************************************************************
 * @fn      HAL_GetADC1Value
 *
 * @brief   Get ADC1 value.
 *
 * @param   ch - ADC channel.
 *
 * @return  Conversion Value.
 */
u16 HAL_GetADC1Value(u8 ch)
{
    u16 temp;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_71Cycles5 );   //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��Ϊ71.5������
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                             //ʹ��ָ����ADC1�����ת����������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                     //�ȴ�ת������
    temp = ADC_GetConversionValue(ADC1);                                //ADC1�������ת�����
    temp = HAL_ADCCalibrattion((s16)temp);                              //��ֵУ׼

    return temp;
}

/*********************************************************************
 * @fn      HAL_GetADC1Average
 *
 * @brief   Get ADC1 average value.
 *
 * @param   ch - ADC channel.
 *          times - Get ADC value times.
 *
 * @return  Conversion Value.
 */
u16 HAL_GetADC1Average(u8 ch, u8 times)
{
    u32 temp_val=0;
    u8 t;

    for( t = 0; t < times; t++)
    {
        temp_val += HAL_GetADC1Value(ch);
    }

    return (temp_val / times);
}

/*********************************************************************
 * @fn      HAL_JoystickInit
 *
 * @brief   Initializes joysticks.
 *
 * @param   none
 *
 * @return  none
 */
void HAL_JoystickInit(void)
{
    HAL_ADC1Init();

    JoystickState.LeftJoystickState.dir = SERVO_DIRECTION_TURN_LEFT;
    JoystickState.LeftJoystickState.per = 0;
    JoystickState.RightJoystickState.dir = MOTOR_ROTATE_FORWARD;
    JoystickState.RightJoystickState.per = 0;
}

/*********************************************************************
 * @fn      HAL_JoystickState
 *
 * @brief   Start/Stop collecting Joystick ADC.
 *
 * @param   en - ENABLE/DISABLE
 *
 * @return  none
 */
void HAL_JoystickPressPercentageGet(void)
{
    uint16_t temp;

    temp = HAL_GetADC1Average(ADC_Channel_1, 8);
    if (temp < LEFT_JOYSTICK_MIDDLE)
    {
        JoystickState.LeftJoystickState.dir = SERVO_DIRECTION_TURN_RIGHT;
        JoystickState.LeftJoystickState.per = ((1000 * (LEFT_JOYSTICK_MIDDLE - temp) / (LEFT_JOYSTICK_MIDDLE - LEFT_JOYSTICK_RIGHT_MAX)) + 5) / 10;
    }
    else
    {
        JoystickState.LeftJoystickState.dir = SERVO_DIRECTION_TURN_LEFT;
        JoystickState.LeftJoystickState.per = ((1000 * (temp - LEFT_JOYSTICK_MIDDLE) / (LEFT_JOYSTICK_LEFT_MAX - LEFT_JOYSTICK_MIDDLE)) + 5) / 10;
    }

    temp = HAL_GetADC1Average(ADC_Channel_3, 8);
    if (temp > RIGHT_JOYSTICK_MIDDLE)
    {
        JoystickState.RightJoystickState.dir = MOTOR_ROTATE_BACKWARD;
        JoystickState.RightJoystickState.per = ((1000 * (temp - RIGHT_JOYSTICK_MIDDLE) / (RIGHT_JOYSTICK_BEHIND_MAX - RIGHT_JOYSTICK_MIDDLE)) + 5) / 10;
    }
    else
    {
        JoystickState.RightJoystickState.dir = MOTOR_ROTATE_FORWARD;
        JoystickState.RightJoystickState.per = ((1000 * (RIGHT_JOYSTICK_MIDDLE - temp) / (RIGHT_JOYSTICK_MIDDLE - RIGHT_JOYSTICK_FRONT_MAX)) + 5) / 10;
    }

    PRINT("Left dir:%d  per:%d\n", JoystickState.LeftJoystickState.dir, JoystickState.LeftJoystickState.per);
    PRINT("Right dir:%d  per:%d\n", JoystickState.RightJoystickState.dir, JoystickState.RightJoystickState.per);
}

/******************************** endfile @ motor ******************************/
