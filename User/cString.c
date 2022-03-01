/**
  ******************************************************************************
  * @file    key.c
  * @author  yang feng wu 
  * @version V1.0.0
  * @date    2019/10/13
  * @brief   �ַ���������
  ******************************************************************************

  ******************************************************************************
  */
#define CSTRING_C_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cString.h"


char *StringStr = NULL;
void cStringFree(void){
	free(StringStr);
}

/**
* @brief  ��ȡ�����ַ���֮����ַ���
* @param  Str  Դ�ַ���
* @param  StrBegin  ��ʼ���ַ���
* @param  StrEnd    �������ַ���
* @retval �ַ����׵�ַ
* @example  printf("%s",StrBetwString("wqe5w4ew46e5w","5w","6e"));cStringFree(Str);  ���:4ew4   
**/
char *StrBetwStringMalloc(char *Str,char *StrBegin,char *StrEnd)
{
	char *StrStart=0,*StrStop=0,len=0;
	len = strlen(StrBegin);//�ַ�������
	
  StrStart=strstr(Str, StrBegin);//��һ���ַ�����ʼ�ĵ�ַ
	if(StrStart)
	{
	  StrStop = strstr(StrStart+len+1, StrEnd);//�ڶ����ַ�����ʼ�ĵ�ַ
		if(StrStop)
		{
			StringStr = (char *)malloc(((StrStop - (StrStart+len))+1) *sizeof(char));//�����һ���ռ�,��ֹ�������ݸ���
			memset(StringStr,NULL,(StrStop - (StrStart+len))+1);
			memcpy(StringStr, StrStart+len, (StrStop - (StrStart+len)));
			return StringStr;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
	  return NULL;
	}
}

char *StrStop = NULL;
char StringValue;
void cStringRestore(void){
	if(StrStop!=NULL){
		*StrStop = StringValue;//���ϵ�ʱ�ضϵ��ַ���
	}
}

/**
* @brief  ��ȡ�����ַ���֮����ַ���
* @param  Str  Դ�ַ���
* @param  StrBegin  ��ʼ���ַ���
* @param  StrEnd    �������ַ���
* @retval �ַ����׵�ַ
* @example  printf("%s",StrBetwString("wqe5w4ew46e5w","5w","6e"));cStringRestore();  ���:4ew4   
**/
char *StrBetwString(char *Str,char *StrBegin,char *StrEnd)
{
	char *StrStart=0,len=0;
	len = strlen(StrBegin);//�ַ�������
  StrStart=strstr(Str, StrBegin);//��һ���ַ�����ʼ�ĵ�ַ
	if(StrStart)
	{
	  StrStop = strstr(StrStart+len+1, StrEnd);//�ڶ����ַ�����ʼ�ĵ�ַ
		if(StrStop)
		{
      StringValue = *StrStop;
			*StrStop = 0;//�ض�
			return StrStart+len;
		}
		else return NULL;
	}
	else return NULL;
}


/**����
* @brief  �ָ��ַ���
* @param  src        Դ�ַ���
* @param  separator  �ָ�
* @param  dest       �����Ӵ�������
* @param  num        ���ַ����ĸ���
* @retval None
* @example split("42,uioj,dk4,56",",",temp,&cnt);  temp[0]=42,...temp[3]=56  cnt=4
**/
/*
void split(char *src,const char *separator,char **dest,int *num) 
{
	char *pNext;
	int count = 0;
	if (src == NULL || strlen(src) == 0)
		return;
	if (separator == NULL || strlen(separator) == 0)
		return;
	pNext = (char *)strtok(src,separator);
	while(pNext != NULL) 
	{
		if(dest != NULL)
		*dest++ = pNext;
		++count;
		pNext = (char *)strtok(NULL,separator);
	}  
	*num = count;
}
*/


char *split_buff[MAXList] = {0};
/**
* @brief  �ָ��ַ���
* @param  src        Դ�ַ���
* @param  separator  �ָ�
* @param  dest       �����Ӵ�������
* @param  DestLen    Ԥ�����ַ�����������
* @retval ʵ�����ַ����ĸ���
* @example split("42,uioj,dk4,56",",",temp,4);  temp[0]=42,...temp[3]=56
**/
int split(char *src,const char *separator,char **dest,int DestLen)
{
	char *pNext;
	int count = 0;
	if (src == NULL || strlen(src) == 0)
		return 0;
	if (separator == NULL || strlen(separator) == 0)
		return 0;
	pNext = (char *)strtok(src,separator);
	while(pNext != NULL) 
	{
		if(dest != NULL)
		*dest++ = pNext;
		++count;
		pNext = (char *)strtok(NULL,separator);
		if(count>=DestLen){
		  break;
		}
	}  
//	*num = count;
	return count;
}


/**
* @brief  �ַ���ת16���Ʊ�ʾ���ַ���
* @param  pbSrc   Դ��ַ
* @param  pbDest  Ŀ�ĵ�ַ
* @param  nlen    Դ�ַ�������
* @param  None    
* @retval None
* @example 
		temp[0] = 'AA';
		temp[1] = '8';
		temp[2] = 8;
		temp[3] = 0x0F;
		HexToStr(temp,temp1,4)  
		temp1= "414138080F";
**/
void HexToStr(char *pbSrc,char *pbDest,int nlen)
{
	char ddl,ddh;
	int i;
	for(i=0;i<nlen;i++)
	{
		ddh = 48 +pbSrc[i] / 16;
		ddl = 48 +pbSrc[i] % 16;
		if(ddh>57) ddh = ddh+7;
		if(ddl>57) ddl = ddl+7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] =ddl;
	}
	pbDest[nlen*2] = '\0';
}


/**
* @brief  16���Ʊ�ʾ���ַ���ת�ַ���(�����ϵ��෴)
* @param  pbSrc   Դ��ַ
* @param  pbDest  Ŀ�ĵ�ַ
* @param  nlen    Դ�ַ�������
* @param  None    
* @retval None
* @example 
**/
void StrToHex(char *pbSrc, char *pbDest, int nLen)
{
	char h1,h2;
	char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
		s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
		s2 -= 7;

		pbDest[i] = s1*16 + s2;
	}
}