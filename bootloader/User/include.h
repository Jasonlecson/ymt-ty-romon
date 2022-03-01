#ifndef __INCLUDE_H__
#define __INCLUDE_H__

//==========================================================================
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
//#include 	<intrins.h>
#include	<ctype.h>
#include	"stm32f10x.h"

#include	"user_flash.h"


//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1        //ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_IAP_SIZE 20         //BootLoader���ó����С(KB)
#define FLASH_DATA_SIZE 6         //�洢�û��������ó����С(KB)
//�û������С = (��FLASH���� - �洢�û��������ó����С - IAP���ó����С)/2
#define FLASH_USER_SIZE  ((STM32_FLASH_SIZE - FLASH_DATA_SIZE - FLASH_IAP_SIZE)/2)  //�û������С(KB)

#define STM32_FLASH_BASE 0x8000000 	//STM32 FLASH����ʼ��ַ

//�û��������е�ַ
#define FIREWARE_ADDR_L  (STM32_FLASH_BASE+1024*FLASH_IAP_SIZE)
//�û����򱸷ݵ�ַ
#define FIREWARE_ADDR_H  (STM32_FLASH_BASE+1024*(FLASH_USER_SIZE+FLASH_IAP_SIZE))
//�洢�û����ݵĵ�ַ :Flash���� FLASH_DATA_SIZE KB  ��Ϊ���ݴ洢�ĵ�ַ 
#define FLASH_DATA_ADDR  (STM32_FLASH_BASE + 1024*(STM32_FLASH_SIZE - FLASH_DATA_SIZE) + 10) 


//#define FLASH_RUNUSER_ADDR (FLASH_DATA_ADDR+0)       //�洢���е���һ�ݳ�����Ϣ 0:û�п������еĳ���;1:���еĵ�һ�ݳ���;2:���еĵڶ��ݳ���
#define FLASH_UPDATE_FLAGE_ADDR (FLASH_DATA_ADDR+2)  //�洢�ǲ����и��³���
//#define FLASH_UPDATE_STATUS_ADDR (FLASH_DATA_ADDR+4) //�洢���µ�״̬
//#define FLASH_UPDATE_VERSION1_ADDR (FLASH_DATA_ADDR+6) //�洢�ƶ˷����İ汾��,�汾���20���ַ�
#define FLASH_UPDATE_VERSION2_ADDR (FLASH_DATA_ADDR+26) //�洢�豸��ǰ�İ汾��,�汾���20���ַ�
//���ڵĴ洢���  FLASH_DATA_ADDR+46  ��ʼ


//(7+1K)+28K+28K
//#define         BASIC_FLASH_ADDR                        0x08000000

//#define         OFFSET_PARA                             0x1c00                                
//#define         OFFSET_FIRMWARE_L                       0x2000
//#define         OFFSET_FIRMWARE_H                       0x9000

//#define         PARA_ADDR                               (BASIC_FLASH_ADDR + OFFSET_PARA)
//#define         FIREWARE_ADDR_L                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_L)   //
//#define         FIREWARE_ADDR_H                         (BASIC_FLASH_ADDR + OFFSET_FIRMWARE_H)   //

#define         FIREWARE_UPDATE_FLAG                    0x55555555


#endif

