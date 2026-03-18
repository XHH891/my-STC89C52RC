#include <REGX51.H>
#include <INTRINS.H>

unsigned char seg_code[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};
//                            0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F    空
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
	unsigned int b[4];
	unsigned int a;
	unsigned int f;
	unsigned int i = 1;
	b[0]=16;
	b[1]=16;
	b[2]=16;
	b[3]=16;
	nixie(1,b[0]);
			Delay(2);
			nixie(2,b[1]);
			Delay(2);
			nixie(3,b[2]);
			Delay(2);
			nixie(4,b[3]);
			Delay(2);
	while(1)
	{
		//对矩阵键盘逐列扫描。判断每一列的按钮有没有被按下。
		P1 = 0xff;
		P1_3 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 7;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 4;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 1;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = 0;}
		P1 = 0xff;
		P1_2 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 8;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 5;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 2;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = -1;}
		P1 = 0xff;
		P1_1 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 9;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 6;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 3;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = -1;}
		P1 = 0xff;
		P1_0 = 0;
		if(P1_7 == 0){Delay(20);while(P1_7 ==0);Delay(20);a = 11;}
		if(P1_6 == 0){Delay(20);while(P1_6 ==0);Delay(20);a = 12;}
		if(P1_5 == 0){Delay(20);while(P1_5 ==0);Delay(20);a = 13;}
		if(P1_4 == 0){Delay(20);while(P1_4 ==0);Delay(20);a = -1;}
		
		if(a<10 && a>=0)
		{
			b[i-1] = a;
			if(i != 4)
				i+=1;
		}
		else if(a == 12)
		{
			b[i-1] = 16;
			if(i != 1)
				i-=1;
		}
		else if(a == 13)
		{
			b[0]=16;
			b[1]=16;
			b[2]=16;
			b[3]=16;
			i = 1;
		}
		else if (a==11)
		{
			f = b[0]*1000+b[1]*100+b[2]*10+b[3];
			if(f == 1234)
				break;
		}
		nixie(1,b[0]);
		Delay(1);
		nixie(2,b[1]);
		Delay(1);
		nixie(3,b[2]);
		Delay(1);
		nixie(4,b[3]);
		Delay(1);
		a=-1;
	}
	while(1)
	{
		nixie(1,10);
		Delay(1);
		nixie(2,11);
		Delay(1);
		nixie(3,12);
		Delay(1);
		nixie(4,13);
		Delay(1);
		nixie(5,14);
		Delay(1);
		nixie(6,15);
		Delay(1);
	}
		
}