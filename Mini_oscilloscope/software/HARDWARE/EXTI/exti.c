#include "exti.h"
#include "key.h"
#include "SBQ.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
//�ⲿ�жϷ������
void EXTIX_Init(void)
{
 
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

		KEY_Init();	 //	�����˿ڳ�ʼ��
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��


  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

   //GPIOB.5	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOB.6	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//GPIOB.7	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//GPIOB.8	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY3
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0~3���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
} 
u8 KEY_Value;
extern _oscill_dev oscilldev;
void EXTI9_5_IRQHandler(void)
{
		if(KEY0==0)	 //����KEY0
		{
			KEY_Value = 0;
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
		else if(KEY1 == 0)//����KEY1
		{
			KEY_Value = 1;
			EXTI_ClearITPendingBit(EXTI_Line6);
		}
		else if(KEY2 == 0)//����KEY2
		{
			KEY_Value = 2;
			EXTI_ClearITPendingBit(EXTI_Line7);
		}
		else if(KEY3 == 0)//����KEY3
		{
			KEY_Value = 3;
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
		else  ;
}
