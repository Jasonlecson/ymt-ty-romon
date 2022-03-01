/**
  ******************************************************************************
  * @author  yang feng wu 
  * @version V1.0.0
  * @date    2020/10/10
  * @brief   main
  ******************************************************************************
  ******************************************************************************
*/

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cString.h"
#include "System.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "relay.h"
#include "key.h"
#include "buzzer.h"
#include "ConfigModuleNoBlock.h"
#include "PVD.h"
#include "flash_helper.h"
#include "iwdg.h"
#include "wifi.h"
#include "wifi_state_led.h"
#include "ds18b20.h"
#include "TM601.h"
#include "iic.h"
#include "iic2.h"
#include "sensor_control.h"
#include "fill_light.h"
#include "FlashData.h"


/*0:����汾*/
/*1:���Ĺ��մ�����ΪBH1750; ����PID*/
u16 HardwareVersion = 0;

//��������ʹ��
unsigned char MainBuffer[MainBufferLen];//��������,ȫ��ͨ��
u32  MainLen=0;      //ȫ��ͨ�ñ���
unsigned char *MainString;    //ȫ��ͨ�ñ���
u16 ReadDat[2];

char RelayState=0;

//
char led_state_change_flag=0;

void wifi_work_state_led(void);
void KeyFun(void);//������
void TimerData(void);//��Ҫ��ʱ���еĳ���

int main(void)
{
	SCB->VTOR = FLASH_BASE | 0x5000;
  NVIC_Configuration();
	/*��ȡӲ���汾*/
	HardwareVersion = IAPGetHardwareVersion();
	if(HardwareVersion == 1)
	{
		UartInit(115200,9600,115200);	 //���ڳ�ʼ��Ϊ9600
	}
	else
	{
		UartInit(115200,115200,115200);	 //���ڳ�ʼ��Ϊ115200
	}
	printf("\r\nHardwareVersion:%d\r\n",HardwareVersion);
	Timer2Init();
	DelayInit();
	/*��ʼ���洢*/
	FlashHelperInit(FLASH_DATA_ADDR_1,FLASH_DATA_ADDR_2,FLASH_DATA_ADDR_3,FLASH_DATA_ADDR_4);
	
	
	RelayInit();
	AdcInit();
	KeyInit();
	BuzzerInit();
	DS18B20Init();//�¶�
	IIC_Init();//����
	IIC2_Init();//ˮλ
	fill_light_init();//�����
	wifi_state_led_init();
	PVDInit(PWR_PVDLevel_2V9);
	
	STMFLASH_Read(FLASH_UPDATE_FLAGE_ADDR,ReadDat,2);
	if(ReadDat[0] != 0xFFFF || ReadDat[1] != 0xFFFF){
		ReadDat[0] = 0xFFFF;
		ReadDat[1] = 0xFFFF;
		STMFLASH_Write(FLASH_UPDATE_FLAGE_ADDR,ReadDat,2);
	}
	
	wifi_protocol_init();
	sensor_data_value_init();//��ʼ������������

	printf("***********start***********\r\n");
	IWDG_Init(IWDG_Prescaler_256,156*5);
	
	wifi_state_led_set(1);
	
	while(1)
	{
		IWDG_Feed();//ι��
		
		wifi_work_state_led();
		
		if(wifi_state == WIFI_CONN_CLOUD && mcu_updata_flag ==0){
			//�ɼ�����������
			sensor_data_collection();
			//�ϱ�����������
			sensor_data_appear();

			//ÿ����1��,���ʼ�һ , ����ֵ�ۼ�10
			NurtureDayFunction();
			//ѭ����ˮ
			WateringCirculationFunction();
			//�¼��ϱ�����
			event_report_fun();
			//ͳ�ƹ���ǿ��
			illumination_intensity_statistics();
		}
			/*******************************************ˮ��*******************************************/
			sensor_control_struct_value.pump_value = RelayGet();
			//���ƽ�ˮһ��ʱ���Ժ�ֹͣ��ˮ
			sensor_watering();
		wifi_uart_service();
		
		if(mcu_updata_flag==0){
			//��Ҫ��ʱ���еĳ���
			TimerData();
			KeyFun();//������
		}
	}
}

/*****************************************************************************
�������� : wifi_work_state_led
�������� : wifi״̬led����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void wifi_work_state_led(void)
{
	wifi_state = mcu_get_wifi_work_state();
	if(wifi_state != wifi_state_copy){
		wifi_state_copy = wifi_state;
		printf("*****************wifi_state*****************: %d \r\n",wifi_state_copy);
	}
	
	if(SmartConfigStartFlag){wifi_state=0;}
	switch(wifi_state)
	{
		case SMART_CONFIG_STATE://����ģʽ,����
			if(timer2Value1>250){
				timer2Value1=0;
				wifi_state_led_set(-1);
			}
			led_state_change_flag=1;
		break;
			
		case AP_STATE://APģʽ,����
			if(timer2Value1>1500){
				timer2Value1=0;
				wifi_state_led_set(-1);
			}
			led_state_change_flag=1;
		break;

		case WIFI_NOT_CONNECTED://û������·����
			if(timer2Value1>1500){
				timer2Value1=0;
				wifi_state_led_set(-1);
			}
			led_state_change_flag=1;
		break;

		case WIFI_CONNECTED://����·����
//			if(SmartConfigFlag==1){
//				SmartConfigFlag=0;
//				factory_reset();//�ָ���������
//				wifi_state_led_set(1);
//				FlashSetLedState(1);
//			}
//			led_state_change_flag=1;
		break;

		case WIFI_CONN_CLOUD://�������ƶ�
			if(SmartConfigFlag==1){
				SmartConfigFlag=0;
				factory_reset();//�ָ���������
				wifi_state_led_set(1);
				FlashSetLedState(1);
				all_data_update();
			}
			if(led_state_change_flag==1){
				led_state_change_flag=0;
				sensor_light_set(FlashGetLedState());
			}
		break;
		
		default:
			if(timer2Value1>1500){
				timer2Value1=0;
				wifi_state_led_set(-1);
			}
			led_state_change_flag=1;
		break;
	}
}



/**
* @brief   ������
* @param   None
* @retval  None
* @warning None
* @example
**/
void KeyFun(void)
{
	static int Key_down_value=0;
	if(KeyValueTemp[3] == 1)
	{
		Key_down_value = KeyValueTemp[5];
		if(KeyValueTemp[5]>=2500)//����ʱ�����3S
		{
			printf("\r\nKeyFun\r\n");
			SmartConfigStartFlag = 1;
			SmartConfigFlag = 1;
			KeyValueTemp[3] = 0;//�����Ժ�,ֻ�а����ɿ��ٰ��µ�ʱ��Ż����
			mcu_set_wifi_mode(SMART_CONFIG);
		}
	}
	
	//�ɿ�
	if(KeyValueTemp[4] == 1)
	{
		KeyValueTemp[4] = 0;//�����Ժ�,ֻ�а�������Ȼ���ɿ���ʱ��Ż����
		//ִ�еĴ���
		printf("\r\nKeyUP\r\n");
		if(Key_down_value>100 && Key_down_value<2000)
			{
				if(wifi_state == WIFI_CONN_CLOUD)
					{
					printf("\r\nKeyUP _ ��ˮ\r\n");
					sensor_pump_set(1);
					}
			}
	}
}


/**
* @brief   ��ʱѭ������(��1ms��ʱ������ִ��)
* @param   value
* @retval  None
* @warning None
* @example
**/
void TimerLoopFun(int value)
{
	
}



/**
* @brief   ��Ҫ��ʱ���еĳ���
* @param   None
* @retval  None
* @warning None
* @example
**/
void TimerData(void)
{
	if(wifi_state == WIFI_CONN_CLOUD){
		//��ʱ��ȡһ��ʱ���
		if(timestamp == 0){//û�л�ȡ��ʱ���
			if(TimerDataValue1 > TIMER_COMPARE_3SEC){
				TimerDataValue1 = 0;
				mcu_get_system_time();// mcu_get_greentime
			}
		}else{
			if(TimerDataValue1>=TIMER_COMPARE_HALF_HOUR){
//			if(TimerDataValue1>=TIMER_COMPARE_3SEC){
				TimerDataValue1 = 0;
				mcu_get_system_time();// mcu_get_greentime
			}
		}
	}

//	if(timer2Value3>TIMER_COMPARE_SEC){
//		timer2Value3=0;
//		mcu_get_wifi_connect_status();
//	}
	
	
	//ÿ��1S��ʱ���ת��Ϊʱ��
	if(timestamp!=0)//�Ѿ���ȡ��ʱ���
	{
		if(timer2Value2 > TIMER_COMPARE_SEC){
			timer2Value2 = 0;
			
			//����ʱ��
			timestamp_copy = timestamp;
			//ʱ���任
			timestamp_copy = timestamp_copy + timezone*3600;
			//ʱ���תʱ��
			lcTime = localtime(&timestamp_copy);
			lcTime->tm_year = lcTime->tm_year + 1900;
			lcTime->tm_mon = lcTime->tm_mon + 1;
//			printf("\r\nʱ��=%d,%d,%d,%d,%d,%d\r\n",lcTime->tm_year,lcTime->tm_mon,lcTime->tm_mday,\
//			lcTime->tm_hour,lcTime->tm_min,lcTime->tm_sec);
		}
	}
}
