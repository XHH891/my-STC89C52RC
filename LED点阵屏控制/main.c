#include <REGX52.H>

sbit RCK = P3^5; //RCLK
sbit SCK = P3^6; //SRCLK
sbit SER = P3^4; //SER

void _74HC595_WriteByte(unsigned char byte);
void LED_show(unsigned char a,b);
void Delay(unsigned int a);

void main()
{
	SCK = 0;
	RCK = 0;

	while(1)
	{
		LED_show(0,0x78);
		LED_show(1,0xfc);
		LED_show(2,0xfe);
		LED_show(3,0x7f);
		LED_show(4,0x7f);
		LED_show(5,0xfe);
		LED_show(6,0xfc);
		LED_show(7,0x78);
	}
}

void LED_show(unsigned char a,b)
{
	//通过段选  位选  时延  清零的方式来消除残影
	_74HC595_WriteByte(b); //段选
	P0 = ~(0x80>>a);//位选
	Delay(1);//时延
	P0 = 0xff;//清零
}

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

void Delay(unsigned int a)
{
	unsigned char i,j;

	while(a)
	{
		i = 4;
		j = 239;
		do
		{
			while(--j);
		}while(--i);
		a--;
	}
}