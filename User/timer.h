#ifndef __TIMER_H
#define __TIMER_H

#ifndef _TIME_C_//���û�ж���
#define _TIME_Ex_ extern
#else
#define _TIME_Ex_
#endif

#include "stm32f10x.h"

#define TIMER_COMPARE_SEC  1000 //1S
#define TIMER_COMPARE_3SEC  3000 //3S
#define TIMER_COMPARE_60SEC  60000 //60S
#define TIMER_COMPARE_3MIN  180000 //3 min

#define TIMER_COMPARE_20MIN  1200000 //20 min
//#define TIMER_COMPARE_20MIN  60000 //20 min

#define TIMER_COMPARE_HALF_HOUR  1800000 //30 min
#define TIMER_COMPARE_1HOUR  3600000 //1 hour
#define TIMER_COMPARE_3HOUR  10800000 //3 hour

_TIME_Ex_ int timer2Value;
_TIME_Ex_ int timer2Value1;
_TIME_Ex_ int timer2Value2;//ÿ��1S��ʱ���ת��Ϊʱ��
_TIME_Ex_ int timer2Value3;//ÿ��1S��ȡһ������״̬
_TIME_Ex_ int timer2_watering;//���ƽ�ˮ
_TIME_Ex_ u32 timer2_auto_watering;//�����Զ���ˮ

_TIME_Ex_ int timer2_water_stage;//���Ʋɼ�ˮλ������ʱ��

_TIME_Ex_ u32 timer2_event_appear;//�¼��ϱ�
_TIME_Ex_ int timer2_appear1;//�����ϱ�Ƶ��
_TIME_Ex_ int timer2_appear2;//�����ϱ�Ƶ��
_TIME_Ex_ int timer2_appear3;//�����ϱ�Ƶ��


_TIME_Ex_ int TimerDataValue1;//��ʱ��ȡʱ���


_TIME_Ex_ void Timer2Init(void);
#endif

