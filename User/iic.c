#include "iic.h"
#include "delay.h"
#include <math.h>
#include "main.h"

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PA�˿�ʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
//	IIC_SDA=1;
//	delay_us(1);
//	IIC_SDA=0;
	IIC_SDA=1;
	delay_us(1);
	IIC_SCL=1;
	delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(5);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
//	SDA_OUT();//sda�����
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(4);					
	SDA_OUT();
	IIC_SDA=0;
	delay_us(1);
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=1;
	delay_us(5);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
u8 IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	u8 ucErrTime=0;
	SDA_OUT(); 	    
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
	SDA_IN();//SDA����Ϊ����
	delay_us(10);
	IIC_SCL=1;
	
	while(READ_SDA==1 && ucErrTime<200){
		ucErrTime++;
	}
	if(READ_SDA==0 && ucErrTime<200){
		IIC_SCL=0;
		delay_us(2);
		return 1;
	}else{
		IIC_SCL=0;
		delay_us(2);
		return 0;
	}
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
	}					 
	if (!ack)
			IIC_NAck();//����nACK
	else
			IIC_Ack(); //����ACK   
	return receive;
}



int iic_read_byte(char addr){
	u8 ack=0;
	u8 data;
	IIC_Start();
	ack = IIC_Send_Byte(0x94);//��Ҫд���㶫��
	delay_us(2); 
	//��Ҫ����Ӧ��
	if(!ack) return -1;
	
	ack = IIC_Send_Byte(addr);//�������ַ
	delay_us(2); 
	//��Ҫ����Ӧ��
	if(!ack) return -1;
	
	IIC_Start();
	ack = IIC_Send_Byte(0x95);//��Ҫ��ȡ�ϴθ����ַ�����ֵ
	delay_us(2); 
	//��Ҫ����Ӧ��
	if(!ack) return -1;
	
	data = IIC_Read_Byte(0);//��ȡһ���ֽ�,����ҪӦ��
	
	IIC_Stop();
	return(data);
}

//��ȡ����ǿ��
int illumination_get(void)
{
	int value=0;
	char temp[2];
	float sensor_lignt_exponent,sensor_lignt_mantissa;
	u8 ack=0;
	int data=0;
	static int delay=0;
	
	if(HardwareVersion == 1){//BH1750
		if(delay==0){
			IIC_Start();
			ack = IIC_Send_Byte(0x46);//��ַ+д
			delay_us(2); 
			//��Ҫ����Ӧ��
			if(!ack) return -1;
			IIC_Send_Byte(0x01);//����
			IIC_Stop();

			IIC_Start();
			ack = IIC_Send_Byte(0x46);//��ַ+д
			delay_us(2); 
			//��Ҫ����Ӧ��
			if(!ack) return -2;
			IIC_Send_Byte(0x10);
			IIC_Stop();
		}
		
		delay = delay+1;
		
		if(delay>=150){
			delay=0;
			IIC_Start();
			ack = IIC_Send_Byte(0x47);
			delay_us(2); 
			//��Ҫ����Ӧ��
			if(!ack) return -3;

			data = IIC_Read_Byte(1);
			data = data<<8;
			data +=0x00ff&IIC_Read_Byte(0);

			IIC_Stop();                        //?????? 
		}else{
			data=-4;
		}
		return data; 
	}
	else
	{
		value = iic_read_byte(0x03);
		if(value == -1){
			
			return -1;
		}

		temp[0] = value;

		value = iic_read_byte(0x04);
		if(value == -1){
			return -1;
		}

		temp[1] = value;

		sensor_lignt_exponent = (temp[0] & 0xF0) >> 4;
		sensor_lignt_mantissa = ((temp[0] & 0x0F) << 4) | (temp[1] &0x0F); 
		value = sensor_lignt_mantissa * pow(2, sensor_lignt_exponent)  * 0.045;

		return value;
	}
	
	
//	IIC_Start();
//	ack = IIC_Send_Byte(0x94);//��Ҫд���㶫��
//	delay_us(2); 
//	//��Ҫ����Ӧ��
//	if(!ack) return -1;
//	
//	ack = IIC_Send_Byte(addr);//�������ַ
//	delay_us(2); 
//	//��Ҫ����Ӧ��
//	if(!ack) return -1;
//	
//	IIC_Start();
//	ack = IIC_Send_Byte(0x95);//��Ҫ��ȡ�ϴθ����ַ�����ֵ
//	delay_us(2); 
//	//��Ҫ����Ӧ��
//	if(!ack) return -1;
//	
//	data = IIC_Read_Byte(0);//��ȡһ���ֽ�,����ҪӦ��
//	
//	IIC_Stop();
//	return(data);
}

