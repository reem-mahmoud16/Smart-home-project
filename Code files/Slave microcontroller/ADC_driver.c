/*
 * ADC_driver.c
 *
 * Created: 3/3/2018 4:59:46 PM
 *  Author: Mohamed Zaghlol
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_macros.h"

void ADC_vinit(void)
{
	SET_BIT(ADMUX,REFS0); //configure VREF
	SET_BIT(ADMUX,REFS1);
	SET_BIT(ADCSRA,ADEN); // enable ADC
	/* adjust ADC clock*/
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
}
 unsigned short ADC_u16Read(void)
{
	 unsigned short read_val;
	
	SET_BIT(ADCSRA,ADSC);
	while(IS_BIT_CLR(ADCSRA,ADIF));//stay in your position till ADIF become 1
	SET_BIT(ADCSRA,ADIF); // clear ADIF
	read_val=(ADCL);
	read_val|=(ADCH<<8);
	return read_val ;
}

//for free running mode ADC

void ADC_vinit_free(void)
{
	SET_BIT(ADCSRA,ADEN); // enable ADC
	SET_BIT(ADMUX,REFS0); //configure VREF 2.56v
	SET_BIT(ADMUX,REFS1);
	//free running mode enable
	SET_BIT(ADCSRA,ADATE);
	/* adjust ADC clock*/
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
	sei();
}

unsigned short ADC_u16Read_free(void)
{
	unsigned short read_val;
	read_val=(ADCL);
	read_val|=(ADCH<<8);
	return read_val ;
}

void ADC_start_free(void)
{
	SET_BIT(ADCSRA,ADIE);
	SET_BIT(ADCSRA,ADSC);
}

void ADC_stop_free(void)
{
	CLR_BIT(ADCSRA,ADIE);
}

void ADC_start_conversion(void)
{
	SET_BIT(ADCSRA,ADIE);
	SET_BIT(ADCSRA,ADSC);
}