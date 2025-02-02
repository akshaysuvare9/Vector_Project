#include<reg51.h>
#include"header.h"
extern u8 pcnt,hr,min,sec,day,date,mon,yr,temp,tempmsb,DA_mr,id;
extern u16 light,pot_vtg;
sbit RS=P0^0;
sbit RW=P0^1;
sbit EN=P0^2;
extern bdata flag;
void INT0_handler(void ) interrupt 0
{
	flag=1;
}
void INT0_config(void)
{
	EA=EX0=1;
	IT0=1;
}

void lcd_init()
{
	lcd_cmd(0x2);
	lcd_cmd(0x28);
	lcd_cmd(0x0c);
	lcd_cmd(0x06);
	lcd_cmd(0x1);
}

void lcd_cmd(u8 cmd)
{
	P0=(P0 & 0x0f)|(cmd & 0xf0);
	RS=0;
	RW=0;
	EN=1;
	delay_ms(10);
	EN=0;
	P0=(P0 & 0x0f)|(cmd <<4);
	EN=1;
	delay_ms(10);
	EN=0;
}
void lcd_data(u8 d)
{
	P0=(P0 & 0x0f) | (d & 0xf0);
	RS=1;
	RW=0;
	EN=1;
	delay_ms(10);
	EN=0;
	P0=(P0 & 0x0f) | (d <<4);
	EN=1;
	delay_ms(10);
	EN=0;
}
void lcd_tx_string(u8 *s)
{
s8 i;
	for(i=0;s[i];i++)
	lcd_data(s[i]);
}
void lcd_integer(s32 num1)
{
	bdata flag=0;
	u8 c=0;
	u32 num=0;
	if(num1==0)
	{	
	lcd_data('0');
	return;
	}
	if(num1<0)
	{
	lcd_data('-');
		num1=-num1;
	}
	while(num1)
	{
		if(flag==0)
			((num1%10)==0)?c++:(flag=1);
	num=num*10+(num1%10);
		num1/=10;
	}
	while(num)
	{
	lcd_data(num%10+48);
		num/=10;
	}
	while(c>0)
	{
		lcd_data('0');
		c--;
	}
}
void lcd_display_bcd(u8 d)
{
	lcd_data(d/16+48);
	lcd_data(d%16+48);
}
void display_cur_status(void )
{
	bdata am_pm;
	u16 yr1=2000;
	am_pm=(hr>>5)&1;
	hr=hr & 0x1F;
	lcd_display_bcd(hr);
	lcd_data(':');
	lcd_display_bcd(min);
	lcd_data(':');
	lcd_display_bcd(sec);
	am_pm?lcd_tx_string(" PM "):lcd_tx_string(" AM ");
	if(day==1) lcd_tx_string("Sun");
	else if(day==2) lcd_tx_string("Tue");
	else if(day==3) lcd_tx_string("Wed");
	else if(day==4) lcd_tx_string("Thu");
	else if(day==5) lcd_tx_string("Tue");
	else if(day==6) lcd_tx_string("Fri");
	else lcd_tx_string("Sat");
	lcd_cmd(0xc0);
	lcd_display_bcd(date);
	lcd_data(' ');
	switch((((mon & 0xf0)>>4) *10)+(mon & 0x0f))
	{
		case 1:lcd_tx_string("Jan "); break;
		case 2:lcd_tx_string("Feb "); break;
		case 3:lcd_tx_string("Mar "); break;
		case 4:lcd_tx_string("Apr "); break;
		case 5:lcd_tx_string("May "); break;
		case 6:lcd_tx_string("Jun "); break;
		case 7:lcd_tx_string("Jul "); break;
		case 8:lcd_tx_string("Aug "); break;
		case 9:lcd_tx_string("Sep "); break;
		case 10:lcd_tx_string("Oct "); break;
		case 11:lcd_tx_string("Nov "); break;
		case 12:lcd_tx_string("Dec "); break;
	}
//	yr1+=(((yr & 0xf0)>>4) *10)+(yr & 0x0f);
//	lcd_integer(yr1);
	lcd_display_bcd(yr);
	lcd_cmd(0x94);
	lcd_tx_string("TEMPERATURE=");
	if(temp>200)
		{
			lcd_data('-');
			temp=~temp;
			temp++;
		}
		if((tempmsb>>7)&1)
		lcd_float(temp+0.5);
		else
		lcd_float(temp);
		lcd_data(223);
		lcd_data('C');
		lcd_cmd(0xD4);
		lcd_tx_string("Lgt intensity=");
		lcd_integer(light/39);
		lcd_data('%');
		delay_ms(500);
		lcd_cmd(0x01);
		lcd_tx_string("POT vtg=");
		lcd_float((3.3/4095)*pot_vtg);
		lcd_data('V');
		delay_ms(500);
		lcd_cmd(0x1);
}
void lcd_float(float f1)
{
s32 num;
	if(f1<0)
	{
	lcd_data('-');
		f1=-f1;
	}
	num=f1;
	lcd_integer(num);
	lcd_data('.');
	if((f1-num)<=0.09f)
	{
	lcd_data('0');
		
	}
num=(f1-num)*100;
	if(num==0)
		lcd_data('0');
	else
	lcd_integer(num);
}