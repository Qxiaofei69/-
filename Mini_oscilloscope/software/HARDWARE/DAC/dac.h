#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
#define POINT_NUM 256								    
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
//#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20)								    

void Dac2_Init(void);//�ػ�ģʽ��ʼ��		 	 
void DAC_Mode_Init_2(void);
#endif

















