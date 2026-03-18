#include <REGX52.H>

sbit RCK = P3^5; //RCLK
sbit SCK = P3^6; //SRCLK
sbit SER = P3^4; //SER

unsigned char animaton[] = {
	0xFF, 0x08, 0x08, 0x08, 0xFF, 0x00, 0x0E, 0x15, 
	0x15, 0x15, 0x08, 0x00, 0x7E, 0x01, 0x02, 0x00,
  0x7F, 0x01, 0x02, 0x00, 0x0E, 0x11, 0x11, 0x0E, 
  0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char offset=0;

void _74HC595_WriteByte(unsigned char byte);
void LED_show(unsigned char a,b);
void MatrixLED_Init();
void Delay(unsigned int a);
void Timer0_Init();

void main()
{
	unsigned char i;
	
	MatrixLED_Init();
	Timer0_Init();
	
	while(1)
	{
		for(i=0;i<8;i++)
		{
			LED_show(i,animaton[i+offset]);
		}
	}
}

void Timer0_Init()
{
	//AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;		  	//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	//中断操作
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

unsigned int T = 0;

void Timer0_Routine(void) interrupt 1
{
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	T++;
	if(T >= 500)
	{
		offset++;
		if(offset > 25)
			offset = 0;
		T = 0;
	}
		
}

void MatrixLED_Init()
{
	SCK = 0;
	RCK = 0;
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