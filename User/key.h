#ifndef __KEY_H_
#define __KEY_H_
#include "stm32f10x.h"
#ifndef __KEY_C_
#define __KEY_C_  extern
#else 
#define __KEY_C_
#endif


/*config pin*/
#define KEY_CLK    RCC_APB2Periph_GPIOB
#define KEY_PIN    GPIO_Pin_0
#define KEY_PORT   GPIOB


#define KeyDown 0  //0- �͵�ƽ��Ϊ�ǰ���  1-�ߵ�ƽ��Ϊ�ǰ���
#define KeyPinIn GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN)

__KEY_C_ int KeyValueTemp[7];//�������� ���±�� �ɿ���� ���±�ǿ���  �ɿ���ǿ��� ����ʱ��(Ms) �ɿ�ʱ��(Ms)

void KeyInit(void);
void KeyWhile(void);
void KeyLoop(void);

#endif
