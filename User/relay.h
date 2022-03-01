#ifndef RELAY_H_
#define RELAY_H_

#ifndef RELAY_Cx_//���û�ж���
#define RELAY_Cx_ extern
#else
#define RELAY_Cx_
#endif

#include "stm32f10x.h"

/*config pin*/
#define RELAY_CLK    RCC_APB2Periph_GPIOB
#define RELAY_PIN    GPIO_Pin_1
#define RELAY_PORT   GPIOB


void RelayInit(void);
void RelaySet(int value);
char RelayGet(void);
#endif

