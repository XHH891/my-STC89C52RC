//单片机向电脑发送数据
#include <REGX52.H>

void Delay(unsigned int a);
void nixie(unsigned char a,b);
void UART_Init();
void UART_SendByte(unsigned char Byte);

unsigned int a;

void main()
{	
	
	UART_Init();

	while(1)
	{
		UART_SendByte(a);
		Delay(1000);
		a++;
		if(a>10)
			a=0;
	}
}

void UART_SendByte(unsigned int Byte)//发送数据
{
	SBUF=Byte;
	while(TI==0);//如果数据比较多,会发送一段时间,期间TI=0,当发送完成是变为1
	TI=0;//重置TI,为下一次做准备
}

void UART_Init()//配置寄存器
{
	SCON = 0x40;
	PCON &= 0x7F;
	TMOD &= 0x0F;		 	//设置定时器模式
	TMOD |= 0x20;			//设置定时器模式
	TL1 = 0xFA;			//设置定时初始值
	TH1 = 0xFA;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
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