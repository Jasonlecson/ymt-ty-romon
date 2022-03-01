#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include <stm32f10x.h>


//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1        //ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_IAP_SIZE 20         //BootLoader���ó����С(KB)
#define FLASH_DATA_SIZE 6         //�洢�û��������ó����С(KB)
//�û������С = (��FLASH���� - �洢�û��������ó����С - IAP���ó����С)/2
#define FLASH_USER_SIZE  ((STM32_FLASH_SIZE - FLASH_DATA_SIZE - FLASH_IAP_SIZE)/2)  //�û������С(KB)

#define STM32_FLASH_BASE 0x8000000 	//STM32 FLASH����ʼ��ַ

//�������е�ַ
#define FIREWARE_ADDR_L  (STM32_FLASH_BASE+1024*FLASH_IAP_SIZE)
//���򱸷ݵ�ַ
#define FIREWARE_ADDR_H  (STM32_FLASH_BASE+1024*(FLASH_USER_SIZE+FLASH_IAP_SIZE))
//�洢�û����ݵĵ�ַ :Flash���� FLASH_DATA_SIZE KB  ��Ϊ���ݴ洢�ĵ�ַ 
#define FLASH_DATA_ADDR  (STM32_FLASH_BASE + 1024*(STM32_FLASH_SIZE - (FLASH_DATA_SIZE)) + 10) 


//���ݴ洢��ַ(Ĭ�ϴ洢�����4ҳ)
#define FLASH_DATA_ADDR_1  (STM32_FLASH_BASE+1024*124)
#define FLASH_DATA_ADDR_2  (STM32_FLASH_BASE+1024*125)
#define FLASH_DATA_ADDR_3  (STM32_FLASH_BASE+1024*126)
#define FLASH_DATA_ADDR_4  (STM32_FLASH_BASE+1024*127)


#define FLASH_DATA_ADDR_OFFSET 0

//#define FLASH_RUNUSER_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+0)       //�洢���е���һ�ݳ�����Ϣ 0:û�п������еĳ���;1:���еĵ�һ�ݳ���;2:���еĵڶ��ݳ���
#define FLASH_UPDATE_FLAGE_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+2)  //�洢�ǲ����и��³���
//#define FLASH_UPDATE_STATUS_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+4) //�洢���µ�״̬
//#define FLASH_UPDATE_VERSION1_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+6) //�洢�ƶ˷����İ汾��,�汾���20���ַ�
#define FLASH_UPDATE_VERSION2_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+26) //�洢�豸��ǰ�İ汾��,�汾���20���ַ�
//���ڵĴ洢���  FLASH_DATA_ADDR+46  ��ʼ

#define FLASH_USER_WATER_MODE_ADDR  (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+46)  //��ˮģʽ 46,47
#define FLASH_USER_WATER_COUNT_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+48)  //��ˮ���� 48,49
#define FLASH_USER_BOTANY_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+50) //ֲ������ 50 51 52 53 54 55 56 57

#define FLASH_USER_TEMP_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+54+4) //��ֵ:�¶�  54-55(L) 56-57(H)
#define FLASH_USER_HUMI_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+58+4) //��ֵ:ʪ��  58-59(L) 60-61(H)
#define FLASH_USER_ILL_ADDR  (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+62+4) //��ֵ:����  62-63(L) 64-65(H)
#define FLASH_USER_FERT_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+66+4) //��ֵ:����  66-67(L) 68-69(H)

#define FLASH_USER_DATA_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+70+4) //��ֲʱ��:70-77
#define FLASH_USER_LOVE_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+78+4) //����ֵ:78-79
#define FLASH_USER_HARVEST_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+80+4) //�ۼ��ջ�:80-81
//#define FLASH_USER_MATRIX_ADDR  (FLASH_DATA_ADDR+82) //����:82-83 ����

#define FLASH_USER_ONEDATA_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+84+4) //�״���ֲʱ��:84-91
#define FLASH_USER_MATRIX_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+92+4) //����ʱ��:92-99
#define FLASH_USER_NURTURE_ADDR (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+102+4) //���������� 102-103

/*ʹ�� flash_helper �洢����*/
#define FLASH_USE_FLASH_HELPER (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+104+4) //104 105
/*�Ƿ��ϱ�����ͳ���¼�*/
#define FLASH_USE_FLASH_ILLEVENT (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+106+4) //106 107
/*����ͳ�Ʊ���ֵ*/
#define FLASH_USE_FLASH_LIGHTALARM (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+108+4) //108 109 110 111
/*��������Ϣʱ����Ƿ񱨾�*/
#define FLASH_USE_FLASH_REPORTEVENT (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+112+4) //112 113
/*�����Ƿ��Զ��򿪲����*/
#define FLASH_USE_FLASH_FILLLIGHT (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+114+4) //114 115

#define FLASH_USE_SWITCH (FLASH_DATA_ADDR+FLASH_DATA_ADDR_OFFSET+116+4) //116 117




u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

char WriteFlashHalfWord(uint32_t WriteAddress,u16 data);
char FlashErasePage(uint32_t EraseAddress,u16 PageCnt);	
char FlashErasePages(uint32_t EraseAddress,u16 PageCnt);
void WriteFlashHalfWords(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);


/*****************************************************************************
�������� : Write_Flash
�������� : ��Flash�����ֽڱ��
������� : Addr_Sour:д���Դ����
           Addr_Obj:FLASH��Ŀ����ʼ��ַ
           Length:д����ֽ���
���ز��� : SUCCESS:д��ɹ�
            ERROR:д��ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Write_Flash(uint32_t Addr_Obj,uint8_t *Addr_Sour,uint16_t Length);

/*****************************************************************************
�������� : Earse_Flash
�������� : ��Flash����
������� : Addr_Obj:FLASH��Ŀ����ʼ��ַ
���ز��� : SUCCESS:�����ɹ�
            ERROR:����ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj);

/*****************************************************************************
�������� : Read_Flash
�������� : ��ȡFLASH������
������� : pulData:Դ����
           ulAddress:FLASH����ʼ��ַ
           Length:��ȡ���ֽ���
���ز��� : true:��ȡ�ɹ�
           false:��ȡʧ��
*****************************************************************************/
uint8_t Read_Flash(uint8_t *pulData,uint32_t ulAddress,uint32_t Length);

#endif

















