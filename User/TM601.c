

#include "TM601.h"
//***************************************************************************  
void i2c_delay()//����ʱ����  
{ 
	char i;
	for(i=0;i<2;i++); }  
//***************************************************************************  
void i2c_start()  //��ʼ�ź� SCL�ڸߵ�ƽ�ڼ䣬SDAһ���½������ʾ�����ź�  
{     
	TM601_SDA_OUT=1; //�ͷ�SDA����  
	i2c_delay();  
	SCL=1;  
	i2c_delay();  
	TM601_SDA_OUT=0;  
	i2c_delay();  
	SCL=0;
	i2c_delay(); 
}  
//***************************************************************************  
void i2c_stop()   //ֹͣ SCL�ڸߵ�ƽ�ڼ䣬SDAһ�����������ʾֹͣ�ź�  
{  
    SCL=1;  
}
//***************************************************************************  
char i2c_checkack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  
	char	bit_temp;  
	SCL=1;  
	bit_temp=TM601_SDA_IN;
	i2c_delay();  
	if(bit_temp)
		//��ACK��Ӧ
		return 0;
	SCL=0;  
	return 1;  
}  
//***************************************************************************  
void i2c_sendack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  
	TM601_SDA_OUT=0;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
	TM601_SDA_OUT=1;

}  
//***************************************************************************  
void i2c_sendnack()  //Ӧ�� SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��  
{  
	TM601_SDA_OUT=1;
	i2c_delay();
	SCL=1;
	i2c_delay();
	SCL=0;
}
//***************************************************************************  
void i2c_write_byte(unsigned char date) //дһ���ֽ�  
{  
	unsigned char i,temp,m;  
	temp=date;  

	for(i=0;i<8;i++)  
	{  
		m=temp;
		m=m&0x80;
		if(m==0x80){
			TM601_SDA_OUT=1;
		}
		else 
			TM601_SDA_OUT=0;
		temp=temp<<1;
		SCL=1;
		SCL=0;
	}  
	TM601_SDA_OUT=1;//�ͷ�SDA���ߣ��������ɴ��豸���ƣ�������豸���������ݺ���SCLΪ��ʱ������SDA��ΪӦ���ź�  
	i2c_delay();  
}  
//***************************************************************************  
unsigned char i2c_read_byte()//��һ���ֽ�  
{  
	unsigned char i,k;  

	for(i=0;i<8;i++)  
	{
		i2c_delay();  
		SCL=1;//������ʱ��IIC�豸�����ݷ���sda���ϣ����ڸߵ�ƽ�ڼ������Ѿ��ȶ������Խ�����     
		k=(k<<1)|TM601_SDA_IN;  
		SCL=0;
	}  
	return k;  
}  
//***************************************************************************  
char	i2c_write(unsigned dev_addr, unsigned reg_addr, unsigned char * src_buf, unsigned char len)
{
	char i;
	i2c_start();
	i2c_write_byte((dev_addr<<1));//���ͷ��ʹ��豸��ַ д���� 
	if(~i2c_checkack())
	{
		return 0;
	}
	i2c_delay();  		 
	i2c_write_byte(reg_addr);//���ͷ��ͼĴ�����ַ д���� 
	if(~i2c_checkack())
	{
		return 0; 
	}
	i2c_delay();  
	for(i=0;i<len;i++)
	{
		i2c_write_byte(src_buf[i]);//���ͻ��������� д���� 
		i2c_checkack();//�ȴ����豸����Ӧ  
		i2c_delay();  				
	}
	i2c_stop();//ֹͣ 
	return 1;
}
//***************************************************************************  
char	i2c_read_direct(unsigned dev_addr,unsigned char * dest_buf,unsigned char len)
{
	char i;
	i2c_start();//����  
	i2c_write_byte((dev_addr<<1)+1);//���ͷ��ʹ��豸��ַ ������  
	
	if(~i2c_checkack())
		return 0;//�ȴ����豸����Ӧ  
	i2c_delay();
	dest_buf[0]=i2c_read_byte();//��ȡ���� 
	for(i=1;i<len;i++)
	{
		i2c_sendack();
		i2c_delay();
		dest_buf[i]=i2c_read_byte();
	}
	i2c_sendnack();
	i2c_delay();
	i2c_stop();//ֹͣ  
	return 1;
}
//***************************************************************************  
/*
���ܣ�33MHzƵ���¼���ʱ500us
���룺��
�������
*/
void Delay500us() 
{ 
	unsigned int i;
	for(i=0;i<800;i++);
	for(i=0;i<800;i++);
} 
//***************************************************************************  
/*
���ܣ���ȡ����ǰ601�Ĳ���ֵ
���룺chNumΪ��Ҫ��ȡ����ֵ��ͨ���ţ�
			sampleΪ��ȡ������ֵ����Ҫ�����ı�����
�����1��ʾ��ȡ�ɹ�0��ʾ��ȡʧ��
*/
char ReadThresholdFrom601(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char buffer[4];
	unsigned char CON_RESA2EEP=0xCA;
	char flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_RESA2EEP, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
	{
		return 0;
	}
	for(i=0;i<10;i++)
	{
		if(i2c_read_direct(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
		{	
			return 0;
		}
		if(buffer[0]==0xCD)
		{
			flag=1;
			break;
		}
		else if(j>=9)
		{
			return 0;
		}
		else
			Delay500us();
	}

	if(flag==1)
	{
		chnum=chNum+0x30;
		if(i2c_write(CON_ADDR, 0x01,&chnum, 1)==0)//���ֳɹ��󣬷���ˮλоƬ������ͨ����ż���0x30
			return 0;
		for(j=0;j<10;j++)
		{
			if(i2c_read_direct(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;
			if(buffer[0]==chNum)//�ж�ˮλоƬ�����ɹ��󣬽��õ��Ĳ���ֵ��ֵ��ָ��
			{
				*sample=buffer[1]*256+buffer[2];
				break;
			}
			else if(j>=9)
			{
				return 0;
			}
			else
				Delay500us();
		}
		return 1;
	}
	else
		return 0;
}
//***************************************************************************  
/*
���ܣ�����ǰ��Ҫ���õĲ���ֵ������601��
���룺chnelnumΪ��Ҫ���ò���ֵ��ͨ���ţ�
			sampleΪ������601�Ĳ���ֵ������
�����1��ʾ���óɹ�0��ʾ����ʧ��
*/
char SetThresholdTo601(unsigned char chNum,unsigned int *sample)
{
	unsigned char i,j,chnum;
	unsigned char setsampl[2];
	unsigned char buffer[4];
	unsigned char CON_SET601CR=0x8C;
	char flag=0;
	if(i2c_write(CON_ADDR, 0x01,&CON_SET601CR, 1)==0)//���е�Ƭ����ˮλоƬ���ֲ���
		return 0;
	for(i=0;i<10;i++)
	{
		if(i2c_read_direct(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ����е����ݣ��жϵ�Ƭ����ˮλоƬ�Ƿ����ֳɹ�
			return 0;	
		if(buffer[0]==0xCD)
		{
			flag=1;
			break;
		}
		else if(j>=9)
			return 0;
		else
			Delay500us();
	}
	if(flag==1)//���ֳɹ��󣬷�����Ҫ���õ�ˮλоƬ������ͨ����ż���0x30
	{
		chnum=chNum+0x30;
		setsampl[0]=(*sample)/256;
		setsampl[1]=(*sample)%256;
		if(i2c_write(CON_ADDR, 0x02, setsampl, 2)==0)	
			return 0;	
		if(i2c_write(CON_ADDR, 0x01, &chnum, 1)==0)			
			return 0;	
		for(j=0;j<10;j++)
		{
			if(i2c_read_direct(CON_ADDR,buffer,0x03)==0)//��ѯˮλоƬ�Ĵ������ж��Ƿ�����ɹ�
				return 0;	
			if(buffer[0]==chNum)
				break;
			else if(j>=9)				
				return 0;	
			else
				Delay500us();
		}
		return 1;
	}
	else
		return 0;
}
