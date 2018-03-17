/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051 12MHz
  日    期: 2011-8-9
*/
#include "ds18b20.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0; i<12; i++);
	}
}


//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20设备初始化
bit Ds18b20_Init(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    	initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

//从DS18B20读取温度值
unsigned char Temper_Read()
{
	unsigned char temp,Tl,Th;
	Ds18b20_Init();		   //DS18B20初始化
	Write_DS18B20(0xcc);   //跳过ROM的字节命令
	Write_DS18B20(0x44);   //开始转换指令
	Delay_OneWire(200);	   //延时一段时间

  Ds18b20_Init();		   //DS18B20初始化
	Write_DS18B20(0xcc);   //跳过ROM的字节命令
	Write_DS18B20(0xbe);   //读取指令
	
	Tl=Read_DS18B20();     //读低八位
	Th=Read_DS18B20();	   //读高八位
	temp=(Th<<4)|(Tl>>4);
	return temp;
}






