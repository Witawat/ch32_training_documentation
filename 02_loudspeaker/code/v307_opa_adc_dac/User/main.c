/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*
 *@Note

   ADC�ɼ� OPA�Ŵ����˷��źţ�ͨ��DAC�����

   ADC���� timer3 ���������� 8KHz����ͨ��DMA1_Channel1 ����

   DAC���� timer8 ����ת����8KHz����ͨ�� DMA2_Channel3 ����

*/

#include "debug.h"


/* Global typedef */

/* Global define */
#define ADC_DATA_LENGTH  256


/* Global Variable */
volatile uint16_t adc_data_buffer_1[ADC_DATA_LENGTH] = {0};
volatile uint16_t adc_data_buffer_2[ADC_DATA_LENGTH] = {0};


volatile uint8_t adc_buffer_flag = 0; // ADC DAM�жϲɼ���ɣ��л�buffer��־



/**********************************DAC *****************************************/
void timer8_init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter =  0x00;
    TIM_TimeBaseInit( TIM8, &TIM_TimeBaseInitStructure);

    TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);
    TIM_Cmd(TIM8, ENABLE);
}

void dac_dma_init(void)
{
    DMA_InitTypeDef DMA_InitStructure={0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    DMA_StructInit( &DMA_InitStructure);
    /* Note:DAC2--->DMA1.CH3   DAC2--->DMA2.CH4 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(DAC->R12BDHR1);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc_data_buffer_1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = ADC_DATA_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
//    DMA_Cmd(DMA2_Channel3, ENABLE);
}

void dac_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    DAC_InitTypeDef  DAC_InitType = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; /* ģ������ */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);

    /* timer8 ���� */
    DAC_InitType.DAC_Trigger = DAC_Trigger_T8_TRGO;

    /* ��ʹ�����ǲ���α���� */
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;

    /* �������ǲ���ֵ */
//    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;

    /* ʹ�����Buffer����������迹  */
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

    DAC_Init(DAC_Channel_1, &DAC_InitType);
    DAC_Cmd(DAC_Channel_1, ENABLE);

    /* ʹ��DAC DMA */
    dac_dma_init();
    DAC_DMACmd(DAC_Channel_1, ENABLE);

    /* ��ʱ��8��ʼ�� */
    timer8_init(125,72-1); //8KHz
}


/***************************OPA**************/
/* OPA ��ʼ��  */
void opa_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    OPA_InitTypeDef  OPA_InitStructure={0};

    /* opa4 ��ʼ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    OPA_InitStructure.OPA_NUM=OPA4;
    OPA_InitStructure.PSEL=CHP1;
    OPA_InitStructure.NSEL=CHN1;
    OPA_InitStructure.Mode=OUT_IO_OUT0;
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd(OPA4,ENABLE);

    /* opa1 ��ʼ�� */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OPA_InitStructure.OPA_NUM=OPA1;
    OPA_InitStructure.PSEL=CHP0;
    OPA_InitStructure.NSEL=CHN0;
    OPA_InitStructure.Mode=OUT_IO_OUT0;
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd(OPA1,ENABLE);
}

void timer3_init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);

    TIM_Cmd(TIM3,ENABLE);
}


void adc_dma_init(void)
{

    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_data_buffer_1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_DATA_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);


    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 0xf0);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    DMA_Cmd(DMA1_Channel1,ENABLE);

}


void adc_init(void)
{
    /* ADC ͨ��1 ���ų�ʼ�� */
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC1 ģ���ʼ�� */
    ADC_InitTypeDef ADC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;  //����ģʽ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1,ENABLE);
    ADC_BufferCmd(ADC1, ENABLE);   //enable buffer

    /* ADC DMA ��ʼ�� */
    adc_dma_init();
    ADC_DMACmd(ADC1, ENABLE);

    /* ʹ���ⲿ����ģʽ */
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    /* timer3 ��ʼ�� */
    timer3_init(125,72-1); //8KHz
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	Delay_Init();
	USART_Printf_Init(512000);
	printf("SystemClk:%d\r\n",SystemCoreClock);

	// ��Ƶ����ʹ������
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB,GPIO_Pin_12);


	opa_init();

	adc_init();

	dac_init();

	/* ����ADC1 ������ ת��˳���ͨ������ʱ��  */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5 );       /* ADC1 ͨ��1  */

	while(1)
    {
	    ;
	}
}

/* ADC DMA �жϷ����� */
__attribute__((interrupt("WCH-Interrupt-fast")))
void DMA1_Channel1_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC1);

    DMA1_Channel1->CFGR &= (uint16_t)(~DMA_CFGR1_EN); //�޸� MADDR/CNTR ʱ��Ҫ�� �ر� DMA CFGR ENλ
    DMA2_Channel3->CFGR &= (uint16_t)(~DMA_CFGR1_EN);

    if(adc_buffer_flag == 0)
    {
        adc_buffer_flag = 1;
        DMA1_Channel1->MADDR = (uint32_t)adc_data_buffer_2;// adc_data_buffer_1�ɼ���ɣ��л�adc_data_buffer_2
        DMA2_Channel3->MADDR = (uint32_t)adc_data_buffer_1;//dac �����
    }
    else
    {
        adc_buffer_flag = 0;
        DMA1_Channel1->MADDR = (uint32_t)adc_data_buffer_1;  // adc_data_buffer_2�ɼ���ɣ��л�adc_data_buffer_1
        DMA2_Channel3->MADDR = (uint32_t)adc_data_buffer_2;  //dac ���

    }

    DMA2_Channel3->CNTR = ADC_DATA_LENGTH; //���� DMA���䳤��
    DMA2_Channel3->CFGR |= DMA_CFGR1_EN;   //����DAC DMA

    DMA1_Channel1->CNTR = ADC_DATA_LENGTH; //���� DMA���䳤��
    DMA1_Channel1->CFGR |= DMA_CFGR1_EN;   //����ADC DMA
}



