#include <stdio.h>
#include "sbq.h"
#include "ADC.h"
#include "OLED.h"
#include "delay.h"
#include "exti.h"
#include "sys.h"

_oscill_dev oscilldev;
extern u8 KEY_Value;
//�߿���ʾ����
void Draw_BianKuang(void)
{	
  OLED_DrawLine(4,16,8,16);
	OLED_DrawLine(4,63,8,63);
	OLED_DrawLine(0,16,0,63);
	OLED_DrawLine(1,16,1,63);
	OLED_DrawLine(2,16,2,63);
	OLED_DrawLine(3,16,3,63);
	
	OLED_DrawLine(119,16,123,16);
	OLED_DrawLine(119,63,123,63);
	OLED_DrawLine(127,16,127,63);
	OLED_DrawLine(126,16,126,63);
	OLED_DrawLine(125,16,125,63);
	OLED_DrawLine(124,16,124,63);
}
//������ʾ����
void Draw_GeZi(void)
{
	u8 i;
	for(i = 4;i<124;i+=5)
	OLED_DrawPoint(i,40);
	for(i = 16;i<64;i+=5)
	OLED_DrawPoint(33,i);
	for(i = 16;i<64;i+=5)
	OLED_DrawPoint(63,i);
	for(i = 16;i<64;i+=5)
	OLED_DrawPoint(93,i);
}
//���ζ�ȡ/��ʾ����
void Draw_BoXing(void)
{
	u8 t;
	for(t=4;t<123;t++)//�ҹ�����128-4-4�����ݣ����������ĸ�������Ϊ�߿���ʾ��
	{
		OLED_DrawLine_1(t,(47-oscilldev.buffer[t]*47/4095-(((double)(4095-oscilldev.b)/4095)/2)*47)*oscilldev.a+16/oscilldev.a,t+1,(47-oscilldev.buffer[t+1]*47/4095-(((double)(4095-oscilldev.b)/4095)/2)*47)*oscilldev.a+16/oscilldev.a);
	}
}
u8 menu = 0;//ȷ���˵���ǰλ��
void Draw_CanShu(void)
{
	switch (menu)//�жϵ�ǰ�˵�λ��
	{
		case 0:break;
		case 1:OLED_DrawSquare(5,0,44,15);break;
		case 2:OLED_DrawSquare(53,0,80,15);break;
		case 3:OLED_DrawSquare(94,0,123,15);break;
		default:break;
	}	
	switch (oscilldev.tim)//�жϵ�ǰ������Ƶ�ʺ���ʾ
	{
		case 1:    OLED_ShowString(7,2," 1MHZ",12);break;
		case 10:   OLED_ShowString(7,2,"100KHZ",12);break;
		case 100:  OLED_ShowString(7,2,"10KHZ",12);break;
		case 1000: OLED_ShowString(7,2," 1KHZ",12);break;
		case 10000:OLED_ShowString(7,2,"100HZ",12);break;
		default:break;
	}
	switch ((u8)(oscilldev.a*10))//�жϵ�ǰ��ֵ���ʺ���ʾ
	{
	  case 10:OLED_ShowString(55,2,"*  1",12);break;
		case 9:OLED_ShowString(55,2,"*0.9",12);break;
		case 8:OLED_ShowString(55,2,"*0.8",12);break;
		case 7:OLED_ShowString(55,2,"*0.7",12);break;
		case 6:OLED_ShowString(55,2,"*0.6",12);break;
		case 5:OLED_ShowString(55,2,"*0.5",12);break;
		default:break;
	}
	OLED_ShowString(97,2,"MEUN",12);
//sprintf((char *)tmp_buf,"f:%dKHZ",100);
}
//�˵���ʾ����
void Draw_Menu(void)
{
	u8 t;//ѭ������
	u16 max = 0;//��¼��ǰ���ADCֵ
	float V_max = 0;//��¼��ǰ���ADCֵ��Ӧ�ĵ�ѹֵ
	u8 tmp_buf[10];//�Դ��ַ�������
	OLED_Clear();
	for(t = 0;t<4;t++) OLED_ShowChinese(t*16,0,t,16);
	OLED_ShowString(0,18," Regulator",12);
	for(t = 4;t<6;t++) OLED_ShowChinese(82+(t-4)*16,0,t,16);
	OLED_ShowString(90,18,"SET",12);
	for(t = 6;t<11;t++) OLED_ShowChinese((t-6)*16,32,t,16);
	V_max = 4096*3.3/oscilldev.b;
	if(V_max>=10)
		sprintf((char *)tmp_buf,"%.4fV",V_max);
	else
		sprintf((char *)tmp_buf," %.4fV",V_max);
	OLED_ShowString(72,36,tmp_buf,12);
	OLED_Refresh();
	while(1)
	{
			if(KEY_Value != 255)
			{
				switch (KEY_Value)
				{
					case 0://OK�Ƿ��ؼ�
						//����
						goto X;
					case 1://SET��У׼��
						for(t=0;t<255;t++)//ѭ�������ֵ
						{
							if(max<Get_Adc(ADC_Channel_1))
								max=Get_Adc(ADC_Channel_1);
							delay_us(1);
						}
						oscilldev.b = max;max = 0;
						V_max = 4096*3.3/oscilldev.b;
						if(V_max<10)
							sprintf((char *)tmp_buf," %.4fV",V_max);//��������ʾ
						else if(V_max>=100); 
						else
							sprintf((char *)tmp_buf,"%.4fV",V_max);
						OLED_ShowString(72,36,tmp_buf,12);
						OLED_Refresh();
						break;
					case 2://ʣ��������û�õ�
						goto X;
					case 3:
						goto X;
					default: break;
				}
				KEY_Value = 255;	
			}
			delay_ms(10);
	}
X:;
	OLED_Clear();
}
//������ʾ����
void DrawOscillogram(void)//������ͼ
{
	u8 t;
	
	if(oscilldev.flag == 0)//�ж��Ƿ�ɼ���
	{
		oscilldev.flag = 1;
		
		for(t=4;t<124;t++)//�洢AD��ֵ
		{
			oscilldev.buffer[t] =Get_Adc(ADC_Channel_1);
			delay_us(oscilldev.tim);//�ı�ADȡ�����
		}

		OLED_Clear();
		Draw_BianKuang();
		Draw_GeZi();
		Draw_BoXing();
		OLED_Refresh_345678();//��ˢ�²������򣬲�ˢ���ϲ�
		delay_ms(50);
		
		oscilldev.flag = 0;
	}
	else ;

	if(KEY_Value != 255)
	{
		switch (KEY_Value)
		{
			case 0:
				if(menu == 3)//����menu����
				{
					KEY_Value = 255;
					Draw_Menu();
				}
				else ;
				break;
			case 1:
				menu++;if(menu>3)menu=0;//�ı�˵�λ��
				break;
			case 2:
				//�ı�����+
				if(menu == 1)
				{
					if(oscilldev.tim<10000)oscilldev.tim*=10;
				}
				else if(menu == 2)
				{
					if(oscilldev.a<1)oscilldev.a+=0.1;
				}
				break;
			case 3:
				//�ı�����-
				if(menu == 1)
				{
					if(oscilldev.tim>10)oscilldev.tim/=10;
					else if(oscilldev.tim==10)oscilldev.tim=1;
				}
				else if(menu == 2)
				{
					if(oscilldev.a>0.6)oscilldev.a-=0.1;
				}				
				else ;
				break;
			default: break;
		}
		Draw_CanShu();
		OLED_Refresh_12();//��ˢ���ϲ�����ˢ�²�������
		KEY_Value = 255;	
	}
}

