#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//��ȡ����2 


void KEY_Init(void);//IO��ʼ��
#endif
