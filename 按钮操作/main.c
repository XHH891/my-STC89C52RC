#include <REGX52.H>
#include <INTRINS.H>

void Delay(unsigned int a)	//Ê±ÑÓº¯Êý
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
	
	unsigned char LEDNUM = 0;
	
	while(1)
	{
		
			if(P3_1 ==0)
			{
				Delay(5);//·ÀÖ¹¶¶¶¯
				while(P3_1 ==0);//ÅÐ¶ÏÊÇ·ñËÉÊÖ
				Delay(5);//·ÀÖ¹¶¶¶¯
				
				if(LEDNUM >= 8)
					LEDNUM = 0;
				P2 = ~(0x01 << LEDNUM);
				LEDNUM++;
			}
			
			if(P3_0 == 0)
			{
				Delay(5);//·ÀÖ¹¶¶¶¯
				while(P3_0 ==0);//ÅÐ¶ÏÊÇ·ñËÉÊÖ
				Delay(5);//·ÀÖ¹¶¶¶¯
				
				if(LEDNUM == 0)
					LEDNUM = 7;
				else
					LEDNUM--;
				P2 = ~(0x01 << LEDNUM);
			}
			
		
	}
}