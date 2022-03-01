#include "stmflash.h"

FLASH_Status FlashStatus;


//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			
			for(i=0;i<STM_SECTOR_SIZE;i+=2)//�ٴ��ж��Ƿ��в���ʧ��
			{
			  if(STMFLASH_ReadHalfWord(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE+i)!=0XFFFF)
				{
				  FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
					break;
				}
			}
			
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}
		else 
		{
			 STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
		}
				
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
			pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
			NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif




void WriteFlashHalfWords(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	
	FlashStatus = FLASH_BUSY;//����Ϊæ
	if(FLASH_GetStatus() == FLASH_COMPLETE)//���Բ���Flash
	{
		FLASH_Unlock();		
		for(i=0;i<NumToWrite;i++)
		{
			FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
			WriteAddr+=2;//��ַ����2.
		}  
		FLASH_Lock();//����
	}
} 


/**
* @brief  ָ����ַд��һ��16λ����
* @waing  д��ĵ�ַ���Ȳ���
* @param  WriteAddress  д��ĵ�ַ
* @param  data          д�������
* @param  None
* @retval 0:�ɹ�
* @example 
**/
char WriteFlashHalfWord(uint32_t WriteAddress,u16 data)
{
	FlashStatus = FLASH_BUSY;//����Ϊæ
	if(FLASH_GetStatus() == FLASH_COMPLETE)//���Բ���Flash
	{
		FLASH_Unlock();		
		FlashStatus = FLASH_ProgramHalfWord(WriteAddress,data);//д������
		
		if(FlashStatus == FLASH_COMPLETE)//�������
		{
			if(STMFLASH_ReadHalfWord(WriteAddress) == data)//�����ĺ�д���һ��
				FlashStatus = 0;//������д���һ��
			else
				FlashStatus = 5;
		}
		FLASH_Lock();//����
	}
	return FlashStatus;
} 


//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}


/**
* @brief  ����Flash  
* @param  EraseAddress  �����ĵ�ַ
* @param  PageCnt       ���������ļ�ҳ(һҳ����1024����)
* @param  None
* @retval 4:�ɹ�
* @example 
* @waing  ���оƬ��Flash�ǰ���2048��ҳ,����1ҳ�ڶ�������Ӧ����д2
**/
char FlashErasePage(uint32_t EraseAddress,u16 PageCnt)
{
	u32 i=0;
	u32 secpos;	   //������ַ
	if(EraseAddress<STM32_FLASH_BASE||(EraseAddress>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return 0;//�Ƿ���ַ
	secpos = EraseAddress-STM32_FLASH_BASE;//ʵ�ʵ�ַ
	secpos = secpos/STM_SECTOR_SIZE;//������ַ 
	if(STM_SECTOR_SIZE==2048){PageCnt=PageCnt/2;}
	if(FLASH_GetStatus() == FLASH_COMPLETE)//���Բ���Flash
	{
		FLASH_Unlock();
		for(i=0;i<PageCnt;i++)
		{
			FlashStatus = FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			if(FlashStatus != FLASH_COMPLETE) break;
			secpos++;
		}
		FLASH_Lock();//����
	}	
	return FlashStatus;
} 



/**
* @brief  ����Flash  
* @param  EraseAddress  �����ĵ�ַ
* @param  PageCnt       ���������ļ�ҳ
* @param  None
* @retval 4:�ɹ�
* @example 
* @waing  
**/
char FlashErasePages(uint32_t EraseAddress,u16 PageCnt)
{
	u32 i=0;
	u32 secpos;	   //������ַ
	if(EraseAddress<STM32_FLASH_BASE||(EraseAddress>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return 0;//�Ƿ���ַ
	secpos = EraseAddress-STM32_FLASH_BASE;//ʵ�ʵ�ַ
	secpos = secpos/STM_SECTOR_SIZE;//������ַ 
//	if(STM_SECTOR_SIZE==2048){PageCnt=PageCnt/2;}
	if(FLASH_GetStatus() == FLASH_COMPLETE)//���Բ���Flash
	{
		FLASH_Unlock();
		for(i=0;i<PageCnt;i++)
		{
			FlashStatus = FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			if(FlashStatus != FLASH_COMPLETE) break;
			secpos++;
		}
		FLASH_Lock();//����
	}	
	return FlashStatus;
}



/*****************************************************************************
�������� : FlashProgramU8
�������� : ��Flash�����ֽڱ��
������� : pulData:д���Դ����
           ulAddress:FLASH��Ŀ����ʼ��ַ
           ulCount:д����ֽ���
���ز���: SUCCESS:д��ɹ�
          ERROR:д��ʧ��
*****************************************************************************/
static uint8_t FlashProgramU32(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t Wrdata;
  
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                      //�����ֽڸ���Ϊ4�ı���   
  
  while(ulCount) 
  {
    Wrdata  = *pulData ++;                                             
    Wrdata |= (*pulData ++) << 8;
    Wrdata |= (*pulData ++) << 16;
    Wrdata |= (*pulData ++) << 24;                                 //��4���ֽ���ϳ�һ��������    
    
    FLASH_ProgramWord(ulAddress,Wrdata);
    
    ulAddress += 4;                                                 
    ulCount -= 4;
  }
  
  return(SUCCESS);
}
/*****************************************************************************
�������� : VerifyData
�������� : У��д��FLASH������
������� : pulData       Դ����
           ulAddress     FLASH����ʼ��ַ
           ulCount       У����ֽ���
���ز��� : true          У��ɹ�
           false         У��ʧ��
*****************************************************************************/
static uint8_t VerifyData(uint8_t *pulData,uint32_t ulAddress,uint32_t ulCount)
{
  uint32_t ReData;
  if(ulCount % 4 != 0)
    ulCount += (4 - ulCount % 4);                                         /* ����У����ֽڸ���Ϊ4�ı���  */

  while(ulCount) 
  {
    ReData = (*(__IO uint32_t*)ulAddress);
    
    if(*pulData != (ReData &0xff)) 
    {                         /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 8) &0xff)) 
    {                         /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 16) &0xff)) 
    {                        /* У���ֽ�����                 */
      return ERROR;
    }
    if(*(++pulData) != ((ReData >> 24) &0xff)) 
    {                        /* У���ֽ�����                 */
      return ERROR;
    }
    
    ulAddress += 4;                                                     /* ����ָ����Flash��ַ          */
    ulCount -= 4;                                                       /* ������У����ֽڸ���         */
    pulData++;                                                          /* ����Դ����ָ��               */
  }
  return SUCCESS;
}

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
uint8_t Write_Flash(uint32_t Addr_Obj,uint8_t *Addr_Sour,uint16_t Length)
{
  FLASH_Unlock();
  
  if(FlashProgramU32(Addr_Sour,Addr_Obj,Length) == ERROR) 
  {
    FLASH_Lock();
    return ERROR;
  } 
  
  if(VerifyData(Addr_Sour,Addr_Obj,Length) == ERROR) 
  {
    FLASH_Lock();
    return ERROR;
  } 
  FLASH_Lock();
  
  return SUCCESS;
}
/*****************************************************************************
�������� : Earse_Flash
�������� : ��Flash����
������� : Addr_Obj:FLASH��Ŀ����ʼ��ַ
���ز��� : SUCCESS:�����ɹ�
            ERROR:����ʧ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Earse_Flash(uint32_t Addr_Obj)
{
  FLASH_Unlock();
  
  if(FLASH_ErasePage(Addr_Obj) != FLASH_COMPLETE)
  {
    FLASH_Lock();
    return ERROR;
  }
  
  FLASH_Lock();
  
  return SUCCESS;
}

/*****************************************************************************
�������� : Read_Flash
�������� : ��ȡFLASH������
������� : pulData:Դ����
           ulAddress:FLASH����ʼ��ַ
           Length:��ȡ���ֽ���
���ز��� : true:��ȡ�ɹ�
           false:��ȡʧ��
*****************************************************************************/
uint8_t Read_Flash(uint8_t *pulData,uint32_t ulAddress,uint32_t Length)
{
  uint32_t ReData;
  uint32_t cnt = 0;
  
  while(cnt < Length)
  {
    ReData = (*(__IO uint32_t*)ulAddress);
    
    *pulData ++ = (ReData & 0xff); 
    cnt ++;
    if(cnt >= Length)
      break;
    
    *pulData ++ = ((ReData >> 8) & 0xff);
    cnt ++;
    if(cnt >= Length)
      break;

    *pulData ++ = (ReData >> 16) & 0xff;
    cnt ++;
    if(cnt >= Length)
      break;

    *pulData ++ = (ReData >> 24) & 0xff;
    cnt ++;
   
    ulAddress += 4;
  }
  return SUCCESS;
}
