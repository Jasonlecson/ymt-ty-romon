#ifndef _PVD_Cc_
#define _PVD_Cc_

#ifndef _PVD_Cx_//���û�ж���
#define _PVD_Cx_ extern
#else
#define _PVD_Cx_
#endif

#include <stm32f10x.h>


void PVDInit(uint32_t PWR_PVDLevel);

#endif

