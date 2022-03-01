#ifndef SENSOR_CONTROL_Cc_
#define SENSOR_CONTROL_Cc_

#ifndef SENSOR_CONTROL_Cx_//���û�ж���
#define SENSOR_CONTROL_Cx_ extern
#else
#define SENSOR_CONTROL_Cx_
#endif

#include "stm32f10x.h"
#include <time.h>

//���ϵ��ϱ�
#define fault_none 0      //�޹���
#define fault_hardware 1  //Ӳ������
#define FAULT_FERTILITY_HIGH 2 //������
#define FAULT_FERTILITY_LOW 4 //������
#define FAULT_HUMIDITY_HIGH 8  //ʪ�ȸ�
#define FAULT_HUMIDITY_LOW 16  //ʪ�ȵ�
#define FAULT_TEMPERATURE_HIGH 32  //�¶ȸ�
#define FAULT_TEMPERATURE_LOW 64  //�¶ȵ�
#define FAULT_ILLUMINATION_HIGH 128  //���ո�
#define FAULT_ILLUMINATION_LOW 256  //���յ�
#define FAULT_WATER_HIGH 512  //ˮλ��
#define FAULT_WATER_HYDROPENIA 2048  //ȱˮ
#define FAULT_TOTAL_LIGHT 1024 //������������


//��ˮ/����ʱ��(Ĭ��)
#define  SAFE_LIMIT_TIME_START 8
#define  SAFE_LIMIT_TIME_STOP  20
//��������ͳ��ʱ���(Ĭ��)
#define ILLUMINATION_STATISTICS_TIMER_START  8 //��ʼʱ��
#define ILLUMINATION_STATISTICS_TIMER_STOP 20 //����ʱ��

//��ȫֵ-ˮλ
#define SAFE_WATER_HIGH 100 //ˮ��
#define SAFE_WATER_LOW 10 //ˮλ��
#define SAFE_HYDROPENIA 0 //ȱˮ


SENSOR_CONTROL_Cx_ int wifi_state;//ģ��״̬
SENSOR_CONTROL_Cx_ int wifi_state_copy;//ģ��״̬
SENSOR_CONTROL_Cx_ int mcu_updata_flag;//0-��������; 1-������
SENSOR_CONTROL_Cx_ int mcu_updata_flag_clear_cnt;//�������״̬���

SENSOR_CONTROL_Cx_ struct tm *lcTime;//ʱ���תʱ��
SENSOR_CONTROL_Cx_ time_t timestamp; //ʱ���
SENSOR_CONTROL_Cx_ time_t timestamp_copy; //ʱ���
SENSOR_CONTROL_Cx_ char timezone; //ʱ��ƫ��(Ĭ��8,����ʱ��)
//�û����°�ť����SmartConfig������
SENSOR_CONTROL_Cx_ char SmartConfigStartFlag;

//�������ñ�־,������·�����Ժ�����˱�־
//���ָ���������
SENSOR_CONTROL_Cx_ char SmartConfigFlag;

//�¼�
typedef struct event_struct
{
	/*�¶�*/
	u32 low_temperature_timer_cnt;
	u32 high_temperature_timer_cnt;
	/*ʪ��*/
	u32 low_humidity_timer_cnt;
	u32 high_humidity_timer_cnt;
	/*����*/
	u32 low_illumination_timer_cnt;
	u32 high_illumination_timer_cnt;
	/*����*/
	u32 low_fertility_timer_cnt;
	u32 high_fertility_timer_cnt;
	/*ȱˮ*/
	u32 hydropenia_timer_cnt;
	/*ˮλ��*/
	u32 low_water_timer_cnt;
	/*ˮλ��*/
	u32 high_water_timer_cnt;
}event_struct;

SENSOR_CONTROL_Cx_ event_struct event_struct_value;

#define public_temp_char_len 100//ͨ�ñ�������
//����������
typedef struct sensor_control_struct
{
	char on;
	char on_copy;
	int  public_int_value;//ͨ�ñ���
	char public_temp_char[public_temp_char_len];//ͨ�ñ���
	char pump_value;//ˮ��
	char pump_value_copy;//ˮ��
	char pump_err_flag;//ˮ�ô������쳣
	
	char light_value;//LED
	char light_value_copy;//LED
	char light_err_flag;//LED�쳣
	//�����
	char fill_light_value;//LED
	char fill_light_value_copy;//LED
	char fill_light_err_flag;//LED�쳣
		//�����ģʽ
	char fill_light_mode_value;
	char fill_light_mode_value_copy;
	char fill_light_on; //�ƿ���
	char fill_light_on_copy;
	//ʪ��
	float humidity_v;//ʪ�ȴ�������ѹ
	int humidity_value;//ʪ�ȴ�����ֵ
	int humidity_value_copy;//ʪ�ȴ�����ֵ
	char humidity_err_count;//����������ɼ�����
	char humidity_err_flag;//ʪ�ȴ������쳣
	//�¶�
	int temperature_value;//�¶ȴ�����ֵ
	int temperature_value_copy;//�¶ȴ�����ֵ
	char temperature_err_count;//����������ɼ�����
	char temperature_err_flag;//�¶ȴ������쳣
	//����
	int illumination_value;//���մ�����ֵ
	int illumination_value_copy;//���մ�����ֵ
	char illumination_err_count;//����������ɼ�����
	char illumination_err_flag;//�¶ȴ������쳣
	//ˮλ
	int water_stage_value;//������ֵ
	int water_stage_value_copy;//������ֵ
	char water_stage_err_count;//����������ɼ�����
	char water_stage_err_flag;//�������쳣
	//����
	int fertility_value;//������ֵ
	int fertility_value_copy;//������ֵ
	char fertility_err_count;//����������ɼ�����
	char fertility_err_flag;//�������쳣
	
	//ģʽ
	int mode_value;//��ˮģʽ
	int mode_value_copy;//��ˮģʽ
	
	//��ˮ����
	int watering_count;
	int watering_count_copy;
	
	//ֲ������
	long botany_type;
	long botany_type_copy;
	
	//����ֵ
	int love_value;
	int love_value_copy;
	
	//ҹ�䲻������ˮ
	int report_night_value;
	int report_night_value_copy;
	
	//��ȫֵ-�¶�
	unsigned short int safe_temperature[2];
	
	//��ȫֵ-ʪ��
	unsigned short int safe_humidity[2];
	
	//��ȫֵ-����
	unsigned short int safe_illumination[2];
	
	//��ȫֵ-����
	unsigned short int safe_fertility[2];
	
	//��ȫֵ-��ˮ����ʱ��
	unsigned short int safe_limit_time[2];
	
	//������������Ƚ�ֵ
	int light_low_limit_value;
	//��������ͳ��ʱ���
	char illumination_statistics_timer_start;//��ʼʱ��
	char illumination_statistics_timer_stop;//����ʱ��
	//����ͳ�����
	int illumination_statistics_timer_delay;//�ڶ�ʱ�������ۼ�,����ͳ��ʱ����
	u16 illumination_statistics_temp[10];//�洢һ��ʱ��βɼ��Ĺ���ǿ��
	int illumination_statistics_cnt;
	u32 illumination_statistics_sum;//����ͳ������
	u16 illumination_statistics_count;//����ͳ�ƴ���
	
	
	//����ͳ�����
	int illumination_statistics_timer_delay1;//�ڶ�ʱ�������ۼ�,����ͳ��ʱ����
	u16 illumination_statistics_temp1[10];//�洢һ��ʱ��βɼ��Ĺ���ǿ��
	int illumination_statistics_cnt1;
	
	//����ʣ����
	char matrix_value;
	char matrix_value_copy;
	
	//����ֲ����ֲ��ʱ��
	time_t plant_value;
	
	//�ۼ��ջ�
	long cumulative_harvest_value;
	long cumulative_harvest_value_copy;
	
	//��ʼ��ֲʱ�·���ʱ��
	time_t begin_date_value;
	
	//��������
	unsigned int nurture_day;
	unsigned int nurture_day_copy;
	
	//ˮ��/ȱˮ,���Ʒ������ε�
	char water_full_flag;//ˮ����־
	char water_lack_flag;//ȱˮ��־
	
	
	
} sensor_control_struct;


SENSOR_CONTROL_Cx_ sensor_control_struct sensor_control_struct_value;


//ͳ�ƹ���ǿ��
void illumination_intensity_statistics(void);
//�¼��ϱ�����
void event_report_fun(void);
//�¼��ɼ�����(����1ms��ʱ��)
void event_loop_fun(void);
//ÿ����1��,���ʼ�һ , ����ֵ�ۼ�10
void NurtureDayFunction(void);
//ѭ����ˮ
void WateringCirculationFunction(void);
//���ƽ�ˮ
void sensor_watering(void);
//�ϱ�����������
void sensor_data_appear(void);
//��ʼ������������
void sensor_data_value_init(void);
//��ѹ��ʪ�ȵĻ���
int humidity_conversion(float Value);
//��ѵ�ɼ�����������
void sensor_data_collection(void);
//����ˮ��
void sensor_pump_set(char value);
//����ָʾ��
void sensor_light_set(char value);
//���Ʋ����
void sensor_fill_light_set(char value);
//���Ʒ�����
void sensor_buzzer_set(char value);
//�ָ���������
void factory_reset(void);
#endif
