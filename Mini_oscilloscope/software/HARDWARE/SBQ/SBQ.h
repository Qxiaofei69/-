#ifndef _SBQ_H
#define _SBQ_H
#include "sys.h"

typedef struct 
{
	u16 buffer[128];//���ݴ����
	u8 	flag;		//�Ƿ����
	u32  tim;		//һ�����us
    float resolut;  //ÿ��ADԭʼֵ������ĵ�ѹ(��λΪmv)
	float a;		//���������a
	float b;		//���������b
}_oscill_dev;

void DrawOscillogram(void);
void Draw_Menu(void);

#endif
