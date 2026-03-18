#include <REGX52.H>

sbit LCD_RS = P2^6;
sbit LCD_RW = P2^5;
sbit LCD_E  = P2^7;

#define LCD_DataPort P0

void LCD_Delay1ms();
void LCD_Showchar(unsigned char line,unsigned char column,unsigned char Char);
void LCD_Init();
void LCD_Writecommand(unsigned char command);
void LCD_Writedata(unsigned char Data);

void main()
{
		LCD_Init();
		LCD_Showchar(2,3,'A');
		while(1)
		{
				
		}
}

void LCD_Showchar(unsigned char line,unsigned char column,unsigned char Char)
{
		if(line == 1)
		{
				LCD_Writecommand(0x80|(column-1));
		}
		else
		{
				LCD_Writecommand(0x80|(column-1)+0x40);
		}
		LCD_Writedata(Char);
}

void LCD_Init()
{
		LCD_Writecommand(0x38);
		LCD_Writecommand(0x0c);
		LCD_Writecommand(0x06);
		LCD_Writecommand(0x01);
}

void LCD_Writecommand(unsigned char command)
{
		LCD_RS = 0;
		LCD_RW = 0;
		LCD_DataPort = command;
		LCD_E = 1;
		LCD_Delay1ms();
		LCD_E = 0;
		LCD_Delay1ms();
}

void LCD_Writedata(unsigned char Data)
{
		LCD_RS = 1;
		LCD_RW = 0;
		LCD_DataPort = Data;
		LCD_E = 1;
		LCD_Delay1ms();
		LCD_E = 0;
		LCD_Delay1ms();
}

void LCD_Delay1ms(void)	
{
	unsigned char data i, j;

	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}