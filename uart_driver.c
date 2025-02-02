#include<reg51.h>
#include"header.h"
static u8 pcnt1=0,DA_mr1=0xA0;
void uart_init(u16 baud)
{
	TMOD|=0x20;
	SCON=0x50;
switch(baud)
{
	case 9600:TH1=253;break;
	case 28800:TH1=255;break;
	case 56700:TH1=255; PCON|=0x80; break;
	default:TH1=253;
}
TR1=1;
}
void uart_tx(u8 d)
{
SBUF=d;
	while(TI==0);
	TI=0;
}
void uart_tx_string(u8 *s)
{
while(*s)
{
SBUF=*s++;
	while(TI==0);
	TI=0;
}
}
void uart_integer(s32 num1)
{
	bdata flag=0;
	u8 c=0;
	u32 num=0;
	if(num1==0)
	{	
	uart_tx('0');
	return;
	}
	if(num1<0)
	{
	uart_tx('-');
		num1=-num1;
	}
	while(num1)
	{
		if(flag==0)
		{
		if((num1%10)==0)
			c++;
		else
			flag=1;
		}
	num=num*10+(num1%10);
		num1/=10;
	}
	while(num)
	{
	uart_tx(num%10+48);
		num/=10;
	}
	while(c>0)
	{
		uart_tx('0');
		c--;
	}
}
//u8 bcdtodec(u8 num)
//{
//	return (((num & 0xf0)>>4) *10)+(num & 0x0f);
//}
void uart_display_bcd(u8 d)
{
	uart_tx(d/16+48);
	uart_tx(d%16+48);
}
void change_page1(void )
{
	if(pcnt1==255)
	{
		pcnt1=0; 
		if(DA_mr1==0xA6)
		{
			DA_mr1=0xA0;
			return;
		}
		DA_mr1+=2;
	}
	else
		pcnt1++;
}
void display_10_logs(void )
{
	u8 cnt=10,temp=0,temp1=0;
	u16 yr1,temp_16=0;
	uart_tx_string("LOG ID  TIME        DATE             TEMPERATURE  LIGHT INTENSITY  POT READING\r\n");
	while(cnt)
	{
	uart_integer(i2c_byte_read(DA_mr1,pcnt1++));
	uart_tx_string("       ");
		temp=i2c_byte_read(DA_mr1,pcnt1);												
change_page1();
uart_display_bcd(temp&0x1F);											//hour
		uart_tx(':');
		uart_display_bcd(i2c_byte_read(DA_mr1,pcnt1++));		//min
		uart_tx(':');
		uart_display_bcd(i2c_byte_read(DA_mr1,pcnt1));		//sec
change_page1();		
		if((temp>>5)&1)
			uart_tx_string("PM  ");
		else
			uart_tx_string("AM  ");
	temp=i2c_byte_read(DA_mr1,pcnt1++);									//day
		if(temp==1)
		uart_tx_string("Sun ");
	else if(temp==2)
		uart_tx_string("Tue ");
	else if(temp==3)
		uart_tx_string("Wed ");
	else if(temp==4)
		uart_tx_string("Thu ");
	else if(temp==5)
		uart_tx_string("Tue ");
	else if(temp==6)
		uart_tx_string("Fri ");
	else
		uart_tx_string("Sat ");
		uart_display_bcd(i2c_byte_read(DA_mr1,pcnt1));		//date
change_page1();	
	uart_tx(' ');
	temp=i2c_byte_read(DA_mr1,pcnt1++);									//month		
	switch((((temp & 0xf0)>>4) *10)+(temp & 0x0f))
	{
		case 1:uart_tx_string("Jan "); break;
		case 2:uart_tx_string("Feb "); break;
		case 3:uart_tx_string("Mar "); break;
		case 4:uart_tx_string("Apr "); break;
		case 5:uart_tx_string("May "); break;
		case 6:uart_tx_string("Jun "); break;
		case 7:uart_tx_string("Jul "); break;
		case 8:uart_tx_string("Aug "); break;
		case 9:uart_tx_string("Sep "); break;
		case 10:uart_tx_string("Oct "); break;
		case 11:uart_tx_string("Nov "); break;
		case 12:uart_tx_string("Dec "); break;
	}
	temp=i2c_byte_read(DA_mr1,pcnt1);	//year
change_page1();	
	yr1=2000;
	yr1+=(((temp & 0xf0)>>4) *10)+(temp & 0x0f);
	uart_integer(yr1);
	uart_tx_string("  ");
	temp=i2c_byte_read(DA_mr1,pcnt1++);	//temperatute
	if(temp>200)
		{
			uart_tx('-');
			temp=~temp;
			temp++;
		}
		temp1=i2c_byte_read(DA_mr1,pcnt1);
change_page1();
		if((temp1>>7)&1)
		uart_float(temp+0.5);
		else
		uart_float(temp);
		uart_tx(248);
		uart_tx_string("c");
		uart_tx_string("      ");
		temp=i2c_byte_read(DA_mr1,pcnt1++);		//light intensity lower byte
		temp_16=temp;													
		temp=i2c_byte_read(DA_mr1,pcnt1);		//higher byte
change_page1();		
		uart_integer((temp_16+((temp<<8)&0xff00))/39);
		uart_tx('%');
		uart_tx_string("              ");
		temp=i2c_byte_read(DA_mr1,pcnt1++);		//POT voltage
		temp_16=temp;													//lower byte
		temp=i2c_byte_read(DA_mr1,pcnt1);		//higher byte
change_page1();
		uart_float(((3.3/4095)*(temp_16+((temp<<8)&0xff00))));
		cnt--;
		uart_tx_string("V\r\n");
		temp=temp1=temp_16=0;
	}
}
void uart_float(float f1)
{
s32 num;
	if(f1<0)
	{
	uart_tx('-');
		f1=-f1;
	}
	num=f1;
	uart_integer(num);
	uart_tx('.');
	if((f1-num)<=0.09f)
	{
	uart_tx('0');
	}
num=(f1-num)*100;
	if(num==0)
		uart_tx('0');
	else
	uart_integer(num);
}