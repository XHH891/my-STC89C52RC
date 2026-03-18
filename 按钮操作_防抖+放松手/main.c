#include <REGX52.H>
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

void main()
{
	//P2 = 0xfe;
	P2_0 = 1;
	while(1)
	{
		if(P3_1 == 0)
		{
			
			Delay(5);
			while(P3_1 ==0);
			Delay(5);
			
			P2_0 = ~P2_0;
		}
	}
}