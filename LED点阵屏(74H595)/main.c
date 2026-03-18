#include <REGX52.H>

sbit RCK = P3^5; //RCLK
sbit SCK = P3^6; //SRCLK
sbit SER = P3^4; //SER

void _74HC595_WriteByte(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SER = byte&(0x80>>i);//通过与每一位做&得到 byte 的每一位。
		SCK = 1;
		SCK = 0;
	}
	RCK = 1;
	RCK = 0;
}

void main()
{
	SCK = 0;
	RCK = 0;
	
	P0 = 0;
	_74HC595_WriteByte(0xff);
	
	while(1)
	{
	}
}