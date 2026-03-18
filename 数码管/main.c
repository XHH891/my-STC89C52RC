#include <REGX51.H>
#include <INTRINS.H>



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
	unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;
	
	P0 = seg_code[b];
	Delay(1);
	P0 = 0x00;
}

void main()
{
	unsigned char e=1;
	while(1)
	{
		
		nixie(1,1);
		Delay(2);
		nixie(2,2);
		Delay(2);
		nixie(3,3);
		Delay(2);
	}
}