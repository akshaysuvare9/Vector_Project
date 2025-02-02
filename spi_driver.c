#include<reg51.h>
#include"header.h"
sbit clk=P1^0;
sbit din=P1^1;
sbit dout=P1^2;
sbit cs=P1^3;
u16 spi_read_mcp3204(u8 ch_num)
{
	u16 temp=0;
	s8 i;
	cs=0;
	clk=0; din=1; clk=1;
	clk=0; din=1; clk=1;
	clk=0;din=(ch_num>>2)&1;clk=1;
	clk=0;din=(ch_num>>1)&1;clk=1;
	clk=0;din=ch_num&1;clk=1;
	clk=0;din=1;clk=1;
	clk=0;din=1;clk=1;
	for(i=11;i>=0;i--)
	{
		clk=0;
		if(dout==1)
		temp|=(1<<i);
		clk=1;
	}
	cs=1;
	return temp;
}