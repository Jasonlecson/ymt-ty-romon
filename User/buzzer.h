#ifndef BUZZER_Cc_
#define BUZZER_Cc_

#ifndef BUZZER_Cx_//���û�ж���
#define BUZZER_Cx_ extern
#else
#define BUZZER_Cx_
#endif

#include "stm32f10x.h"

/*config pin*/
#define BUZZER_CLK    RCC_APB2Periph_GPIOA
#define BUZZER_PORT   GPIOA
#define BUZZER_PIN    GPIO_Pin_4
//#define BUZZER_CLK    RCC_APB2Periph_GPIOB
//#define BUZZER_PORT   GPIOB
//#define BUZZER_PIN    GPIO_Pin_8


void BuzzerInit(void);
void BuzzerSet(int value);

#endif

