#ifndef __MAIN_H_
#define __MAIN_H_
#include <stm32f10x.h>

#ifndef __MAIN_C_
#define __MAIN_EXT_ extern
#else
#define __MAIN_EXT_
#endif


//��������ʹ��
#define MainBufferLen 1500
__MAIN_EXT_ unsigned char MainBuffer[MainBufferLen];//��������,ȫ��ͨ��
__MAIN_EXT_ u32  MainLen;      //ȫ��ͨ�ñ���
__MAIN_EXT_ unsigned char *MainString;    //ȫ��ͨ�ñ���
__MAIN_EXT_ u16 HardwareVersion;

__MAIN_EXT_ void TimerLoopFun(int value);

#endif
