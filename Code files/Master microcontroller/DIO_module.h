/*
 * DIO_module.h
 *
 * Created: 7/23/2022 5:18:07 PM
 *  Author: HP.SXH08
 */ 


#ifndef DIO_MODULE_H_
#define DIO_MODULE_H_

void DIO_setDIR(char port, char bin, char dir);
void DIO_write(char port, char bin, char val);
char DIO_read(char port, char bin);
void DIO_TogBin(char port, char bin);
void DIO_setPortDIR(char port, char dir);
void DIO_PortWrite(char port, char val);
void DIO_TogPort(char port);
char DIO_PortRead(char port);

#endif /* DIO_MODULE_H_ */