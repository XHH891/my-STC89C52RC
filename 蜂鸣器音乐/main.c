#include <REGX52.H>

sbit buzzer = P2^5;

#define P0  0
#define L1  1
#define L1_ 2
#define L2  3
#define L2_ 4
#define L3  5
#define L4  6
#define L4_ 7
#define L5  8
#define L5_ 9
#define L6  10
#define L6_ 11
#define L7  12
#define M1  13
#define M1_ 14
#define M2  15
#define M2_ 16
#define M3  17
#define M4  18
#define M4_ 19
#define M5  20
#define M5_ 21
#define M6  22
#define M6_ 23
#define M7  24
#define H1  25
#define H1_ 26
#define H2  27
#define H2_ 28
#define H3  29
#define H4  30
#define H4_ 31
#define H5  32
#define H5_ 33
#define H6  34
#define H6_ 35
#define H7  36

unsigned int freqtable[] = {63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
														64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
														65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283};
unsigned char f=0,m=0;
unsigned char	music[]	= {
	P0,4,
	P0,4,
	P0,4,
	M6,2,
	M7,2,
	
	H1,4+2,
	M7,2,
	H1,4,
	H3,4,
	
	M7,4+4+4,
	M3,2,
	M3,2,
	
	M6,4+2,
	M5,2,
	M6,4,
	H1,4,
	
	M5,4+4+4,
	M3,4,

         };								
														
void Timer0_Init();
void Delay(unsigned int a);

void main()
{
	
	Timer0_Init();
	while(1)
	{
		f=music[m];
		m++;
		Delay(200);
		TR0 = 0;
		Delay(5);
		TR0 = 1;
		if(m>13)
			m=0;
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
	TL0 = freqtable[f]%256;				//设置定时初始值
	TH0 = freqtable[f]/256;				//设置定时初始值
	buzzer = !buzzer;
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