/*
 * Question6.c
 *
 *  Created on: Apr 15, 2022
 *      Author: dell
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "Types_Macros.h"
#include "Keypad.H"
#include "LCD.h"
#include "Servo.h"
#include "Lm35_Sensor.h"
#include "LDR.h"

int main()
{
	SET_BIT(SREG,7);

	CLEAR_BIT(DDRA,2);
	DDRB = 0xff;
	PORTB = 0;

	DDRD = 0x0E;
	PORTD = 0;

	SET_BIT(DDRD,0);
	CLEAR_BIT(PORTD,0);

	SET_BIT(DDRD,6);
	CLEAR_BIT(PORTD,6);

	Configuration lcd = {1 ,2 ,3 , PortD , PortB , _8bits ,_2Line,_5_10};
	LCD_Init(&lcd);

	DDRC = 0xf0;

	servo_init();

	ADC_init();

	uint8 in[16] = "Enter password:";
	uint8 con[11] = "Confirmed.";
	uint8 wor[16] = "Wrong password.";
	uint8 stl[16] = " Attempts left.";
	uint8 fir[11] = "Enter new ";
	uint8 sec[16] = "Please, Confirm";
	uint8 it[4] = "it.";
	uint8 is[4] = "is ";
	uint8 thir[11] = "Unmatched.";
	uint8 th[16] = "Theft attempted.";
	uint8 tem[16] = "The temperature";
	uint8 Units[4] = "'C";
	uint8 wel[11] = "password:";

	uint8 num1[6] ;
	uint8 num2[6] ;

	Timer_ConfigType timer2 = {TIMER2 , NORMAL , F_CPU_1024 , 0 , 0 };
	Timer_ConfigType timer0 = {TIMER0 , NORMAL , F_CPU_1024 , 0 , 0 };

	SET_BIT(DDRA,7);
	SET_BIT(DDRA,6);
	SET_BIT(DDRA,5);

	uint8 i = 0;
	uint8 flag1 = 0;
	uint8 flag2 = 0;
	uint8 trick = 0;
	uint8 temp = 0;
	uint8 r = 0;

	while(1)
	{
		check();
		while(1)
		{
			check();
			LCD_SendCommand(DISPLAY_CLEAR);
			_delay_ms(20);
			LCD_PrintString(fir);
			_delay_ms(20);
			LCD_SendCommand(NEXT_LINE);
			_delay_ms(20);
			LCD_PrintString(wel);
			_delay_ms(500);
			for(i=0 ; i<5 ; i++)
			{
				check();
				num1[i] = get_Value();
				if(i==0)
					LCD_SendCommand(DISPLAY_CLEAR);
				else
					LCD_SendCommand(CURSOR_SHIFT_RIGHT);
				_delay_ms(20);
				LCD_PrintCharacter(num1[i] + 48);
				_delay_ms(500);
			}
			LCD_SendCommand(DISPLAY_CLEAR);
			_delay_ms(20);
			LCD_PrintString(sec);
			_delay_ms(20);
			LCD_SendCommand(NEXT_LINE);
			_delay_ms(20);
			LCD_PrintString(it);
			_delay_ms(500);
			for(i=0 ; i<5 ; i++)
			{
				check();
				num2[i] = get_Value();
				if(i==0)
					LCD_SendCommand(DISPLAY_CLEAR);
				else
					LCD_SendCommand(CURSOR_SHIFT_RIGHT);
				_delay_ms(20);
				LCD_PrintCharacter(num2[i] + 48);
				_delay_ms(500);
			}

			for(i=0 ; i<5 ; i++)
			{
				if(num1[i] != num2[i])
				{
					check();
					LCD_SendCommand(DISPLAY_CLEAR);
					_delay_ms(20);
					LCD_PrintString(thir);
					_delay_ms(500);
					flag1 = 0;
					break;
				}
				else
					flag1 = 1;
			}

			if(flag1 == 1)
				break;
		}
		LCD_SendCommand(DISPLAY_CLEAR);
		_delay_ms(20);
		LCD_PrintString(con);
		_delay_ms(500);
		while(1)
		{
			check();
			LCD_SendCommand(DISPLAY_CLEAR);
			_delay_ms(20);
			LCD_PrintString(in);
			_delay_ms(500);
			for(r=0 ; r<3 ;r++)
			{
				check();
				for(i=0 ; i<5 ; i++)
				{
					check();
					num2[i] = get_Value();
					if(i==0)
						LCD_SendCommand(DISPLAY_CLEAR);
					else
						LCD_SendCommand(CURSOR_SHIFT_RIGHT);
					_delay_ms(20);
					LCD_PrintCharacter(num2[i] + 48);
					_delay_ms(500);
				}
				for(i=0 ; i<5 ; i++)
				{
					if(num1[i] != num2[i])
					{
						LCD_SendCommand(DISPLAY_CLEAR);
						_delay_ms(20);
						LCD_PrintString(wor);
						_delay_ms(500);
						LCD_SendCommand(DISPLAY_CLEAR);
						_delay_ms(20);
						LCD_PrintCharacter((2-r) + 48 );
						LCD_SendCommand(CURSOR_SHIFT_RIGHT);
						LCD_PrintString(stl);
						_delay_ms(500);
						flag2 = 0;
						break;
					}
					else if(i == 4)
					{
						flag2 = 1;
						break;
					}
				}
				if(flag2 == 1)
				{break;}
				else if(flag2 == 0 && r==2)
				{break;}
			}
			if(flag2 == 0)
			{
				check();
				Timer_init(&timer0);
				LCD_SendCommand(DISPLAY_CLEAR);
				_delay_ms(20);
				LCD_PrintString(th);
				while(trick<23)
				{
					_delay_ms(1000);
					trick++;
				}
				trick = 0;
			}
			else if(flag2 == 1)
			{
				LDR_check();
				temp = LM35_readtemp();
				Timer_init(&timer2);
				LCD_SendCommand(DISPLAY_CLEAR);
				_delay_ms(20);
				LCD_PrintString(tem);
				_delay_ms(20);
				LCD_SendCommand(NEXT_LINE);
				_delay_ms(20);
				LCD_PrintString(is);
				_delay_ms(20);
				LCD_intgerToString(temp);
				_delay_ms(20);
				LCD_PrintString(Units);
				while(trick<5)
				{
					_delay_ms(1000);
					trick++;
				}
				trick = 0;
			}
		}
	}
}
