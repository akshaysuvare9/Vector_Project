#include"header.h"
void delay_ms(s32 ms)
{
	u8 i;
	for(ms ;ms>=0;ms--)
	{
		for(i=250;i>0;i--);
		for(i=247;i>0;i--);
	}
}

//added 1st comment
