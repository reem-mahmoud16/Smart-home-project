/*
 * DIO_module.c
 *
 * Created: 7/23/2022 5:17:17 PM
 *  Author: HP.SXH08
 */ 


#include <avr/io.h>
#include "std_MACROS.h"

void DIO_setDIR(char port, char bin, char dir)
{
	switch(port)
	{
		case 'A':
		case 'a': 
					if(dir == 1)
					{
						SET_BIT(DDRA,bin);
					}
					else
					{
						CLR_BIT(DDRA,bin);
					}
					break;
					
		case 'B':
		case 'b':
					if(dir == 1)
					{
						SET_BIT(DDRB,bin);
					}
					else
					{
						CLR_BIT(DDRB,bin);
					}
					break;
		
		case 'C':
		case 'c':
					if(dir == 1)
					{
						SET_BIT(DDRC,bin);
					}
					else
					{
						CLR_BIT(DDRC,bin);
					}
					break;
		
		case 'D':
		case 'd':
					if(dir == 1)
					{
						SET_BIT(DDRD,bin);
					}
					else
					{
						CLR_BIT(DDRD,bin);
					}
					break;    
	}
}
	
	void DIO_write(char port, char bin, char val)
	{
		switch(port)
		{
			case 'A':
			case 'a':
						if(val == 1)
						{
							SET_BIT(PORTA,bin);
						}
						else
						{
							CLR_BIT(PORTA,bin);
						}
						break;
						
			case 'B':
			case 'b':
						if(val == 1)
						{
							SET_BIT(PORTB,bin);
						}
						else
						{
							CLR_BIT(PORTB,bin);
						}
						break;
			
			case 'C':
			case 'c':
						if(val == 1)
						{
							SET_BIT(PORTC,bin);
						}
						else
						{
							CLR_BIT(PORTC,bin);
						}
						break;
			
			case 'D':
			case 'd':
						if(val == 1)
						{
							SET_BIT(PORTD,bin);
						}
						else
						{
							CLR_BIT(PORTD,bin);
						}
						break;
	}
}

	char DIO_read(char port, char bin)
	{
		char val;
		switch(port)
		{
			case 'A':
			case 'a':
						val = READ_BIT(PINA,bin);
						break;
						
			case 'B':
			case 'b':
						val = READ_BIT(PINB,bin);
						break;
			
			case 'C':
			case 'c':
						val = READ_BIT(PINC,bin);
						break;
			
			case 'D':
			case 'd':
						val = READ_BIT(PIND,bin);
						break;
		}			
		return val;
	}
	
	void DIO_TogBin(char port, char bin)
	{
		switch(port)
		{
			case 'A':
			case 'a':
						TOG_BIT(PORTA,bin);
						break;
			
			case 'B':
			case 'b':
						TOG_BIT(PORTB,bin);
						break;
			
			case 'C':
			case 'c':
						TOG_BIT(PORTC,bin);
						break;
			
			case 'D':
			case 'd':
						TOG_BIT(PORTD,bin);
						break;
		}
	}	
	
	
	void DIO_setPortDIR(char port, char dir)
	{
		switch(port)
		{
			case 'A':
			case 'a':
						DDRA=dir;
						break;
			
			case 'B':
			case 'b':
						DDRB=dir;
						break;
			
			case 'C':
			case 'c':
						DDRC=dir;
						break;
			
			case 'D':
			case 'd':
						DDRD=dir;
						break;
		}
	 }
	 void DIO_PortWrite(char port, char val)
	 {
		 switch(port)
		 {
			 case 'A':
			 case 'a':
						 PORTA=val;
						 break;
			 
			 case 'B':
			 case 'b':
						 PORTB=val;
						 break;
			 
			 case 'C':
			 case 'c':
						 PORTC=val;
						 break;
			 
			 case 'D':
			 case 'd':
						 PORTD=val;
						 break;
		 }
	 }
	 
	 void DIO_TogPort(char port)
	 {
		 switch(port)
		 {
			 case 'A':
			 case 'a':
						 PORTA = ~PORTA;
						 break;
			 
			 case 'B':
			 case 'b':
						 PORTB = ~PORTB;
						 break;
			 
			 case 'C':
			 case 'c':
						 PORTC = ~PORTC;
						 break;
			 
			 case 'D':
			 case 'd':
						 PORTD = ~PORTD;
						 break;
		 }
	 }	
	 
	 char DIO_PortRead(char port)
	 {
		 char val;
		 switch(port)
		 {
			 case 'A':
			 case 'a':
						 val = PORTA;
						 break;
			 
			 case 'B':
			 case 'b':
						 val = PORTB;
						 break;
			 
			 case 'C':
			 case 'c':
						 val = PORTC;
						 break;
			 
			 case 'D':
			 case 'd':
						 val = PORTD;
						 break;
		 }
		 return val;
	 }

