/*
 * smart_home_slave.c
 *
 * Created: 2/15/2023 4:40:14 PM
 *  Author: HP.SXH08
 */ 


#include <avr/io.h>
#include "LED.h"
#include "SPI_driver.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include "ADC_driver.h"
#include <avr/interrupt.h>

volatile unsigned char currentTemp=0;
volatile unsigned char chosenTemp = 25;

int main(void)
{
	ADC_vinit_free();
	LED_vInit('D',0);
	LED_vInit('D',1);
	LED_vInit('D',2);
	LED_vInit('D',3);
	LED_vInit('D',4);
	LED_vInit('D',5);
	SPI_SlaveInit();
	
	//sei();
	unsigned char choice;
	unsigned char room;
	unsigned char condChoice;
	
    while(1)
    {
		room = SPI_SlaveReceivechar(0x00);
		_delay_ms(500);
		if(room!=6)
		{
			choice = SPI_SlaveReceivechar(0x00);
			_delay_ms(500);
			if(choice==1)
			{
				LED_vTurnOn('D',room-1);
			}
			else if(choice==2)
			{
				LED_vTurnOff('D',room-1);
			}
		}
		else
		{
			condChoice = SPI_SlaveReceivechar(0x00);
			_delay_ms(500);
			if(condChoice ==1)
			{
				chosenTemp = SPI_SlaveReceivechar(0x00);
				_delay_ms(500);
			}
			else if(condChoice == 2)
			{
				choice = SPI_SlaveReceivechar(0x00);
				_delay_ms(500);
				if(choice==1)
				{
					ADC_start_free();
				}
				else if(choice==2)
				{
					ADC_stop_free();
					LED_vTurnOff('D',5);
				}
			}
		}
		
	}   
}


ISR(ADC_vect)
{
	currentTemp = 0.25*ADC_u16Read_free();
	if(currentTemp <= chosenTemp)
	{
		LED_vTurnOff('D',5);
	}
	else
	{
		LED_vTurnOn('D',5);
	}
	ADC_start_conversion();
}