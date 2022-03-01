#define ADC_C_

#include "adc.h"
#include "delay.h"


/**
* @brief  ����ADC����
* @param  
* @param  None
* @param  None
* @retval None
* @example 
**/
void AdcInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure; 
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//������������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIO�˿�
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
  
	ADC_DeInit(ADC1);  //��λADC1 
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת���������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

//  delay_ms(100); 
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
//	delay_ms(100); 
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
//	delay_ms(100); 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
// delay_ms(100); 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


u16 Get_Adc(u8 ch)
{
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_28Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))//�ȴ�ת������
	{
  }

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
	}
	return temp_val/times;
}












