#define DS18B20_C_

#include "ds18b20.h"
#include "delay.h"
#include "DataParse.h"


/**
* @brief  ��ʼ��DS18B20��IO��,��⴫�����Ƿ����(��ֲ���滻�ڲ�����)
* @param  None
* @param  None
* @param  None
* @retval 1:δ�����������; 0:��⵽������
* @example 
**/ 	 
char DS18B20Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	 //ʹ��PORTA��ʱ�� 
	
 	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);

 	GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);    //���1
	
	DS18B20_Rst();

	return DS18B20_Check();
}

/**
* @brief  ����������Ϊ����������(��ֲ���滻�ڲ�����)
* @param  value:1-���; 0-����
* @param  None
* @param  None
* @retval None
* @example 
**/ 
void DS18B20_IO_Dir_SET(char value){
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(value){
		GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	}else{
		GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	}
}


/**
* @brief  ������������ߵ͵�ƽ(��ֲ���滻�ڲ�����)
* @param  value:1-����ߵ�ƽ; 0-����͵�ƽ
* @param  None
* @param  None
* @retval None
* @example 
**/ 
void DS18B20_IO_SET(char value){
	switch(value)
	{
		case 1:
			GPIO_SetBits(DS18B20_PORT,DS18B20_PIN);
		break;
		case 0:
			GPIO_ResetBits(DS18B20_PORT,DS18B20_PIN);
		break;
	}
}

/**
* @brief  ��ȡ��������ĸߵ͵�ƽ(��ֲ���滻�ڲ�����)
* @param  None
* @param  None
* @param  None
* @retval 1-�ߵ�ƽ; 0-�͵�ƽ
* @example 
**/
char DS18B20_IO_GET(void){
	return GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_PIN);
}


//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_Dir_SET(1); 	//SET PA0 OUTPUT
	DS18B20_IO_SET(0); 			//����DQ
	delay_us(750);    			//����750us
	DS18B20_IO_SET(1); 			//DQ=1 
	delay_us(15);     			//15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
char DS18B20_Check(void)  
{   
	unsigned char retry=0;
	DS18B20_IO_Dir_SET(0);//��Ϊ����
	while (DS18B20_IO_GET()&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
	while (!DS18B20_IO_GET()&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
	unsigned char data;
	DS18B20_IO_Dir_SET(1);//SET PA0 OUTPUT
	DS18B20_IO_SET(0);
	delay_us(2);
	DS18B20_IO_SET(1);
	DS18B20_IO_Dir_SET(0);//SET PA0 INPUT
	delay_us(12);
	if(DS18B20_IO_GET())data=1;
		else data=0;	 
	delay_us(50);           
	return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
char DS18B20_Read_Byte(void)    // read one byte
{        
	unsigned char i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}						    
	return dat;
}
//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(unsigned char dat)     
{             
	unsigned char j;
	unsigned char testb;
	DS18B20_IO_Dir_SET(1);//SET PA0 OUTPUT;
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			DS18B20_IO_SET(0);// Write 1
			delay_us(2);                            
			DS18B20_IO_SET(1);
			delay_us(60);             
		}
		else
		{
			DS18B20_IO_SET(0);// Write 0
			delay_us(60);             
			DS18B20_IO_SET(1);
			delay_us(2);                          
		}
	}
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
	DS18B20_Rst();	   
	DS18B20_Check();	 
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert
}

/********************************************************/  
/*DS18B20?CRC8????*/  
/********************************************************/   
u8 calcrc_1byte(unsigned char abyte)    
{    
   unsigned char i,crc_1byte;     
   crc_1byte=0;                //??crc_1byte???0   
   for(i = 0; i < 8; i++)    
    {    
      if(((crc_1byte^abyte)&0x01))    
         {    
           crc_1byte^=0x18;     
           crc_1byte>>=1;    
           crc_1byte|=0x80;    
          }          
       else     
          crc_1byte>>=1;   
       abyte>>=1;          
     }   
     return crc_1byte;   
}   

unsigned char calcrc_bytes(unsigned char *p,unsigned char len)  
{  
	unsigned char crc=0;  
	while(len--) //len??????????   
	{  
		crc=calcrc_1byte(crc^*p++);  
	}  
	return crc;  //??????crc?0,???????   
}  


//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-55~125�� 
short DS18B20_Get_Temp(void)
{
	unsigned char ack=0;
	unsigned char temp;
	unsigned char data[9];
	
	short tem;
	DS18B20_Start ();                    // ds1820 start convert
	DS18B20_Rst();
	ack = DS18B20_Check();
	if(ack){
		return 255;
	}
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0xbe);// convert	    
	data[0]=DS18B20_Read_Byte(); // LSB   
	data[1]=DS18B20_Read_Byte(); // MSB  
	    	  
	data[2] = DS18B20_Read_Byte();
	data[3] = DS18B20_Read_Byte();
	data[4] = DS18B20_Read_Byte();
	data[5] = DS18B20_Read_Byte();
	data[6] = DS18B20_Read_Byte();
	data[7] = DS18B20_Read_Byte();
	data[8] = DS18B20_Read_Byte();
	
	if(CRC8(data,8) != data[8]){
		return 255;
	}
	
	/*��ҪCRC8У��,���У��ʧ�ܷ��� 255*/
	if(data[1]>7)
	{
		data[1]=~data[1];
		data[0]=~data[0]; 
		temp=0;//�¶�Ϊ��  
	}else temp=1;//�¶�Ϊ��	  	  
	tem=data[1]; //��ø߰�λ
	tem<<=8;    
	tem+=data[0];//��õװ�λ
	tem=(float)tem*0.0625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}

