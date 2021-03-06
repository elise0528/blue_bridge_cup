/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(12MHz)
  日    期: 2011-8-9
*/

#include "iic.h"


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//应答位控制
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

void ADC_Init()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_Stop();
}

u8 ADC_Read()
{
	u8 temp;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();	
	temp=IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return temp;
}

/*
void E2PROM_Write(u8 addr,u8 dat[],u8 num)
{
	u8 i;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	for(i=0;i<num;i++)
	{
		IIC_SendByte(dat[i]);
		IIC_WaitAck();		
	}
	IIC_Stop();	
}

void E2PROM_Read(u8 addr,u8 dat[],u8 num)
{
	u8 i;

	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	for(i=0;i<num;i++)
	{
		dat[i]=IIC_RecByte();
		IIC_Ack(1);	
	}
	IIC_Ack(0);
	IIC_Stop();	
}
*/

void WriteByte_AT24C02(u8 add,u8 date)
{
	IIC_Start();
	IIC_SendByte(0xa0);//发方式字1010 0000
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();	
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_Stop();
	Delay10ms();
}

u8 ReadByte_AT24C02(u8 add)
{
	u8 date;
	IIC_Start();
	IIC_SendByte(0xa0);//发方式字1010 0000
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();

	IIC_Start();	//注意！！！
	IIC_SendByte(0xa1);//发方式字1010 0001
	IIC_WaitAck();
	date = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return date;
}
