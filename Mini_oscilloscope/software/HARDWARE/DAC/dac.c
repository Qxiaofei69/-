#include "dac.h"
#include "math.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DAC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//���Ҳ��������ڵĵ���
//#define POINT_NUM 32

/* �������� ---------------------------------------------------------*/
//const uint16_t Sine12bit[POINT_NUM] = {
//	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
//	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,	2661	, 2255	, 
//	1841	, 1435	, 1054	, 714		, 428		, 209		, 65		, 3			,
//	24		, 127		, 310		, 564		, 878		, 1240	, 1636	, 2048
//};
//uint32_t DualSine12bit[POINT_NUM];
#define PI  3.14159
#define DAC_DHR12R2_ADDRESS     0x40007414
uint32_t DualSine12bit[POINT_NUM];

//DACͨ��1�����ʼ��
void Dac2_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=0;			
	DAC_InitType.DAC_Trigger=DAC_Trigger_T2_TRGO;	//ʹ�ô������� TIM2��Ϊ����Դ
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
   
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��1

	
	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��ͨ��1 ��PA5���
	

   
     /* ʹ��DAC��DMA����   �� DAC1 �� 2 �Ĳ�����ͬ���ģ�����ֻҪ�� DMA ��
     DAC ͨ�� 2 �����������ɣ���ʹ�� DMA ����ͨ�� 2 ������ֵʱ�� ͬʱ����ͨ�� 1 ������*/
     DAC_DMACmd(DAC_Channel_2, ENABLE);
}
static void DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ72M */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* TIM2������ʱ������ */
	TIM_TimeBaseStructure.TIM_Period = (20-1);       	//��ʱ���� 2000 
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1);       	//Ԥ��Ƶ������Ƶ 72M / (0+1) = 72M
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    //ʱ�ӷ�Ƶϵ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* ����TIM2����Դ */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* ʹ��TIM2 */
	TIM_Cmd(TIM2, DISABLE);

}

static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* ʹ��DMA2ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	/* ����DMA2 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_ADDRESS;					//�������ݵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit ;				//�ڴ����ݵ�ַ DualSine12bit
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//���ݴ��䷽���ڴ�������
	DMA_InitStructure.DMA_BufferSize = POINT_NUM;																	//�����СΪPOINT_NUM�ֽ�	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//���ڴ����ڴ�ģʽ	

	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	/* ʹ��DMA2-4ͨ�� */
	DMA_Cmd(DMA2_Channel4, ENABLE);
}

void DAC_Mode_Init_2(void)
{
	u16 n;
	float outdata=0;
	Dac2_Init();
	DAC_TIM_Config();
	for(n=0;n<POINT_NUM;n++)
	{
		outdata = (sin(2*PI*n/POINT_NUM)+1)*2047;
        //printf("%f\r\n",outdata);
        DualSine12bit[n]=outdata;		
	}	
	DAC_DMA_Config();
}
