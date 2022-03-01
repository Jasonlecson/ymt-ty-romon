/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: main.c
**��        ��: ������
**ʹ �� ˵ �� : ��
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
**
**-----------------------------------------------------------------------------
******************************************************************************/
#define MAIN_GLOBAL
#include "include.h"

uint8_t value[1024];

/**
* @brief   ���ڳ�ʼ��
* @param   bound  ������
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void UartInit(u32 bound1){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO , ENABLE);
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //USART ��ʼ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_InitStructure.USART_BaudRate = bound1;//���ڲ�����
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	/*����--1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
}

/**
* @brief  ���ڷ����ַ�������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStr(unsigned char *c,uint32_t cnt)
{
	while(cnt--)
	{
		USART_SendData(USART1, *c++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
	}
}


/*****************************************************************************
�������� : NVIC_Configuration
�������� : NVIC����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0);
  
  //Configure the NVIC Preemption Priority Bits
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
/*****************************************************************************
�������� : Exist_MainProgram
�������� : �жϳ����Ƿ����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t Exist_MainProgram(uint32_t *pulApp)
{
  // See if the first location is 0xfffffffff or something that does not
  // look like a stack pointer, or if the second location is 0xffffffff or
  // something that does not look like a reset vector.
  //
  if((pulApp[0] == 0xffffffff) || ((pulApp[0] & 0xfff00000) != 0x20000000) ||
     (pulApp[1] == 0xffffffff) || ((pulApp[1] & 0xfff00000) != 0x08000000))
  {
    return 0;
  }
  else{
    return 1; 
	}
}
/*****************************************************************************
�������� : RCC_Configuration
�������� : ʱ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void RCC_Configuration(void)
{
  //����DMAʱ�� 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
  
  //Enable AFIO clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //ʹ��JTAG����ΪGPIO  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
/*****************************************************************************
�������� : JumpToApp
�������� : ������ת
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void JumpToApp(uint32_t app_address)            
{
  typedef  void (*pFunction)(void);
  pFunction Jump_To_Application;
  uint32_t JumpAddress;
  
  JumpAddress = *(__IO uint32_t*) (app_address + 4);
  Jump_To_Application = (pFunction) JumpAddress;
  
  __set_MSP(*(__IO uint32_t*) app_address);
  Jump_To_Application();
}

/*****************************************************************************
�������� : update_firmware
�������� : �̼�����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
uint8_t update_firmware(void)
{
  uint32_t src,obj;
  uint8_t t;
  uint32_t offset;
  
  src = FIREWARE_ADDR_H;
  obj = FIREWARE_ADDR_L;
  
  for(t = 0; t < FLASH_USER_SIZE; t ++)
  {
    offset = t * 1024;
    if(Read_Flash(value,src + offset,1024) == ERROR)
		{
      return ERROR;
    }
		
    if(Earse_Flash(obj + offset) == ERROR)
    {
      return ERROR;
    }
    
    if(Write_Flash(obj + offset,value,1024) == ERROR)
    {
      return ERROR;
    }
  }
	return SUCCESS;
}
/*****************************************************************************
�������� : Reset
�������� : ϵͳ��λ
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Reset(void)
{
  NVIC_SystemReset();
}


//��ȡ�豸�İ汾
u16 IAPGetHardwareVersion(void)
{
	u16 ReadDat = 0;
  STMFLASH_Read(FLASH_UPDATE_VERSION2_ADDR,&ReadDat,1);
	return ReadDat == 65535 ? 0 : ReadDat;
}

void IAPSetHardwareVersion(u16 data)
{
	u16 ReadDat = data;
  STMFLASH_Write(FLASH_UPDATE_VERSION2_ADDR,&ReadDat,1);
}


/*****************************************************************************
�������� : main
�������� : ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void main (void)
{
	u16  HardwareVersion = 1;
  /*
  //����(����������RAM״̬��)
  FLASH_Unlock();
  FLASH_ReadOutProtection(DISABLE); 
  */
  uint32_t magic_code;
  uint32_t app_address;
  
  RCC_Configuration();
  
  NVIC_Configuration();
  UartInit(115200);
	
	if(IAPGetHardwareVersion() != HardwareVersion)
	{
		IAPSetHardwareVersion(HardwareVersion);
	}
	
	
  //��ȡ����״̬
  Read_Flash((unsigned char *)&magic_code,FLASH_UPDATE_FLAGE_ADDR,4);
   
	UsartOutStr("update_firmware...\r\n",20);
	//���������³���
  if(magic_code == FIREWARE_UPDATE_FLAG)
  {
		//������������
    if(update_firmware() != SUCCESS)
    {
			UsartOutStr("update_firmware_err\r\n",21);
      Reset();
    }
  }  
  
  //�жϳ����Ƿ����
  app_address = FIREWARE_ADDR_L;
	
	UsartOutStr("check_firmware....\r\n",20);
  if(Exist_MainProgram((uint32_t *)app_address) == 1)
  {
		UsartOutStr("load_user_program\r\n",19);
    JumpToApp(app_address); 
  }

	UsartOutStr("update_firmware_err\r\n",21);
	
  while(1)
  {
		
  }
}

