#include"header.h"
#define AM 0
#define PM 1
extern u8 pcnt,hr,min,sec,day,date,mon,yr,temp,tempmsb,DA_mr,id;
extern u16 light,pot_vtg;
void change_page(void )
{
	if(pcnt==255)
	{
		pcnt=0; 
		if(DA_mr==0xA6)
		{
			DA_mr=0xA0;
			id=0;
			return;
		}
		DA_mr+=2;
	}
	else
		pcnt++;
}
void i2c_memory_write()
{
			i2c_byte_write(DA_mr,pcnt++,id++); delay_ms(10);	
			i2c_byte_write(DA_mr,pcnt,hr);	delay_ms(10);	//16
change_page();
			i2c_byte_write(DA_mr,pcnt++,min);	delay_ms(10);
			i2c_byte_write(DA_mr,pcnt,sec);	delay_ms(10);	//4
change_page();
			i2c_byte_write(DA_mr,pcnt++,day);	delay_ms(10);
			i2c_byte_write(DA_mr,pcnt,date);	delay_ms(10);	//20
change_page();
			i2c_byte_write(DA_mr,pcnt++,mon);	delay_ms(10);
			i2c_byte_write(DA_mr,pcnt,yr);	delay_ms(10);		//8
change_page();
			i2c_byte_write(DA_mr,pcnt++,temp);	delay_ms(10);		//LSB
			i2c_byte_write(DA_mr,pcnt,tempmsb);	delay_ms(10);	//MSB		//24
change_page();
			i2c_byte_write(DA_mr,pcnt++,(light&0xff));	delay_ms(10);	
			i2c_byte_write(DA_mr,pcnt,(light>>8)&0x00ff);	delay_ms(10);	//12
change_page();
	i2c_byte_write(DA_mr,pcnt++,pot_vtg&0x00ff);	delay_ms(10);
	i2c_byte_write(DA_mr,pcnt,(pot_vtg>>8)&0x00ff);	delay_ms(10);	//28
change_page();
}
void i2c_read_ds1307(void )
{
		sec=i2c_byte_read(0xD0,0x0);
		min=i2c_byte_read(0xD0,0x1);
		hr=i2c_byte_read(0xD0,0x2);
		day=i2c_byte_read(0xD0,0x3);
		date=i2c_byte_read(0xD0,0x4);
		mon=i2c_byte_read(0xD0,0x5);
		yr=i2c_byte_read(0xD0,0x6);
}
void i2c_write_ds1621(void )
{
	i2c_byte_write(0x90,0xAC,0x2);	//access config. active high op, contineous conv.
	i2c_cmd_write(0x90,0xEE);		//start conv.
}
void i2c_write_ds1307(void )
{
	i2c_byte_write(0xD0,0x0,0x55);	//sec
	i2c_byte_write(0xD0,0x1,0x59);	//min
	i2c_byte_write(0xD0,0x2,0x11|(1<<6)|(1<<5));	//hr
	i2c_byte_write(0xD0,0x3,0x6);		//day
	i2c_byte_write(0xD0,0x4,0x31);	//date
	i2c_byte_write(0xD0,0x5,0x12);	//month
	i2c_byte_write(0xD0,0x6,0x20);	//year
}
u8 i2c_byte_read(u8 SA,u8 MA)
{
u8 temp=0;
	i2c_start();
	i2c_write(SA);
	i2c_ack();
	i2c_write(MA);
	i2c_ack();
	i2c_start();
	i2c_write(SA|1);
	i2c_ack();
	temp=i2c_read();
	i2c_noack();
	i2c_stop();
	return temp;
}
void i2c_cmd_write(u8 DA,u8 cmd)
{
i2c_start();
	i2c_write(DA);
	i2c_ack();
	i2c_write(cmd);
	i2c_ack();
	i2c_stop();
}
void i2c_byte_write(u8 SA,u8 MA,u8 d)
{
	i2c_start();
	i2c_write(SA);
	i2c_ack();
	i2c_write(MA);
	i2c_ack();
	i2c_write(d);
	i2c_ack();
	i2c_stop();
}