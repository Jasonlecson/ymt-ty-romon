/**
  ******************************************************************************
  * @file    key.c
  * @author  fengwu yang
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   �������ģ��
  ******************************************************************************
  һ,ʹ��˵��:
		1,�����³������1ms��ʱ���ж���
			KeyLoop();
		
		2,��ѭ������ʹ��
			while(1)
			{
				KeyWhile();
				
				//��1:��⵽��������ִ��ĳ�δ���
				if(KeyValueTemp[3] == 1)
				{
					KeyValueTemp[3] = 0;//�����Ժ�,ֻ�а����ɿ��ٰ��µ�ʱ��Ż����
					//ִ�еĴ���
				}

				//��2:��⵽��������,Ȼ����ݰ���ʱ��ִ��ĳ�δ���
				if(KeyValueTemp[3] == 1)
				{
					if(KeyValueTemp[5]>=3000)//����ʱ�����3S
					{
						KeyValueTemp[3] = 0;//�����Ժ�,ֻ�а����ɿ��ٰ��µ�ʱ��Ż����
						//ִ�еĴ���
					}
				}
				
				//��3:��⵽�����ɿ�ִ��ĳ�δ���
				if(KeyValueTemp[4] == 1)
				{
					KeyValueTemp[4] = 0;//�����Ժ�,ֻ�а�������Ȼ���ɿ���ʱ��Ż����
					//ִ�еĴ���
				}
				
				//��4:��⵽�����ɿ�,Ȼ������ɿ�ʱ��ִ��ĳ�δ���
				if(KeyValueTemp[4] == 1)
				{
					if(KeyValueTemp[6]>=3000)//�ɿ�ʱ�����3S
					{
						KeyValueTemp[4] = 0;//�����Ժ�,ֻ�а����������ɿ���ʱ��Ż����
						//ִ�еĴ���
					}
				}
			}
  ******************************************************************************
  */

#define __KEY_C_
#include "key.h"

int KeyValueTemp[7]={0};//�������� ���±�� �ɿ���� ���±�ǿ���  �ɿ���ǿ��� ����ʱ��(Ms) �ɿ�ʱ��(Ms)
	


/**
* @brief  key init
* @param  
* @param  None
* @param  None
* @retval None
* @example 
**/
void KeyInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);	 //Clock
	
	GPIO_InitStructure.GPIO_Pin = KEY_PIN;//pin		

	#if KeyDown == 0 //0 is key down
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	#else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	#endif
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}


/**
* @brief  ��������
* @param  
* @param  None
* @param  None
* @retval None
* @example 
**/
void KeyWhile(void)
{
  if(KeyPinIn == KeyDown)
	{
		if(KeyValueTemp[1] == 0)
		{
			KeyValueTemp[0]++;
			if(KeyValueTemp[0]>=100)//�����Ժ����
			{
				KeyValueTemp[0] = 0;//Cnt
				KeyValueTemp[2] = 0;//UP
				KeyValueTemp[1] = 1;//Down
				KeyValueTemp[3] = 1;//DownCopy
				KeyValueTemp[4] = 0;//UpCopy
			}
		}
	}
	else
	{
//		if(KeyValueTemp[1]==1)
		{
			KeyValueTemp[0]++;
			if(KeyValueTemp[0]>200)
			{
				KeyValueTemp[0]=0;
				if(KeyValueTemp[1]==1)//���º��ɿ������
				{
					KeyValueTemp[2] = 1;//UP
					KeyValueTemp[1] = 0;//Down
					KeyValueTemp[3] = 0;//DownCopy
					KeyValueTemp[4] = 1;//UpCopy
				}
			}
		}
	}
}



/**
* @brief  place in  1ms timer
* @param  
* @param  None
* @param  None
* @retval None
* @example 
**/
void KeyLoop(void)
{
	if(KeyValueTemp[1])
		KeyValueTemp[5]++;
	else
		KeyValueTemp[5]=0;

	if(KeyValueTemp[2])
		KeyValueTemp[6]++;
	else
		KeyValueTemp[6]=0;
}

