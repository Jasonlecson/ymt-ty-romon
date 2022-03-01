#ifndef __USART_H
#define __USART_H
#include <stm32f10x.h>
#ifndef USART_C_//���û�ж���
#define USART_Ex_ extern
#else
#define USART_Ex_
#endif


#include "LoopList.h"

/****************************����1****************************/
USART_Ex_ rb_t rb_t_usart1_send;//����1���ͻ��滷�ζ���
#define Usart1SendBufflen 2048  //����1�������ݻ����С
USART_Ex_ unsigned char Usart1SendBuff[Usart1SendBufflen];  //����1�������ݻ���


/****************************����2****************************/
#define Usart2ReadBuffLen 1460  //���ڽ������������С
USART_Ex_ unsigned char Usart2ReadBuff[Usart2ReadBuffLen];  //�������ݻ���
USART_Ex_ u32  Usart2ReadCnt;//���ڽ��յ������ݸ���


USART_Ex_ rb_t rb_t_usart2_send;//���ڷ��ͻ��滷�ζ���
#define Usart2SendBufflen 1024  //���ڷ������ݻ����С
USART_Ex_ unsigned char Usart2SendBuff[Usart2SendBufflen];  //���ڷ������ݻ���




void UartInit(u32 bound1,u32 bound2,u32 bound3);//���ڳ�ʼ��
void UsartOutStrIT(USART_TypeDef* USARTx, unsigned char *c,uint32_t cnt);
void UsartOutStr(USART_TypeDef* USARTx, unsigned char *c,uint32_t cnt);//���ڷ����ַ�������
void UsartOutChar(USART_TypeDef* USARTx, unsigned char c);//���ڷ���һ���ֽ�
void Usart2IdleLoop(int IdleCnt);
#endif


