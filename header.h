typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;
typedef signed int s16;
typedef unsigned long int u32;
typedef signed long int s32;

void uart_init(u16 baud);
void uart_tx(u8 d);
void uart_tx_string(u8 *s);
void uart_integer(s32 num);
void uart_display_bcd(u8 d);
void uart_float(float f);
void change_page1(void );

void i2c_start(void );
void i2c_stop(void );
void i2c_write(u8 d);
u8 i2c_read(void );
bit i2c_ack(void );
void i2c_noack(void );
void i2c_byte_write(u8 SA,u8 MA,u8 d);
u8 i2c_byte_read(u8 SA,u8 MA);
void i2c_write_ds1621(void );
void i2c_write_ds1307(void );
void i2c_read_ds1307(void );
void i2c_cmd_write(u8 DA,u8 cmd);
void i2c_memory_write(void );
void change_page(void );

void lcd_data(u8 d);
void lcd_cmd(u8 cmd);
void lcd_tx_string(u8 *s);
void lcd_init(void );
void lcd_integer(s32 num);
void lcd_float(float f1);
void display_cur_status(void );
void lcd_display_bcd(u8 d);
void delay_ms(s32 ms);

u16 spi_read_mcp3204(u8 ch_num );
void INT0_config(void);
void display_10_logs(void );