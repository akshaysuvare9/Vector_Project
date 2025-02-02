#include<reg51.h>
#include<intrins.h>
#include"header.h"
sbit SDA=P2^0;
sbit SCL=P2^1;
void i2c_start()
{
SCL=1;
	SDA=1;
	SDA=0;
}

void i2c_stop()
{
SCL=0;
	SDA=0;
	SCL=1;
	SDA=1;
}
void i2c_write(u8 d)
{
s8 i;
	for(i=7;i>=0;i--)
	{
	SCL=0;
		SDA=(d&(1<<i))?1:0;
		SCL=1;
	}
}
u8 i2c_read()
{
s8 i;
	u8 temp=0;
	for(i=7;i>=0;i--)
	{
	SCL=0;
		if(SDA==1)
			temp|=(1<<i);
		SCL=1; 
	}
	return temp;
}
bit i2c_ack()
{
SCL=0;
	SDA=1;
	SCL=1;
	_nop_();
	if(SDA==0)
	{
	SCL=0;
		return 0;
	}
	else
	{
	SCL=0;
		return 1;
	}
}
void i2c_noack()
{
SCL=0;
	SDA=1;
	SCL=1;
}