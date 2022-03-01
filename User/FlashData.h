#ifndef __FLASHDATA_H__
#define __FLASHDATA_H__
#include <stm32f10x.h>

typedef enum {/*����/��ȡ����ֵ����*/
	safe_type_temperature = 0,
  safe_type_humidity = 1,
  safe_type_illumination = 2,
	safe_type_fertility = 3,
}safe_type_typedef;

//��ˮģʽ
void FlashSetModeAuto(void);
void FlashSetModeManual(void);
u16 FlashGetModeWatering(void);

//��ˮ����
//��ˮ����:����
void FlashSetWaterCount(uint16_t data);
//��ˮ����:��ȡ
u16 FlashGetWaterCount(void);

//����ֲ������
void FlashSetBotanyType(long long data);
//��ȡֲ������
long long FlashGetBotanyType(void);

//����ֵ:����
void FlashSetLoveValue(uint16_t data);
//����ֵ:��ȡ
u16 FlashGetLoveValue(void);

//ҹ�䲻������ˮ 0:��  1:��
void FlashSetReportEvent(uint16_t data);
//ҹ�䲻������ˮ 0:��  1:��
u16 FlashGetReportEvent(void);
//����ƴ�ģʽ 0:�Զ�  1:�ֶ�
void FlashSetFillLightMode(uint16_t data);
//����ƴ�ģʽ 0:�Զ�  1:�ֶ�
u16 FlashGetFillLightMode(void);


void FlashSetSafe(uint16_t L,uint16_t H,safe_type_typedef safe_type);
void FlashGetSafe(uint16_t* L,uint16_t* H,safe_type_typedef safe_type);

void FlashSetSwitch(uint16_t data);//////////////
//����ƴ�ģʽ 0:�Զ�  1:�ֶ�
u16 FlashGetSwitch(void);

//���ù���ǿ�ȱ���ֵ
void FlashSetLightAlarm(int value);
//��ȡ����ǿ�ȱ���ֵ
int FlashGetLightAlarm(void);

//���Ļ���ʣ����
void FlashSetMatrixData(long long data);
//��ȡ���Ļ���ʣ����
long long FlashGetMatrixData(void);

//������ֲʱ��
void FlashSetDataPlant(long long data);
//��ȡ��ֲʱ��
long long FlashGetDataPlant(void);
//��������
void FlashSetNurtureDay(uint16_t data);
//��������
u16 FlashGetNurtureDay(void);

//�ۼ��ջ�:����
void FlashSetHarvestCount(uint16_t data);
//�ۼ��ջ�:��ȡ
u16 FlashGetHarvestCount(void);
//������ֲʱ��(�״�)
void FlashSetOneDataPlant(long long data);
//��ȡ��ֲʱ��(�״�)
long long FlashGetOneDataPlant(void);


//����LED״̬
void FlashSetLedState(uint16_t data);
//��ȡLED״̬
u16 FlashGetLedState(void);



//���Ĺ���ͳ��
//data ͳ�ƵĹ���ǿ��
//ͳ�ƵĴ���
void FlashSetIlluminationStatistics(u32 data,u16 count,long long time);

//��ȡ����ͳ��
//count ���ص�ͳ�ƴ���
//return ͳ�ƵĹ���ǿ��
u32 FlashGetIlluminationStatistics(u16* count,long long *time);



//���ù���ǿ��ͳ���ϱ�״̬(���)
void FlashClearIlluminationState(void);

//���ù���ǿ��ͳ���ϱ�״̬(����)
void FlashSetIlluminationState(void);

//��ȡ����ǿ��ͳ���ϱ�״̬
u16 FlashGetIlluminationState(void);

//��ȡ�豸�İ汾
u16 IAPGetHardwareVersion(void);
#endif

