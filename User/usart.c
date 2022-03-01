/**
  ******************************************************************************
  * @file    usart.c
  * @author  fengwu yang
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   �������ú���
				
  ******************************************************************************
  */
#define USART_C_
#include <stdio.h>
#include "usart.h"
#include "BufferManage.h"
#include "main.h"
#include "cString.h"

#include "wifi.h"
/****************************����1****************************/
unsigned char Usart1SendBuff[Usart1SendBufflen];  //���ڷ������ݻ���
u8 Usart1SendBits=0;//������ȡ���ζ���1���ֽ�

unsigned char *Usart1SendData;//�����жϷ������ݵĵ�ַ
u32   Usart1SendDataCnt=0;//�����жϷ������ݵĸ���


/****************************����2****************************/
unsigned char Usart2ReadBuff[Usart2ReadBuffLen]={0};  //�������ݻ���
u32  Usart2ReadCnt = 0;//���ڽ��յ������ݸ���


unsigned char Usart2SendBuff[Usart2SendBufflen];  //���ڷ������ݻ���
u8 Usart2SendBits=0;//������ȡ���ζ���1���ֽ�

unsigned char *Usart2SendData;//�����жϷ������ݵĵ�ַ
u32   Usart2SendDataCnt=0;//�����жϷ������ݵĸ���


/*���ڿ���ʱ����*/
u32 Usart2ReadCntCopy=0;//���ڽ��յ������ݸ���
u8  Usart2IdleCnt=0;//���м���



/**
* @brief   ���ڳ�ʼ��
* @param   bound  ������
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void UartInit(u32 bound1,u32 bound2,u32 bound3){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	rbCreate(&rb_t_usart1_send,Usart1SendBuff,Usart1SendBufflen);//�������ζ���(���ڷ�������)
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
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
	
	//USART1_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART1_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
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
	
	USART_InitStructure.USART_BaudRate = bound2;//���ڲ�����
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	
	/*����--1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
  /*����--2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
}


/**
* @brief  �����жϷ�������
* @param  c:���ݵ��׵�ַ  cnt:���͵����ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStrIT(USART_TypeDef* USARTx, unsigned char *c, uint32_t cnt)
{
	if(USARTx == USART1){
		PutData(&rb_t_usart1_send,c,cnt);
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
	else if(USARTx == USART2){
		Usart2SendData=c;
		Usart2SendDataCnt=cnt;
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
}


/**
* @brief  ���ڷ����ַ�������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStr(USART_TypeDef* USARTx, unsigned char *c,uint32_t cnt)
{
	while(cnt--)
	{
		USART_SendData(USARTx, *c++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}


/**
* @brief  ���ڷ���һ���ֽ�
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutChar(USART_TypeDef* USARTx, unsigned char c)
{
	USART_SendData(USARTx,c);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}


/**
* @brief  ����2�Զ�������жϼ��(�ŵ�1ms��ʱ��)
* @param  IdleCnt: ����ʱ��
* @param  None
* @param  None
* @retval None
* @example 
**/
void Usart2IdleLoop(int IdleCnt)
{
	if(Usart2ReadCnt!=0)//���ڽ��յ�����
	{
		if(Usart2ReadCntCopy != Usart2ReadCnt)
		{
			Usart2ReadCntCopy = Usart2ReadCnt;
			Usart2IdleCnt = 0;
		}
		else
		{
			Usart2IdleCnt++;
			if(Usart2IdleCnt > IdleCnt)//�������ʱ��
			{
				Usart2IdleCnt = 0;
				Usart2ReadCnt=0;
				
//				PutData(&rb_t_usart1_send,"\r\n\r\nUsart2Read:\r\n",17);
//				HexToStr((char*)Usart2ReadBuff,(char*)MainBuffer,Usart2ReadCntCopy);
//				PutData(&rb_t_usart1_send,MainBuffer,Usart2ReadCntCopy*2);
//				USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
				
				uart_receive_buff_input(Usart2ReadBuff,Usart2ReadCntCopy);
			}
		}
	}
}


void USART1_IRQHandler(void)//�����жϷ������
{
	u8 Res;
	int value;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
	} 
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    if(rbCanRead(&rb_t_usart1_send)>0)//�����������ݸ�������0
    {
			rbRead(&rb_t_usart1_send,&Usart1SendBits,1);
			USART_SendData(USART1, Usart1SendBits);
    }
    else
    {
      //�����ֽڽ���			
      USART_ClearITPendingBit(USART1,USART_IT_TXE);
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    }
  }
	//�������
  if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_TC);
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  }
} 


void USART2_IRQHandler(void)//�����жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		
//		uart_receive_input(Res);
		
		Usart2ReadBuff[Usart2ReadCnt] = Res;	//���յ����ݴ�������
		Usart2ReadCnt++;
		if(Usart2ReadCnt > Usart2ReadBuffLen -10)//��ֹ�������
		{
			Usart2ReadCnt = 0;
		}
	}
}


/**
* @brief  ʹ��microLib�ķ���,ʹ��printf
* @warningg ��ѡ microLib
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/

int fputc(int ch, FILE *f)
{
	static char flage=0;
	uint8_t data = ch;
	if(!flage)
	{
	  flage=1;
//		USART_ClearITPendingBit(USART1,USART_IT_TC);//���printf��ʧ��һ���ֽڵ�����
	}
//	PutData(&rb_t_usart1_send,&data,1);
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  return ch;
}

