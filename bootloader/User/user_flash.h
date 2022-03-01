#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__



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




//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr);


//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);


//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);

void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);

#endif
