#ifndef DS18B20_Cc_
#define DS18B20_Cc_

#ifndef DS18B20_Cx_//���û�ж���
#define DS18B20_Cx_ extern
#else
#define DS18B20_Cx_
#endif

/*config pin*/
#define DS18B20_CLK    RCC_APB2Periph_GPIOA
#define DS18B20_PIN    GPIO_Pin_7
#define DS18B20_PORT   GPIOA


//��ʼ��DS18B20��IO��,��⴫�����Ƿ����(��ֲ���滻�ڲ�����) 
char DS18B20Init(void);
//��λDS18B20
void DS18B20_Rst(void);
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
char DS18B20_Check(void);

short DS18B20_Get_Temp(void);
#endif

