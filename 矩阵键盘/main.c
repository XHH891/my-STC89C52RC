#include <REGX51.H>
#include <INTRINS.H>

unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Delay(unsigned int a)	
{
	unsigned char i,j;

	while(a)
	{
		_nop_();
		i = 4;
		j = 239;
		do
		{
			while(--j);
		}while(--i);
		a--;
	}
}

void nixie(unsigned char a,b)
{
	//确定8位数码的显示位置。
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;
	//确定8位数码的显示数字。
	P0 = seg_code[b];
	//在显示多位数字时进行消影操作
	Delay(1);
	P0 = 0x00;
}

void main()
{
	unsigned int a;
	while(1)
	{
		//对矩阵键盘逐列扫描。判断每一列的按钮有没有被按下。
		P1 = 0xff;
		P1_3 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 1;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 5;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 9;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = 13;}
		P1 = 0xff;
		P1_2 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 2;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 6;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 10;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = 14;}
		P1 = 0xff;
		P1_1 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 3;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 7;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 11;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = 15;}
		P1 = 0xff;
		P1_0 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 4;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 8;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 12;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = 16;}
		
		//对按下的数字进行显示从1到16。
		if(a<10)
			nixie(1,a);
		else
		{
			nixie(1,1);
			Delay(2);
			nixie(2,a%10);
			Delay(2);
		}
	}
}