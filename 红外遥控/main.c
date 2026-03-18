#include <REGX52.H>
#include "LCD1602.H"

unsigned char Num;

void Int0_Init();//外部中断
void timer0_setcounter(unsigned int value);
unsigned int timer0_getcounter(void);
void timer0_run(unsigned char flag);

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

void main()
{
		LCD_Init();
		LCD_ShowString(1,1,"A");
	
		Int0_Init();
		timer0_setcounter(0);
		timer0_run(1);
		Delay(10);
		Num = timer0_getcounter();
		LCD_ShowNum(2,1,Num,3);
		while(1)
		{
				
		}
		
}

void Int0_Init()//外部中断
{
		TMOD &= 0xf0;
		TMOD |= 0x01;
		TL0 = 0;
		TH0 = 0;
		TF0 = 0;
		TR0 = 0;
		//IT0 = 1;
		//IE0 = 0;
		//EX0 = 1;
		//EA = 1;
		//PX0 = 1;
}

void timer0_setcounter(unsigned int value)
{
		TH0 = value/256;
		TL0 = value%256;
}

unsigned int timer0_getcounter(void)
{
		return (TH0<<8)|TL0;
}

void timer0_run(unsigned char flag)
{
		TR0 = flag;
}

void Int0_Routine(void) interrupt 0
{
		Num++;
}