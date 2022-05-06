#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "adc.h"
#include "KEY.h"
#include "exti.h"
#include "timer.h"
#include "SBQ.h"

extern _oscill_dev oscilldev;//ʾ�������Խṹ��
extern u8 KEY_Value;//�����ж�ֵ

void begin(void);//������ʼ������
int main(void)
{
	delay_init();
	EXTIX_Init();//�����жϳ�ʼ��
	Adc_Init();//ADCͨ����ʼ��
	TIM3_PWM_Init(1000-1,72-1);//PWM  1M/1000=1khz �Ƚ�ֵ1000 ������ʼ�� PA6 
	
	OLED_Init();//OLED_SPI�ӿڳ�ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Clear();//����
	OLED_Refresh();//ˢ�Դ�����
	
	KEY_Value = 254;//����ֵ��ʼ��
	oscilldev.flag = 0;//ʾ����ADC�����Ƿ�ɼ����־λ
	oscilldev.tim = 10;//ʾ��������Ƶ������
	oscilldev.a = 0.8;//���������a
	oscilldev.b = 4096;//���������b
	
	begin();//������ʼ������
	while(1)
	{
		DrawOscillogram();//������ʾ	
	}
}

void begin(void)
{
	u8 t = 0;
	OLED_ShowString(4,16,"MINI",24);//��ʾ������ȥOLED.c�ļ���鿴����
	OLED_ShowChinese(52,16,0,24);
	OLED_ShowChinese(76,16,1,24);
	OLED_ShowChinese(100,16,2,24);
	OLED_DrawSquare(10,48,117,60);
	for(;t<108;t++)//��������ʾѭ��
	{
		OLED_DrawLine(10+t,49,10+t,59);
		OLED_Refresh();
		delay_ms(15);
	}
	delay_ms(100);
	OLED_Refresh();
	OLED_Clear();
}


