#include <REGX52.H>

void Timer0_Init();

/*按键相关函数*/
unsigned char key();
unsigned char Keyr();
void key_loop();
unsigned char keyo=0;

/*数码管显示相关函数*/
void nixie(unsigned char a,b);
void n_loop(void);
void nixie_setbuf(unsigned char a,b);
unsigned char n_buf[9]={0,10,10,10,10,10,10,10,10};

/*时间相关函数*/
void sec_loop();
unsigned char min=0,sec=0,minisec=0;
unsigned char f=0;

void main()
{
	unsigned char keynum;
	 
		
	Timer0_Init();
	while(1)
	{
		keynum = key();
		if(keynum == 1)
		{
			f = !f;
		}
		if(keynum == 2)
		{
			min=0;
			sec=0;
			minisec=0;
		}
		nixie_setbuf(1,min/10);
		nixie_setbuf(2,min%10);
		nixie_setbuf(3,11);
		nixie_setbuf(4,sec/10);
		nixie_setbuf(5,sec%10);
		nixie_setbuf(6,11);
		nixie_setbuf(7,minisec/10);
		nixie_setbuf(8,minisec%10);
	}
}

/*时间相关----------------------*/
void sec_loop()
{
	if(f)
	{
		minisec++;
		if(minisec >= 100)
		{
			minisec = 0;
			sec++;
			if(sec>60)
			{
				sec = 0;
				min++;
				if(min>60)
				{
					min = 0;
				}
			}
		}
	}

}
/*-------------------------------*/

/*数码管相关----------------------*/
void nixie_setbuf(unsigned char a,b)
{
	n_buf[a] = b;
}

void nixie(unsigned char a,b)
{
	unsigned char seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x00,0x40};
	P0=0x00;
	
	a = 8-a;
	P2_2 = a % 2;
	a = a / 2;
	P2_3 = a % 2;
	a = a / 2;
	P2_4 = a % 2;
	a = a / 2;

	P0 = seg_code[b];
}

void n_loop(void)
{
	static unsigned char i;
	nixie(i,n_buf[i]);
	i++;
	if(i>=9)
		i=1;
}
/*-------------------------------*/

/*按键相关----------------------*/
unsigned char key()
{
	unsigned char temp=0;
	temp = keyo;
	keyo = 0;
	return temp;
}

unsigned char Keyr()
{
	unsigned char KeyNumber=0;
	if(P3_1==0) {KeyNumber=1;}
	if(P3_0==0) {KeyNumber=2;}
	if(P3_2==0) {KeyNumber=3;}
	if(P3_3==0) {KeyNumber=4;}
	return KeyNumber;
}

void key_loop(void)
{
	static unsigned char nowstate,laststate;
	
	laststate = nowstate;
	nowstate = Keyr();
	if(laststate==1 && nowstate==0)
	{
		keyo = 1;
	}
	if(laststate==2 && nowstate==0)
	{
		keyo = 2;
	}
	if(laststate==3 && nowstate==0)
	{
		keyo = 3;
	}
	if(laststate==4 && nowstate==0)
	{
		keyo = 4;
	}
}
/*-------------------------------*/

unsigned int Tk=0,Tn=0,T=0;
void Timer0_Routine(void) interrupt 1
{
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	Tk++;
	Tn++;
	T++;
	if(Tk >= 20)
	{
		Tk = 0;
		key_loop();
	}	
	if(Tn >= 2)
	{
		Tn = 0;
		n_loop();
	}
	if(T >= 10)
	{
		T = 0;
		sec_loop();
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