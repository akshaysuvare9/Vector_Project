#include"header.h"
#define CH0 0
#define CH1 1
volatile u8 pcnt=0,id=1,hr,min,sec,day,date,mon,yr,temp,tempmsb;
volatile u16 light,pot_vtg;
u8 DA_mr=0xA0;
bdata flag;
main(void )
{
	u8 var=0;
	uart_init(9600);	//initialization of UART
	INT0_config();		//interrupt INT0 configuration
	lcd_init();				//initialization of LCD
	lcd_tx_string("VECTOR INDIA");
	lcd_cmd(0xc0);
	lcd_tx_string("REAL TIME WEATHER");
	lcd_cmd(0x94);
	lcd_tx_string("MONITORING SYSTEM");
	delay_ms(1000);
	lcd_cmd(0x01);
	i2c_write_ds1621();	//initialization of digital thermometer
	i2c_write_ds1307();	//initialization of real time clock 
	while(1)
	{
		i2c_read_ds1307();	//reading time & date
		temp=i2c_byte_read(0x90,0xAA);	//reading temperarure
	  tempmsb=i2c_byte_read(0x90,0xAB); //reading MSB bit
		light=spi_read_mcp3204(CH0);		//reading light intensity
		pot_vtg=spi_read_mcp3204(CH1);	//reading pot voltage
		if(var!=min)
		{
				//uart_tx_string("memory write\r\n");
				i2c_memory_write();		//writting data into memory
				var=min;
		}
		display_cur_status();		//displaying current status 
	if(flag==1)
	{
		display_10_logs();		//displaying 3 logs after interrupting the uc
		flag=0;
	}
}
}