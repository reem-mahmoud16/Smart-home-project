/*
 * ADC_driver.h
 *
 * Created: 3/3/2018 5:00:21 PM
 *  Author: Mohamed Zaghlol
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_
void ADC_vinit(void);
unsigned short ADC_u16Read(void);
void ADC_vinit_free(void);
unsigned short ADC_u16Read_free(void);
void ADC_start_free(void);
void ADC_stop_free(void);
void ADC_start_conversion(void);


#endif /* ADC_DRIVER_H_ */