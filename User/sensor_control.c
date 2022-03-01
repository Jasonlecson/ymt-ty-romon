#define SENSOR_CONTROL_C_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "sensor_control.h"
#include "relay.h"
#include "wifi_state_led.h"
#include "adc.h"
#include "timer.h"
#include "DataParse.h"
#include "wifi.h"
#include "ds18B20.h"
#include "iic.h"//����ǿ��
#include "iic2.h"//ˮλ
#include "FlashData.h"
#include "fill_light.h"
#include "buzzer.h"
#include "delay.h"

//�û����°�ť����SmartConfig������
char SmartConfigStartFlag = 0;
//�������ñ�־,������·�����Ժ�����˱�־
//���ָ���������
char SmartConfigFlag=0;

int wifi_state = -1;//ģ��״̬
int wifi_state_copy = -1;//ģ��״̬
int mcu_updata_flag =0 ;//0-��������; 1-������
int mcu_updata_flag_clear_cnt=0;//�������״̬���

struct tm *lcTime;//ʱ���תʱ��
time_t timestamp; //ʱ���
time_t timestamp_copy; //ʱ���(��ת��Ϊ����ʱ��)
char timezone = 0; //ʱ��ƫ��(Ĭ��8,����ʱ��)

extern int fill_light_auto_off_cnt;

sensor_control_struct sensor_control_struct_value;
event_struct event_struct_value;


//�ָ���������
void factory_reset(void){
	//��ֲ��ֲ��
	sensor_control_struct_value.botany_type = -1;
	sensor_control_struct_value.botany_type_copy = -1;
	FlashSetBotanyType(sensor_control_struct_value.botany_type);
	
	//����ֵ
	sensor_control_struct_value.love_value = 0;
	sensor_control_struct_value.love_value_copy =0;
	FlashSetLoveValue(sensor_control_struct_value.love_value);
	
	//��ˮģʽ
	sensor_control_struct_value.mode_value = 0;
	sensor_control_struct_value.mode_value_copy =0;
	FlashSetModeAuto();
	
	//�����ģʽ
	sensor_control_struct_value.fill_light_mode_value = 0;
	sensor_control_struct_value.fill_light_mode_value_copy = 0;
	FlashSetFillLightMode(sensor_control_struct_value.fill_light_mode_value);
	
	
	//��ˮ����
	sensor_control_struct_value.watering_count =0;
	sensor_control_struct_value.watering_count_copy =0;
	FlashSetWaterCount(sensor_control_struct_value.watering_count);
	
	//��ֲʱ��
	sensor_control_struct_value.plant_value=0;
//	sensor_control_struct_value.plant_value_copy=0;
	FlashSetDataPlant(sensor_control_struct_value.plant_value);
	
	//�״���ֲʱ��
	sensor_control_struct_value.plant_value=0;
//	sensor_control_struct_value.plant_value_copy=0;
	FlashSetOneDataPlant(sensor_control_struct_value.plant_value);
	
	//�ۼ��ջ�
	sensor_control_struct_value.cumulative_harvest_value=0;
	sensor_control_struct_value.cumulative_harvest_value_copy=0;
	FlashSetHarvestCount(sensor_control_struct_value.cumulative_harvest_value);
	
	//���Ļ���ʣ����
	sensor_control_struct_value.matrix_value=100;
	sensor_control_struct_value.matrix_value_copy=100;
	FlashSetMatrixData(sensor_control_struct_value.matrix_value);
	
	//��������
	sensor_control_struct_value.nurture_day=0;
	sensor_control_struct_value.nurture_day_copy=0;
	FlashSetNurtureDay(sensor_control_struct_value.nurture_day);

	//ҹ�䲻������ˮ
	sensor_control_struct_value.report_night_value = 1;
	sensor_control_struct_value.report_night_value_copy = 1;
	FlashSetReportEvent(sensor_control_struct_value.report_night_value);
	
	//���ù���ǿ�ȱ���ֵ
	sensor_control_struct_value.light_low_limit_value = 0;
	FlashSetLightAlarm(sensor_control_struct_value.light_low_limit_value);
}


//��ʼ������������ + �¼�����
void sensor_data_value_init(void){
	
	wifi_state = -1;//ģ��״̬
	wifi_state_copy = -1;//ģ��״̬
	
	/*�¼�*/
	/*�¶�*/
	event_struct_value.low_temperature_timer_cnt=0;
	event_struct_value.high_temperature_timer_cnt=0;
	/*ʪ��*/
	event_struct_value.low_humidity_timer_cnt=0;
	event_struct_value.high_humidity_timer_cnt=0;
	/*����*/
	event_struct_value.low_illumination_timer_cnt=0;
	event_struct_value.high_illumination_timer_cnt=0;
	/*����*/
	event_struct_value.low_fertility_timer_cnt=0;
	event_struct_value.high_fertility_timer_cnt=0;
	//ȱˮ
	event_struct_value.hydropenia_timer_cnt = 0;
	//ˮλ��
	event_struct_value.low_water_timer_cnt = 0;
	//ˮλ��
	event_struct_value.high_water_timer_cnt=0;
	
	sensor_control_struct_value.water_full_flag=0;//ˮ����־
	sensor_control_struct_value.water_lack_flag=0;//ȱˮ��־
	
	/*������*/
	//LED��
	sensor_control_struct_value.light_err_flag=0;
	//ˮ��
	sensor_control_struct_value.pump_err_flag=0;
	//ʪ��
	sensor_control_struct_value.humidity_err_count=0;//����������ɼ�����
	sensor_control_struct_value.humidity_err_flag=0;//ʪ�ȴ������쳣
	//�¶�
	sensor_control_struct_value.temperature_err_count=0;//����������ɼ�����
	sensor_control_struct_value.temperature_err_flag=0;//�¶ȴ������쳣
	//����
	sensor_control_struct_value.illumination_err_count=0;
	sensor_control_struct_value.illumination_err_flag=0;
	//ˮλ
	sensor_control_struct_value.water_stage_err_count=0;//����������ɼ�����
	sensor_control_struct_value.water_stage_err_flag=0;//�������쳣
	//����
	sensor_control_struct_value.fertility_err_count=0;//����������ɼ�����
	sensor_control_struct_value.fertility_err_flag=0;//�������쳣
	
	//��ˮģʽ
	sensor_control_struct_value.mode_value = FlashGetModeWatering();
	sensor_control_struct_value.mode_value_copy = 0;
	printf("\r\n��ˮģʽ=%d\r\n",sensor_control_struct_value.mode_value);
	
	//�����ģʽ
	sensor_control_struct_value.fill_light_mode_value = FlashGetFillLightMode();
	sensor_control_struct_value.fill_light_mode_value_copy = 1- sensor_control_struct_value.fill_light_mode_value;
	printf("\r\n�����ģʽ=%d\r\n",sensor_control_struct_value.fill_light_mode_value);
	
	//��ˮ����
	sensor_control_struct_value.watering_count = FlashGetWaterCount();
	sensor_control_struct_value.watering_count_copy = 0;
	printf("\r\n��ˮ����=%d\r\n",sensor_control_struct_value.watering_count);
	
	//ֲ������
	sensor_control_struct_value.botany_type = FlashGetBotanyType();
	sensor_control_struct_value.botany_type_copy = 0;
	printf("\r\nֲ������=%ld\r\n",sensor_control_struct_value.botany_type);
	
	//����ֵ
	sensor_control_struct_value.love_value = FlashGetLoveValue();
	sensor_control_struct_value.love_value_copy = 0;
	printf("\r\n����ֵ=%d\r\n",sensor_control_struct_value.love_value);
	
	//ҹ�䲻������ˮ
	sensor_control_struct_value.report_night_value = FlashGetReportEvent();
	sensor_control_struct_value.report_night_value_copy = 0;
	printf("\r\nҹ�䲻������ˮ=%d\r\n",sensor_control_struct_value.report_night_value);
	
	//��ȫֵ-�¶�
	FlashGetSafe(&sensor_control_struct_value.safe_temperature[0],&sensor_control_struct_value.safe_temperature[1],safe_type_temperature);
	printf("\r\n��ȫֵ-�¶�=%d,%d\r\n",sensor_control_struct_value.safe_temperature[0],sensor_control_struct_value.safe_temperature[1]);
	
	//��ȫֵ-ʪ��
	FlashGetSafe(&sensor_control_struct_value.safe_humidity[0],&sensor_control_struct_value.safe_humidity[1],safe_type_humidity);
	printf("\r\n��ȫֵ-�¶�=%d,%d\r\n",sensor_control_struct_value.safe_humidity[0],sensor_control_struct_value.safe_humidity[1]);
	
	//��ȫֵ-����
	FlashGetSafe(&sensor_control_struct_value.safe_illumination[0],&sensor_control_struct_value.safe_illumination[1],safe_type_illumination);
	printf("\r\n��ȫֵ-����=%d,%d\r\n",sensor_control_struct_value.safe_illumination[0],sensor_control_struct_value.safe_illumination[1]);
	
	//��ȫֵ-����
	FlashGetSafe(&sensor_control_struct_value.safe_fertility[0],&sensor_control_struct_value.safe_fertility[1],safe_type_fertility);
	printf("\r\n��ȫֵ-����=%d,%d\r\n",sensor_control_struct_value.safe_fertility[0],sensor_control_struct_value.safe_fertility[1]);
	
	//��ȫֵ-��������
	sensor_control_struct_value.light_low_limit_value = FlashGetLightAlarm();
	printf("\r\n��ȫֵ-��������=%d\r\n",sensor_control_struct_value.light_low_limit_value);
	//��������ͳ��ʱ��
	sensor_control_struct_value.illumination_statistics_timer_start = ILLUMINATION_STATISTICS_TIMER_START;
	sensor_control_struct_value.illumination_statistics_timer_stop = ILLUMINATION_STATISTICS_TIMER_STOP;
	printf("\r\n��������ͳ��ʱ��=%d,%d\r\n",sensor_control_struct_value.illumination_statistics_timer_start,sensor_control_struct_value.illumination_statistics_timer_stop);
	sensor_control_struct_value.illumination_statistics_timer_delay=0;//�ڶ�ʱ�������ۼ�,����ͳ��ʱ����
	sensor_control_struct_value.illumination_statistics_cnt=0;
	sensor_control_struct_value.illumination_statistics_sum=0;//����ͳ������
	sensor_control_struct_value.illumination_statistics_count=0;//����ͳ�ƴ���
	
	
	sensor_control_struct_value.illumination_statistics_timer_delay1=0;//�ڶ�ʱ�������ۼ�,����ͳ��ʱ����
	sensor_control_struct_value.illumination_statistics_cnt1=0;
	
	
	//����ʣ����
	sensor_control_struct_value.matrix_value = FlashGetMatrixData();
	sensor_control_struct_value.matrix_value_copy = 0;
	printf("\r\n����ʣ����=%d\r\n",sensor_control_struct_value.matrix_value);
	
	//����ֲ����ֲ��ʱ��
	sensor_control_struct_value.plant_value = FlashGetDataPlant();
	printf("\r\n����ֲ����ֲ��ʱ��=%d\r\n",sensor_control_struct_value.plant_value);
	
	//�ۼ��ջ�
	sensor_control_struct_value.cumulative_harvest_value = FlashGetHarvestCount();
	sensor_control_struct_value.cumulative_harvest_value_copy = 0;
	printf("\r\n�ۼ��ջ�=%ld\r\n",sensor_control_struct_value.cumulative_harvest_value);
	
	//�״���ֲֲ��ʱ��(���û��ʱ��,��ֲֵ����ֲʱ��)
	sensor_control_struct_value.begin_date_value = FlashGetOneDataPlant();
	if(sensor_control_struct_value.begin_date_value ==0 && sensor_control_struct_value.plant_value!=0){
		FlashSetOneDataPlant(sensor_control_struct_value.plant_value);
		sensor_control_struct_value.begin_date_value = FlashGetOneDataPlant();
	}
	printf("\r\n�״���ֲֲ��ʱ��=%d\r\n",sensor_control_struct_value.begin_date_value);

	//��ȫֵ-��ˮ����ʱ��
	sensor_control_struct_value.safe_limit_time[0] = SAFE_LIMIT_TIME_START;
	sensor_control_struct_value.safe_limit_time[1] = SAFE_LIMIT_TIME_STOP;
	sensor_control_struct_value.on = 1;
	sensor_control_struct_value.on_copy = 1;
}

/**
* @brief  ��ѵ�ɼ�����������
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/
void sensor_data_collection(void)
{
	float float_value=0;
	float Value;
	unsigned short int usiValue[12];
	
	/*******************************************��ȡʪ��*******************************************/
	usiValue[0] = Get_Adc(1);usiValue[3] = Get_Adc(1);usiValue[6] = Get_Adc(1);usiValue[9] = Get_Adc(1);
	usiValue[1] = Get_Adc(1);usiValue[4] = Get_Adc(1);usiValue[7] = Get_Adc(1);usiValue[10] = Get_Adc(1);
	usiValue[2] = Get_Adc(1);usiValue[5] = Get_Adc(1);usiValue[8] = Get_Adc(1);usiValue[11] = Get_Adc(1);
	
	Value = SortMethod(usiValue,12,2);
	Value = Value/8;
	Value = Value/4096*3.3;//PA1(ʪ��)
	sensor_control_struct_value.humidity_v = Value;
	
	if(Value<1){//������������
		sensor_control_struct_value.humidity_err_count++;//��������ۼ�
		if(sensor_control_struct_value.humidity_err_count > 3){//����3��
			sensor_control_struct_value.humidity_err_count=0;
			sensor_control_struct_value.humidity_err_flag=1;//��λ�����������־
		}
	}
	else{
		sensor_control_struct_value.humidity_err_flag=0;
		sensor_control_struct_value.humidity_err_count=0;
		sensor_control_struct_value.humidity_value = humidity_conversion(Value);
	}
	
	
	/*******************************************��ȡ�¶�*******************************************/
	Value = DS18B20_Get_Temp();
	if(Value==255){//�ɼ��д���
		sensor_control_struct_value.temperature_err_count++;//��������ۼ�
		if(sensor_control_struct_value.temperature_err_count>3){//����3��
			sensor_control_struct_value.temperature_err_count=0;
			sensor_control_struct_value.temperature_err_flag=1;//��λ�����������־
		}
	}
	else{
		sensor_control_struct_value.temperature_err_count=0;
		sensor_control_struct_value.temperature_err_flag=0;
		sensor_control_struct_value.temperature_value = Value;
	}
	
	
	/*******************************************��ȡ����ǿ��*******************************************/
	Value = illumination_get();
	
	if(Value<=0){
		sensor_control_struct_value.illumination_err_count++;//��������ۼ�
		if(sensor_control_struct_value.illumination_err_count>3){//����3��
			sensor_control_struct_value.illumination_err_count=0;
			sensor_control_struct_value.illumination_err_flag=1;//��λ�����������־
		}
	}
	else{
		sensor_control_struct_value.illumination_err_count=0;
		sensor_control_struct_value.illumination_err_flag=0;
		sensor_control_struct_value.illumination_value = Value;
	}
	
	
	/*******************************************ˮλ*******************************************/
	if(timer2_water_stage>100){
		timer2_water_stage=0;
//		__disable_irq();
		Value = iic2_read_byte();
//		__enable_irq();
		if(Value==-1){
			sensor_control_struct_value.water_stage_err_count++;//��������ۼ�
			if(sensor_control_struct_value.water_stage_err_count>3){//����3��
				sensor_control_struct_value.water_stage_err_count=0;
				sensor_control_struct_value.water_stage_err_flag=1;//��λ�����������־
//				printf("ˮλ��������ȡ����!\r\n");
			}
		}
		else{
			sensor_control_struct_value.water_stage_err_count=0;
			sensor_control_struct_value.water_stage_err_flag=0;
			Value = Value/2.5;
			if(Value>100){
				Value = 100;
			}
			if(Value<30){
				Value=0;
			}
			else{
				Value = Value-30;
				Value = Value/70;
				Value = Value*100;
			}
			sensor_control_struct_value.water_stage_value = Value;
		}
	}
	
	
	/*******************************************����*******************************************/
	if(sensor_control_struct_value.humidity_value>95){
		sensor_control_struct_value.fertility_value = 2000;
	}
	else{
		sensor_control_struct_value.fertility_value = \
		(sensor_control_struct_value.humidity_value*sensor_control_struct_value.humidity_value + 29*sensor_control_struct_value.humidity_value)/6 + \
		sensor_control_struct_value.temperature_value/2 ;
		if(sensor_control_struct_value.fertility_value<0){ sensor_control_struct_value.fertility_value=0; }
		if(sensor_control_struct_value.fertility_value>2000){ sensor_control_struct_value.fertility_value=2000; }
	}
	if(sensor_control_struct_value.humidity_value<=0){
		sensor_control_struct_value.fertility_value=0;
	}
	
	

	
	/*******************************************ָʾ��*******************************************/
	sensor_control_struct_value.light_value = wifi_state_led_get();
	
	/*******************************************�����*******************************************/
	sensor_control_struct_value.fill_light_value = fill_light_get();
	
	
	
	if(timer2Value>1500){
		timer2Value=0;
		
		
//		mcu_dp_fault_update(DPID_FAULT,test);//�ϱ��¼�
//		test = test*2;
//		if(test==2048){
//			test=1;
//		}
//			mcu_dp_value_update(DPID_WATER,test1);//�ϱ�
//		
//			
//			
//			if(test1==0){ test1 = 5; }
//			else if(test1==5){ test1 = 10; }
//			else if(test1==10){ test1 = 20; }
//			else if(test1==20){ test1 = 0; }
//			printf("\r\nfloat_value=%d\r\n",sensor_control_struct_value.illumination_value);
		
//		printf("\r\nֲ������=%ld\r\n",sensor_control_struct_value.botany_type);
		
//		mcu_get_system_time();
//		mcu_get_green_time();
		
//		sensor_control_struct_value.watering_count++;
//		mcu_dp_value_update(DPID_WATER_COUNT,1);
		
//		printf("humidity=%f\r\n",float_value);
//		printf("temp=%d\r\n",DS18B20_Get_Temp());
//		printf("illumination_get=%d\r\n",illumination_get());
//		printf("water_stage_value=%d\r\n",sensor_control_struct_value.water_stage_value);
//		printf("fertility_value=%d\r\n",sensor_control_struct_value.fertility_value);
//		
//		printf("pump_value=%d\r\n",sensor_control_struct_value.pump_value);
		
		
	}
}


//�ϱ�����������
void sensor_data_appear(void){
	if(timer2_appear1>100){
		timer2_appear1=0;
		
		//�¶ȴ�����
		if(sensor_control_struct_value.temperature_value - sensor_control_struct_value.temperature_value_copy >=1 ||
			sensor_control_struct_value.temperature_value_copy - sensor_control_struct_value.temperature_value >=1)
		{
			sensor_control_struct_value.temperature_value_copy = sensor_control_struct_value.temperature_value;
			
			mcu_dp_value_update(DPID_TEMP_CURRENT,sensor_control_struct_value.temperature_value);//�ϱ�
		}
	}
	
	if(timer2_appear2>500){
		timer2_appear2=0;
		//ʪ�ȴ�����(�ﵽһ���ı仯)
		if(sensor_control_struct_value.humidity_value - sensor_control_struct_value.humidity_value_copy >2 ||
			sensor_control_struct_value.humidity_value_copy - sensor_control_struct_value.humidity_value >2)
		{
			sensor_control_struct_value.humidity_value_copy = sensor_control_struct_value.humidity_value;
			mcu_dp_value_update(DPID_HUMIDITY_CURRENT,sensor_control_struct_value.humidity_value);//�ϱ�
			
			printf("\r\nʪ��=%d\r\n",sensor_control_struct_value.humidity_value);
		}
		
		//���մ�����(�ﵽһ���ı仯)
		if(sensor_control_struct_value.illumination_value - sensor_control_struct_value.illumination_value_copy >5 ||
			sensor_control_struct_value.illumination_value_copy - sensor_control_struct_value.illumination_value >5)
		{
			sensor_control_struct_value.illumination_value_copy = sensor_control_struct_value.illumination_value;
			mcu_dp_value_update(DPID_ILLUMINATION,sensor_control_struct_value.illumination_value);//�ϱ�
			
			printf("\r\n����=%d\r\n",sensor_control_struct_value.illumination_value);
		}
		
//		mcu_dp_value_update(DPID_WATER,50);//�ϱ�
		
		//ˮλ
		if(sensor_control_struct_value.water_stage_value - sensor_control_struct_value.water_stage_value_copy >2 ||
			sensor_control_struct_value.water_stage_value_copy - sensor_control_struct_value.water_stage_value >2)
		{
			sensor_control_struct_value.water_stage_value_copy = sensor_control_struct_value.water_stage_value;
			mcu_dp_value_update(DPID_WATER,sensor_control_struct_value.water_stage_value);//�ϱ�
			printf("\r\nˮλ=%d\r\n",sensor_control_struct_value.water_stage_value);
		}
	}
	
	
	if(timer2_appear3>3000){
		timer2_appear3=0;
		//����
		if(sensor_control_struct_value.fertility_value - sensor_control_struct_value.fertility_value_copy >20 ||
			sensor_control_struct_value.fertility_value_copy - sensor_control_struct_value.fertility_value >20)
		{
			sensor_control_struct_value.fertility_value_copy = sensor_control_struct_value.fertility_value;
			mcu_dp_value_update(DPID_FERTILITY,sensor_control_struct_value.fertility_value);//�ϱ�
			
			printf("\r\n����=%d\r\n",sensor_control_struct_value.fertility_value);
		}
	}
	
	//ˮ�ÿ���״̬
	if(sensor_control_struct_value.pump_value != sensor_control_struct_value.pump_value_copy){
		sensor_control_struct_value.pump_value_copy = sensor_control_struct_value.pump_value;
		mcu_dp_bool_update(DPID_PUMP,sensor_control_struct_value.pump_value);//�ϱ�
	}
	
	//LED
	if(sensor_control_struct_value.light_value != sensor_control_struct_value.light_value_copy){
		sensor_control_struct_value.light_value_copy = sensor_control_struct_value.light_value;
		mcu_dp_bool_update(DPID_LIGHT,sensor_control_struct_value.light_value);//�ϱ�
	}
	
	
	/*******************************************�����*******************************************/
	if(sensor_control_struct_value.fill_light_value != sensor_control_struct_value.fill_light_value_copy){
		sensor_control_struct_value.fill_light_value_copy = sensor_control_struct_value.fill_light_value;
		mcu_dp_bool_update(DPID_FILL_LIGHT,sensor_control_struct_value.fill_light_value);//�ϱ�
	}

	if(sensor_control_struct_value.on != sensor_control_struct_value.on_copy){
		sensor_control_struct_value.on_copy = sensor_control_struct_value.on;
		mcu_dp_bool_update(DPID_ON,sensor_control_struct_value.on);//�ϱ�
	}
	
	//��ˮģʽ
	if(sensor_control_struct_value.mode_value != sensor_control_struct_value.mode_value_copy){
		sensor_control_struct_value.mode_value_copy = sensor_control_struct_value.mode_value;
		mcu_dp_enum_update(DPID_MODE,sensor_control_struct_value.mode_value); //�ϱ�
		printf("\r\n��ˮģʽ=%d\r\n",sensor_control_struct_value.mode_value);
	}
	
	//�����ģʽ
	if(sensor_control_struct_value.fill_light_mode_value != sensor_control_struct_value.fill_light_mode_value_copy){
		sensor_control_struct_value.fill_light_mode_value_copy = sensor_control_struct_value.fill_light_mode_value;
		mcu_dp_enum_update(DPID_FILL_LIGHT_MODE,sensor_control_struct_value.fill_light_mode_value); //�ϱ�
		printf("\r\n�����ģʽ=%d\r\n",sensor_control_struct_value.fill_light_mode_value);
	}
	
	//��ˮ����
	if(sensor_control_struct_value.watering_count != sensor_control_struct_value.watering_count_copy){
		sensor_control_struct_value.watering_count_copy = sensor_control_struct_value.watering_count;
		
		mcu_dp_value_update(DPID_WATER_COUNT_DEVICE,sensor_control_struct_value.watering_count);
		printf("\r\n��ˮ����=%d\r\n",sensor_control_struct_value.watering_count);
		mcu_dp_value_update(DPID_WATER_COUNT,1);
//		mcu_dp_value_update(DPID_WATER_COUNT,0);
	}
	
	//����ֵ
	if(sensor_control_struct_value.love_value != sensor_control_struct_value.love_value_copy){
		sensor_control_struct_value.love_value_copy = sensor_control_struct_value.love_value;
		mcu_dp_value_update(DPID_LOVE_VALUE,sensor_control_struct_value.love_value);
		printf("\r\n����ֵ=%d\r\n",sensor_control_struct_value.love_value);
	}
	
	//ֲ������
	if(sensor_control_struct_value.botany_type != sensor_control_struct_value.botany_type_copy){
		sensor_control_struct_value.botany_type_copy = sensor_control_struct_value.botany_type;
		mcu_dp_value_update(DPID_TYPE,sensor_control_struct_value.botany_type);
		printf("\r\nֲ������=%ld\r\n",sensor_control_struct_value.botany_type);
	}

	//ҹ�䲻������ˮ
	if(sensor_control_struct_value.report_night_value != sensor_control_struct_value.report_night_value_copy){
		sensor_control_struct_value.report_night_value_copy = sensor_control_struct_value.report_night_value;
		mcu_dp_bool_update(DPID_REPORT_NIGHT,sensor_control_struct_value.report_night_value);
		printf("\r\nҹ�䲻������ˮ=%d\r\n",sensor_control_struct_value.report_night_value);
	}
	
	
	//�ۼ��ջ�
	if(sensor_control_struct_value.cumulative_harvest_value != sensor_control_struct_value.cumulative_harvest_value_copy){
		sensor_control_struct_value.cumulative_harvest_value_copy = sensor_control_struct_value.cumulative_harvest_value;
		mcu_dp_value_update(DPID_CUMULATIVE_HARVEST,sensor_control_struct_value.cumulative_harvest_value);
		printf("\r\n�ۼ��ջ�=%ld\r\n",sensor_control_struct_value.cumulative_harvest_value);
	}
	
	//����ʣ����
	if(sensor_control_struct_value.matrix_value != sensor_control_struct_value.matrix_value_copy){
		sensor_control_struct_value.matrix_value_copy = sensor_control_struct_value.matrix_value;
		mcu_dp_value_update(DPID_MATRIX,sensor_control_struct_value.matrix_value);
		printf("\r\n����ʣ����=%d\r\n",sensor_control_struct_value.matrix_value);
	}
}

/**
* @brief   �¼��ɼ�����(����1ms��ʱ��)
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void event_loop_fun(void){
	//�¶ȵ�
	if(sensor_control_struct_value.temperature_value <= sensor_control_struct_value.safe_temperature[0]){
		event_struct_value.low_temperature_timer_cnt++;
		if(event_struct_value.low_temperature_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.low_temperature_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.low_temperature_timer_cnt = 0;
	}
	//�¶ȸ�
	if(sensor_control_struct_value.temperature_value >= sensor_control_struct_value.safe_temperature[1]){
		event_struct_value.high_temperature_timer_cnt++;
		if(event_struct_value.high_temperature_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.high_temperature_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.high_temperature_timer_cnt = 0;
	}
	
	//ʪ�ȵ�
	if(sensor_control_struct_value.humidity_value <= sensor_control_struct_value.safe_humidity[0]){
		event_struct_value.low_humidity_timer_cnt++;
		if(event_struct_value.low_humidity_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.low_humidity_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.low_humidity_timer_cnt = 0;
	}
	//ʪ�ȸ�
	if(sensor_control_struct_value.humidity_value >= sensor_control_struct_value.safe_humidity[1]){
		event_struct_value.high_humidity_timer_cnt++;
		if(event_struct_value.high_humidity_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.high_humidity_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.high_humidity_timer_cnt = 0;
	}
	
	//������
	if(sensor_control_struct_value.fertility_value <= sensor_control_struct_value.safe_fertility[0]){
		event_struct_value.low_fertility_timer_cnt++;
		if(event_struct_value.low_fertility_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.low_fertility_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.low_fertility_timer_cnt = 0;
	}
	//������
	if(sensor_control_struct_value.fertility_value >= sensor_control_struct_value.safe_fertility[1]){
		event_struct_value.high_fertility_timer_cnt++;
		if(event_struct_value.high_fertility_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.high_fertility_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.high_fertility_timer_cnt = 0;
	}
	
	//���յ�
	if(sensor_control_struct_value.illumination_value <= sensor_control_struct_value.safe_illumination[0]){
		event_struct_value.low_illumination_timer_cnt++;
		if(event_struct_value.low_illumination_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.low_illumination_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.low_illumination_timer_cnt = 0;
	}
	//���ո�
	if(sensor_control_struct_value.illumination_value >= sensor_control_struct_value.safe_illumination[1]){
		event_struct_value.high_illumination_timer_cnt++;
		if(event_struct_value.high_illumination_timer_cnt > TIMER_COMPARE_20MIN){
			event_struct_value.high_illumination_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}else{
		event_struct_value.high_illumination_timer_cnt = 0;
	}
	
	//ȱˮ
	if(sensor_control_struct_value.water_stage_value <= SAFE_HYDROPENIA){
		event_struct_value.hydropenia_timer_cnt++;
		if(event_struct_value.hydropenia_timer_cnt >= TIMER_COMPARE_20MIN){
			event_struct_value.hydropenia_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}
	else{
		event_struct_value.hydropenia_timer_cnt = 0;
	}
	//ˮλ��
	if(sensor_control_struct_value.water_stage_value <= SAFE_WATER_LOW){
		event_struct_value.low_water_timer_cnt++;
		if(event_struct_value.low_water_timer_cnt >= TIMER_COMPARE_20MIN){
			event_struct_value.low_water_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}
	else{
		event_struct_value.low_water_timer_cnt = 0;
	}
	
	//ˮλ��
	if(sensor_control_struct_value.water_stage_value >= SAFE_WATER_HIGH){
		event_struct_value.high_water_timer_cnt++;
		if(event_struct_value.high_water_timer_cnt >= TIMER_COMPARE_20MIN){
			event_struct_value.high_water_timer_cnt = TIMER_COMPARE_20MIN;
		}
	}
	else{
		event_struct_value.high_water_timer_cnt = 0;
	}
}


/**
* @brief   ͳ�ƹ���ǿ��
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void illumination_intensity_statistics(void){
	u32 value=0;
	long long endtime;
	time_t time_t_value;
	//��ģ���ȡ����ʱ���
  if(timestamp !=0){
		//��ֲ��ֲ��
		if(sensor_control_struct_value.botany_type != -1){
			
//			else
			{	//ͳ�ƹ���ǿ�� (����8��)
				if(lcTime->tm_hour >= sensor_control_struct_value.illumination_statistics_timer_start && lcTime->tm_hour < sensor_control_struct_value.illumination_statistics_timer_stop)
				{
					if(sensor_control_struct_value.illumination_statistics_timer_delay>1000*360)//1����
					{
						sensor_control_struct_value.illumination_statistics_timer_delay = 0;
						
						if(sensor_control_struct_value.illumination_statistics_cnt<10)//�ɼ� 10 ������
						{
							sensor_control_struct_value.illumination_statistics_temp[sensor_control_struct_value.illumination_statistics_cnt] = sensor_control_struct_value.illumination_value;
							sensor_control_struct_value.illumination_statistics_cnt++;
						}
						else//�ɼ�������
						{
							sensor_control_struct_value.illumination_statistics_cnt=0;
							
							if(FlashGetIlluminationState() == 1)//����ϱ�
							{
								FlashClearIlluminationState();
							}
							
							SortMethod(sensor_control_struct_value.illumination_statistics_temp,10,0);//����
							
							value = sensor_control_struct_value.illumination_statistics_temp[9] + sensor_control_struct_value.illumination_statistics_temp[8] + sensor_control_struct_value.illumination_statistics_temp[7];
							value = value/3;
							
							/*****����������õĹ�����ֵ,�򿪲����****/
							if(value <= sensor_control_struct_value.safe_illumination[0]){
								if(FlashGetFillLightMode() == 0 && sensor_control_struct_value.on == 1){//�Զ��򿪲����
									sensor_fill_light_set(1);//�򿪲����
									fill_light_auto_off_cnt = 0;
									printf("�Զ��򿪲����\r\n");
								}
								else{
									printf("���Զ��򿪲����\r\n");
								}
							}

							
							//��ȡ�洢�Ĺ���ǿ��, �洢�Ĵ���, ���ͳ������ʱ��ʱ���
							sensor_control_struct_value.illumination_statistics_sum = FlashGetIlluminationStatistics(&sensor_control_struct_value.illumination_statistics_count,&endtime);

							time_t_value = endtime;
							if(localtime(&time_t_value)->tm_hour > localtime(&timestamp_copy)->tm_hour)//ͳ�Ƶ�ʱ�����ڵ�ǰʱ���
							{
								FlashSetIlluminationStatistics(0,0,timestamp_copy);//�������
								sensor_control_struct_value.illumination_statistics_sum=0;
								sensor_control_struct_value.illumination_statistics_count=0;
							}
							else if(localtime(&time_t_value)->tm_hour == localtime(&timestamp_copy)->tm_hour)//Сʱ��ͬ
							{
								if(localtime(&time_t_value)->tm_min >= localtime(&timestamp_copy)->tm_min){//ͳ�Ƶķ��������ڵ�ǰ������
									FlashSetIlluminationStatistics(0,0,timestamp_copy);//�������
									sensor_control_struct_value.illumination_statistics_sum=0;
									sensor_control_struct_value.illumination_statistics_count=0;
								}
							}
							else
							{
								if(timestamp_copy - endtime > 86400)//��ǰʱ���ͳ�Ƶ�ʱ�䳬����24Сʱ
								{
									FlashSetIlluminationStatistics(0,0,timestamp_copy);//�������
									sensor_control_struct_value.illumination_statistics_sum=0;
									sensor_control_struct_value.illumination_statistics_count=0;
								}
							}
							sensor_control_struct_value.illumination_statistics_sum = sensor_control_struct_value.illumination_statistics_sum + value;//�ۼ�����εĹ���ǿ��
							sensor_control_struct_value.illumination_statistics_count = sensor_control_struct_value.illumination_statistics_count + 1;//������һ
							
							FlashSetIlluminationStatistics(sensor_control_struct_value.illumination_statistics_sum,sensor_control_struct_value.illumination_statistics_count,timestamp_copy);//�洢����
						}
					}
							if(FlashGetFillLightMode() == 0 && fill_light_auto_off_cnt >= 1800){//�Զ��򿪲����
								sensor_fill_light_set(0);//�رղ����
							}
				}
				else
				{
					//����ͳ��ʱ��
					if(lcTime->tm_hour == sensor_control_struct_value.illumination_statistics_timer_stop)
					{
						//û���ϱ��¼�
						if(FlashGetIlluminationState() == 0)
						{
							FlashSetIlluminationState();//�����Ѿ��ϱ���־
							
							if(FlashGetFillLightMode() == 0){//�Զ��򿪲����
								sensor_fill_light_set(0);//�رղ����
								printf("�رղ����\r\n");
							}
							
							//��ȡ�洢�Ĺ���ǿ��, �洢�Ĵ���, ���ͳ������ʱ��ʱ���
							sensor_control_struct_value.illumination_statistics_sum = FlashGetIlluminationStatistics(&sensor_control_struct_value.illumination_statistics_count,&endtime);

							FlashSetIlluminationStatistics(0,0,timestamp_copy);//�������
							//ͳ�ƵĹ���ǿ��С�����õĹ���ǿ��
							if(FlashGetLightAlarm()!=65535 && sensor_control_struct_value.illumination_statistics_sum/sensor_control_struct_value.illumination_statistics_count < FlashGetLightAlarm())
							{
								mcu_dp_fault_update(DPID_FAULT,FAULT_TOTAL_LIGHT);//�ϱ��¼�
							}
						}
					}
					
					/*������Ǵ򿪵�,������ǿ��*/
					if(fill_light_get())
					{
						if(sensor_control_struct_value.illumination_statistics_timer_delay1>1000*180)//1����
						{
							sensor_control_struct_value.illumination_statistics_timer_delay1 = 0;
							
							if(sensor_control_struct_value.illumination_statistics_cnt1<10)//�ɼ� 10 ������
							{
								sensor_control_struct_value.illumination_statistics_temp1[sensor_control_struct_value.illumination_statistics_cnt1] = sensor_control_struct_value.illumination_value;
								sensor_control_struct_value.illumination_statistics_cnt1++;
							}
							else//�ɼ�������
							{
								sensor_control_struct_value.illumination_statistics_cnt1=0;
								
								SortMethod(sensor_control_struct_value.illumination_statistics_temp1,10,0);//����
								
								value = sensor_control_struct_value.illumination_statistics_temp1[9] + sensor_control_struct_value.illumination_statistics_temp1[8] + sensor_control_struct_value.illumination_statistics_temp1[7];
								value = value/3;
								
								/*****����������õĹ�����ֵ,�򿪲����****/
								if(value <= sensor_control_struct_value.safe_illumination[0]){
									if(FlashGetFillLightMode() == 0 && sensor_control_struct_value.on == 1){//�Զ��򿪲����
										sensor_fill_light_set(1);//�򿪲����
										fill_light_auto_off_cnt = 0;
										printf("�Զ��򿪲����\r\n");
									}
									else{
										printf("���Զ��򿪲����\r\n");
									}
								}
								else{ 
									if(FlashGetFillLightMode() == 0){//�Զ��򿪲����
								sensor_fill_light_set(0);//�رղ����
								}
								printf("�رղ����\r\n");
							}
							}
						}
					}

				}
			}
		}
	}		
}


/**
* @brief   �¼��ϱ�����
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example
**/
void event_report_fun(void){
	if(timer2_event_appear>TIMER_COMPARE_3HOUR){//3Сʱ
//	if(timer2_event_appear>TIMER_COMPARE_3SEC){//3S
		
		timer2_event_appear=0;
		//��ֲ��ֲ��
		if(sensor_control_struct_value.botany_type != -1){
			//����8�㵽��8��,�����û����ò�����ʱ���
			if((lcTime->tm_hour>=sensor_control_struct_value.safe_limit_time[0] && lcTime->tm_hour<sensor_control_struct_value.safe_limit_time[1]) || FlashGetReportEvent()==0){
				//�¶ȵ�
				if(event_struct_value.low_temperature_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.low_temperature_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_TEMPERATURE_LOW);//�ϱ��¼�
				}
				//�¶ȸ�
				if(event_struct_value.high_temperature_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.high_temperature_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_TEMPERATURE_HIGH);//�ϱ��¼�
				}
				
				//ʪ�ȵ�
				if(event_struct_value.low_humidity_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.low_humidity_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_HUMIDITY_LOW);//�ϱ��¼�
				}
				//ʪ�ȸ�
				if(event_struct_value.high_humidity_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.high_humidity_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_HUMIDITY_HIGH);//�ϱ��¼�
				}
				
				//������
				if(event_struct_value.low_fertility_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.low_fertility_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_FERTILITY_LOW);//�ϱ��¼�
				}
				//������
				if(event_struct_value.high_fertility_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.high_fertility_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_FERTILITY_HIGH);//�ϱ��¼�
				}
				
				//���յ�
				if(event_struct_value.low_illumination_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.low_illumination_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_ILLUMINATION_LOW);//�ϱ��¼�
				}
				//���ո�
				if(event_struct_value.high_illumination_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.high_illumination_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_ILLUMINATION_HIGH);//�ϱ��¼�
				}
				
				//ȱˮ
				if(event_struct_value.hydropenia_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.hydropenia_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_WATER_HYDROPENIA);//�ϱ��¼�
				}
				//ˮλ��
				if(event_struct_value.high_water_timer_cnt >= TIMER_COMPARE_20MIN){
					event_struct_value.high_water_timer_cnt = 0;
					mcu_dp_fault_update(DPID_FAULT,FAULT_WATER_HIGH);//�ϱ��¼�
				}
			}
		}
	}
}

/**
* @brief   ÿ����1��,���ʼ�һ , ����ֵ�ۼ�10
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void NurtureDayFunction(void)
{
	//�Ѿ���ȡ��ʱ���, �п�ʼ��ֲʱ��
	if(timestamp!=0 && sensor_control_struct_value.begin_date_value!=0){
		if(sensor_control_struct_value.botany_type != -1){//������ֲ
			//��ȡ�洢��¼���Ѿ�����������
			sensor_control_struct_value.nurture_day = FlashGetNurtureDay();
			//��ȡʵ������������
			sensor_control_struct_value.nurture_day_copy = (timestamp - sensor_control_struct_value.begin_date_value)/(3600*24);
			//ʱ�䲻һ��
			if(sensor_control_struct_value.nurture_day != sensor_control_struct_value.nurture_day_copy){
				sensor_control_struct_value.nurture_day = sensor_control_struct_value.nurture_day_copy;
				
				FlashSetNurtureDay(sensor_control_struct_value.nurture_day);
				sensor_control_struct_value.love_value=sensor_control_struct_value.love_value+10;//����ֵ�ۼ�
				FlashSetLoveValue(sensor_control_struct_value.love_value);//�洢����ֵ
				/*����ÿ��1���һ*/
				sensor_control_struct_value.matrix_value = FlashGetMatrixData();
				if(sensor_control_struct_value.matrix_value>0){
					sensor_control_struct_value.matrix_value = sensor_control_struct_value.matrix_value -1;
					FlashSetMatrixData(sensor_control_struct_value.matrix_value);
				}
			}
		}
	}
}

/**
* @brief   ѭ����ˮ
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void WateringCirculationFunction(void)
{
	if(timer2_auto_watering>TIMER_COMPARE_1HOUR){//1Сʱ
		timer2_auto_watering=0;		
		//��ֲ��ֲ��
		if(sensor_control_struct_value.botany_type != -1){
			//����8�㵽��8��,�����û����ò�����ʱ���
			if((lcTime->tm_hour>=sensor_control_struct_value.safe_limit_time[0] && lcTime->tm_hour<sensor_control_struct_value.safe_limit_time[1]) || FlashGetReportEvent()==0){
				//ʪ�ȵ�
				if(sensor_control_struct_value.humidity_value<sensor_control_struct_value.safe_humidity[0] && 
					//�Զ�ģʽ && ˮ����ˮ
					sensor_control_struct_value.mode_value == 0 && sensor_control_struct_value.on == 1/*&& sensor_control_struct_value.water_stage_value>SAFE_HYDROPENIA*/){
					sensor_pump_set(1);
				}
			}
		}
	}
}



//��ѹ��ʪ�ȵĻ���
int humidity_conversion(float Value){
	static int humidity_copy=0;
	int humidity=0;
	
	if(Value>=2.7){
		humidity = 0;
		humidity_copy = humidity;
	}else if(Value>=1.8){
		humidity = -22*Value+60;
		if(humidity>100)humidity=100;
		if(humidity<0)humidity=0;
		humidity_copy = humidity;
	}else if(Value>=1.6){
		humidity = -100*Value+200;
		if(humidity>100)humidity=100;
		if(humidity<0)humidity=0;
		humidity_copy = humidity;
	}else if(Value>=1.5){
		humidity = -200*Value+360;
		if(humidity>100)humidity=100;
		if(humidity<0)humidity=0;
		humidity_copy = humidity;
	}else if(Value>=1.44){
		humidity = -333*Value+560;
		if(humidity>100)humidity=100;
		if(humidity<0)humidity=0;
		humidity_copy = humidity;
	}else if(Value>=1.2){
		humidity = -500*Value+800;
		if(humidity>100)humidity=100;
		if(humidity<0)humidity=0;
		humidity_copy = humidity;
	}else{
		
	}
	
	return humidity_copy;
}

//���ƽ�ˮһ��ʱ���Ժ�ֹͣ��ˮ
//��ˮ�����ۼ�,��¼��ǰ��ˮ����
void sensor_watering(void){
	//�ڽ�ˮ״̬
	if(sensor_control_struct_value.pump_value){
		if(timer2_watering>5000){
			timer2_watering=0;
			sensor_pump_set(0);
			
			sensor_control_struct_value.watering_count = sensor_control_struct_value.watering_count+1;
			FlashSetWaterCount(sensor_control_struct_value.watering_count);
			printf("\r\n��ˮ����=%d\r\n",sensor_control_struct_value.watering_count);
			
			sensor_control_struct_value.love_value = sensor_control_struct_value.love_value + 1;
			FlashSetLoveValue(sensor_control_struct_value.love_value);
		}
	}
	else{
		timer2_watering=0;
	}
}


//����ˮ��
void sensor_pump_set(char value){
	RelaySet(value);
}
//����ָʾ��
void sensor_light_set(char value){
	wifi_state_led_set(value);
}
//���Ʋ����
void sensor_fill_light_set(char value){
	fill_light_set(value);
}
//���Ʒ�����
void sensor_buzzer_set(char value){
	BuzzerSet(value);
}
