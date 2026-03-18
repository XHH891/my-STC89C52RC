#include <REGX52.H>
#include "LCD1602.H"

/************************* ROM 指令集 *************************/
#define DS18B20_CMD_SEARCH_ROM      0xF0   // 搜索ROM
#define DS18B20_CMD_READ_ROM        0x33   // 读ROM
#define DS18B20_CMD_MATCH_ROM       0x55   // 匹配ROM
#define DS18B20_CMD_SKIP_ROM        0xCC  // 跳过ROM
#define DS18B20_CMD_ALARM_SEARCH    0xEC   // 报警搜索

/************************* 功能指令集 *************************/
#define DS18B20_CMD_CONVERT_T        0x44  // 温度转换
#define DS18B20_CMD_WRITE_SCRATCHPAD 0x4E  // 写暂存器
#define DS18B20_CMD_READ_SCRATCHPAD  0xBE  // 读暂存器
#define DS18B20_CMD_COPY_SCRATCHPAD  0x48  // 复制暂存器到EEPROM
#define DS18B20_CMD_RECALL_E2        0xB8  // 从EEPROM召回数据
#define DS18B20_CMD_READ_POWER_SUPPLY 0xB4 // 读供电模式

#define a_address 0xA0  //at24c04的地址

sbit DQ = P3^7;
sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

/*-----------单总线相关函数---------*/
unsigned char onewire_init();
void one_sendbit(unsigned char Bit);//写1或者0  
unsigned char one_receivebit();//读1或者0
void one_sendbyte(unsigned char Byte);//写整个字节
unsigned char one_receivebyte();//读整个字节
/*===========单总线相关函数===========*/

/*----------DS18B20相关函数----------*/
void ds18b20_convertT();//触发温度转换
float ds18b20_readt();//读取温度
/*==========DS18B20相关函数===========*/

/*----------按键相关函数-------------*/
unsigned char keyo=0;
unsigned char key();
unsigned char Keyr();
void key_loop();
/*===========按键相关函数=============*/

/*-----------I2C相关函数---------------*/
void i2c_start(void);//发送接收的S表示开始
void i2c_stop(void);//发送接收的P表示停止
void i2c_semdbyte(unsigned char byte);//写入一个字节
unsigned char i2c_receivebyte(void);//接收一个字节
void i2c_sendack(unsigned char abit);//发送应答
unsigned char i2c_receiveack();//接收应答
/*===========I2C相关函数===============*/

/*----------at24c04_wb相关函数---------*/
void at24c04_wb(unsigned char wa,Data);//对at24c04写一个字节
unsigned char at24c04_rb(unsigned char wa);//对at24c04读一个字节  wa表示寄存器
/*==========at24c04_wb相关函数=========*/

/*-----------中断相关函数--------------*/
void Timer0_Init();
/*============中断相关函数=============*/

void Delay(unsigned int a); 

void main()
{
	float T;//存储温度
	char tl,th;//保存低温报警和高温报警阈值
	unsigned char knum;//保存按键
	
	th=at24c04_rb(0);
	tl=at24c04_rb(1);
	if(th>125 || tl<-55 || th<=tl)
	{
			th=20;
			tl=15;
	}
	
	LCD_Init();
	Timer0_Init();
	
	LCD_ShowString(1,1,"T:");
	LCD_ShowString(2,1,"TH:");
	LCD_ShowString(2,9,"TL:");
	LCD_ShowSignedNum(2,4,th,3);
	LCD_ShowSignedNum(2,12,tl,3);
	
	while(1)
	{
		knum = key();
		
		ds18b20_convertT();
		T = ds18b20_readt();
		
		if(knum)
		{
				if(knum == 1)
				{
						th++;
						if(th>125){th=125;}
				}
				if(knum == 2)
				{
						th--;
						if(th<=tl){th++;}
				}
				if(knum == 3)
				{
						tl++;
						if(tl>=th){tl--;}
				}
				if(knum == 4)
				{
						tl--;
						if(tl<=-155){tl=-55;}
				}
				LCD_ShowSignedNum(2,4,th,3);
				LCD_ShowSignedNum(2,12,tl,3);
				
				at24c04_wb(0,th);
				Delay(5); 
				at24c04_wb(1,tl);
				Delay(5); 
		}
		if(T>th)
		{
			LCD_ShowString(1,13,"OV:H");
		}
		else if(T<tl)
		{
			LCD_ShowString(1,13,"OV:L");
		}
		else
		{
			LCD_ShowString(1,13,"    ");
		}
		
		if(T<0)
		{
			LCD_ShowChar(1,3,'-');
			T=-T;
		}
		else
			LCD_ShowChar(1,3,'+');
		
		LCD_ShowNum(1,4,T,3);
		LCD_ShowChar(1,7,'.');
		LCD_ShowNum(1,8,(unsigned long)(T*100)%100,2);
		
	}
}

/*----------按键相关函数-------------*/
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
/*===========按键相关函数=============*/

/*----------DS18B20相关函数----------*/
void ds18b20_convertT(void)//触发温度转换
{
		onewire_init();
		one_sendbyte(DS18B20_CMD_SKIP_ROM);
		one_sendbyte(DS18B20_CMD_CONVERT_T);
}

float ds18b20_readt(void)//读取温度
{
		unsigned char tlsb,tmlb;
		int temp;
		float t;
		onewire_init();//单总线初始化：建立通信的前提
		one_sendbyte(DS18B20_CMD_SKIP_ROM);
		one_sendbyte(DS18B20_CMD_READ_SCRATCHPAD);//告诉传感器 “要读温度数据了”
		tlsb = one_receivebyte();// 先读低8位（暂存器第0字节）
		tmlb = one_receivebyte();// 后读高8位（暂存器第1字节）
		temp = (tmlb<<8)|tlsb;//把两个 8 位字节合成完整的温度数据
		t = temp/16.0;//把原始数据转换成可读的浮点温度
		return t;
}


/*============DS18B20相关函数========*/

/*-----------单总线相关函数---------*/
unsigned char onewire_init(void)//初始化
{
		unsigned char i,ackbit;
	
	  EA = 0;//防止被定时器按键扫描打断
	
		DQ = 1;
		DQ = 0;
		i = 227;while (--i);  //delay 500us
		DQ = 1;
		i = 29;while (--i); //delay 70us
		ackbit = DQ;
		i = 227;while (--i);  //delay 500us
	
	  EA = 1;
	
		return ackbit;
}

void one_sendbit(unsigned char Bit)//写1或者0  若写1,则经过10微秒后从0变1;若写0,则DQ = Bit;为0,经过两段时延后写入
{
		unsigned char i;
	
		EA = 0;//防止被定时器按键扫描打断
	
		DQ = 0;
		i = 4;while (--i);  //delay 10us
		DQ = Bit;
		i = 22;while (--i); //delay 50us
		DQ = 1;
		EA = 1;
}

unsigned char one_receivebit(void)//读1或者0
{
		unsigned char i,Bit;
		
		EA = 0;//防止被定时器按键扫描打断
		
		DQ = 0;
		i = 2;while (--i);  //delay 5us
		DQ = 1;
		i = 2;while (--i);  //delay 5us
		Bit = DQ;
		i = 22;while (--i); //delay 50us
	
		EA = 1;
	
		return Bit;
}

void one_sendbyte(unsigned char Byte)//写整个字节
{
		unsigned char i;
	
		for(i=0;i<8;i++)
		{
				one_sendbit(Byte&(0x01<<i));
		}	
}

unsigned char one_receivebyte(void)//读整个字节
{
		unsigned char i,Byte=0x00;
		for(i=0;i<8;i++)
		{
				if(one_receivebit())
				{
					Byte |= (0x01<<i);
				}
		}
		return Byte;
}
/*===========单总线相关函数=============*/

/*-----------I2C相关函数---------------*/
void i2c_start(void)//发送接收的S 表示开始
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
}
void i2c_stop(void)//发送接收的P 表示停止
{
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}
void i2c_semdbyte(unsigned char byte)//写入一个字节
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA = (byte&(0x80>>i));
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}
unsigned char i2c_receivebyte(void)//接收一个字节
{
	unsigned char i,byte=0x00;
	
	I2C_SDA = 1;
	for(i=0;i<8;i++)
	{
		I2C_SCL = 1;
		if(I2C_SDA)
			byte |= (0x80>>i); 
		I2C_SCL = 0;
	}
	return byte;
}
void i2c_sendack(unsigned char abit)//发送应答
{
		I2C_SDA = abit;
		I2C_SCL = 0;
	  I2C_SCL = 1;
}
unsigned char i2c_receiveack()//接收应答
{
		unsigned char abit;
		I2C_SDA = 1;
		I2C_SCL = 1;
		abit = I2C_SDA;
	  I2C_SCL = 0;
		return abit;
}
/*===========I2C相关函数===============*/

/*----------at24c04_wb相关函数---------*/
void at24c04_wb(unsigned char wa,Data)//对at24c04写一个字节  wa表示寄存器
{
	i2c_start();
	i2c_semdbyte(a_address);
	i2c_receiveack();
	i2c_semdbyte(wa);
	i2c_receiveack();
	i2c_semdbyte(Data);
	i2c_receiveack();
	i2c_stop();
}

unsigned char at24c04_rb(unsigned char wa)//对at24c04读一个字节  wa表示寄存器
{
	unsigned char Data;
	
	i2c_start();
	i2c_semdbyte(a_address);
	i2c_receiveack();
	i2c_semdbyte(wa);
	i2c_receiveack();
	
	i2c_start();
	i2c_semdbyte(a_address|0x01);//原本a_address表示写地址,将最后一位变为1,变为读地址
	i2c_receiveack();
	Data = i2c_receivebyte();
	i2c_sendack(1);
	
	i2c_stop();
	return Data;
}
/*==========at24c04_wb相关函数=========*/

/*-----------中断相关函数--------------*/
unsigned int time=0;
void Timer0_Routine(void) interrupt 1
{
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	time++;
	if(time >= 20)
	{
		time = 0;
		key_loop();
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
/*============中断相关函数=============*/

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