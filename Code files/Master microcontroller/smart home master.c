/*
 * smart_home_master.c
 *
 * Created: 2/15/2023 2:43:15 PM
 *  Author: HP.SXH08
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "LCD.h"
#include "LCD_config.h"
#include "keypad_driver.h"
#include "LED.h"
#include "DIO.h"
#include "SPI_driver.h"
#include "EEPROM_driver.h"
#include "timer.h"

#define PASSTRIALS 3
#define ADMENPASSADD 0x0010
#define GUESTPASSADD 0x0020
#define PASS_SIZE 4
#define BLOCKTIME 30
#define BLOCKSTATEADD 0x50

unsigned char timeCounter=0;
int main(void)
{
	//initialization 
	LCD_vInit();
	keypad_vInit();
	LED_vInit('c',0);
	LED_vInit('c',1);
	LED_vInit('c',2);
	SPI_MasterInit();
	
	unsigned char state1=0;
	unsigned char state2=0;
	unsigned char state3=0;
	unsigned char state4=0;
	unsigned char state5=0;
	unsigned char state6=0;
	
	//welcome massage
	LCD_movecursor(1,3);
	LCD_vSend_string("Welcome to");
	LCD_movecursor(2,3);
	LCD_vSend_string("smart home");
	_delay_ms(2000);
	
	//options page
	while(1)
	{
		//timer_CTC_init_interrupt();
		if(EEPROM_read(BLOCKSTATEADD) != 0xff) {goto BLOCKSTATE;}
HOMEPAGE: 
		LED_vTurnOff('c',0);
		LED_vTurnOff('c',1);
		LED_vTurnOff('c',2);
		LCD_clearscreen();
		LCD_vSend_string("Enter a choice:");
		LCD_movecursor(2,1);
		LCD_vSend_string("1:Admen 2:Guest");
		_delay_ms(250);
		//get guest or admen option, 1 for admen, 2 for guest
		unsigned char op0 = NOTPRESSED;
		do 
		{
			op0= keypad_u8check_press();
		} while (op0 == NOTPRESSED);
		_delay_ms(500);
		
		if(op0 == '1')
		{
			
			//admin mode
			//create pass for first time
			if(EEPROM_read(ADMENPASSADD) == 0xff)
			{
				LCD_clearscreen();
				LCD_vSend_string("Set Admen pass:");
				LCD_movecursor(2,1);
				unsigned char pass;
				for(char i=0; i<PASS_SIZE; i++)
				{
					do
					{
						pass= keypad_u8check_press();
					} while (pass == NOTPRESSED);
					EEPROM_write(ADMENPASSADD+i,pass);
					LCD_vSend_char(pass);
					_delay_ms(500);
					LCD_movecursor(2,1+i);
					LCD_vSend_char(42);
				}
				_delay_ms(500);
				LCD_clearscreen();
				LCD_vSend_string("Pass saved!");
				goto HOMEPAGE;
			}
			//check pass
			char trials = PASSTRIALS;
TYPEPASS:	LCD_clearscreen();
			LCD_vSend_string("type Admen pass:");
			LCD_movecursor(2,1);
			unsigned char writtenPass[PASS_SIZE];
			unsigned char pass;
			for(char i=0; i<PASS_SIZE; i++)
			{
				do
				{
					pass= keypad_u8check_press();
				} while (pass == NOTPRESSED);
				writtenPass[i]=pass;
				LCD_vSend_char(pass);
				_delay_ms(500);
				LCD_movecursor(2,1+i);
				LCD_vSend_char(42);
			}
			_delay_ms(500);
			unsigned char i;
			for(i=0; i<PASS_SIZE; i++)
			{
				if(writtenPass[i] != EEPROM_read(ADMENPASSADD+i)) {break;}
			}
			if(i==PASS_SIZE)
			{
				LCD_clearscreen();
				LCD_vSend_string("Correct Pass!");
				_delay_ms(1000);
			}
			else if(i<PASS_SIZE)
			{
				trials--;
				if(trials>0)
				{
					LCD_clearscreen();
					LCD_vSend_string("wrong Pass!");
					LCD_movecursor(2,1);
					LCD_vSend_string("try again");
					_delay_ms(1000);
					LCD_clearscreen();
					LCD_vSend_string("remained trials:");
					LCD_movecursor(2,1);
					LCD_vSend_char(trials+48);
					_delay_ms(1000);
					goto TYPEPASS;
				}
				else if(trials==0)
				{
BLOCKSTATE:			LCD_clearscreen();
					LCD_vSend_string("You are blocked");
					LCD_movecursor(2,1);
					LCD_vSend_string("for: ");
					LED_vTurnOn('c',2);
					EEPROM_write(BLOCKSTATEADD,0x00);
					signed char counter;
					for (counter = BLOCKTIME; counter>=0; counter--)
					{
						LCD_movecursor(2,6);
						LCD_vSend_char((counter/10)+48);
						LCD_vSend_char((counter%10)+48);
						_delay_ms(1000);
					}
					LED_vTurnOff('c',2);
					EEPROM_write(BLOCKSTATEADD,0xff);
				}
				goto HOMEPAGE;
			}	
						
			LCD_clearscreen();
			LCD_vSend_string("Admen mode on!");
			LED_vTurnOn('c',0);
			_delay_ms(1000);
ROOM_PAGE1: LCD_clearscreen();
			LCD_vSend_string("1:ROOM1 2:ROOM2");
			LCD_movecursor(2,1);
			LCD_vSend_string("3:ROOM3 4:more");
			
			_delay_ms(500);
			unsigned char room= NOTPRESSED;
			do
			{
				room= keypad_u8check_press();
			} while (room == NOTPRESSED);
			room -=48;
			_delay_ms(500);
			if(room!=1 && room!=2 && room!=3 && room!=4)
			{
				LCD_clearscreen();
				LCD_movecursor(1,3);
				LCD_vSend_string("Wrong choice");
				goto ROOM_PAGE1;
			}
			if(room==4)
			{
				LCD_clearscreen();
				LCD_vSend_string("4:Room4 5:TV");
				LCD_movecursor(2,1);
				LCD_vSend_string("6:Air cond 7:ret");
				room= NOTPRESSED;
				do
				{
					_delay_ms(250);
					room= keypad_u8check_press();
				} while (room == NOTPRESSED);
				room -=48;
				if(room!=7) 
				{ 
					SPI_MasterTransmitchar(room);
					_delay_ms(500);
				}				
				if(room!=4 && room!=5 && room!=6 && room!=7)
				{
					LCD_clearscreen();
					LCD_movecursor(1,3);
					LCD_vSend_string("Wrong choice");
					goto ROOM_PAGE1;
				}
				
				if(room==4)
				{
					LCD_clearscreen();
					LCD_vSend_string("Room4: ");
					if(state4==0){LCD_vSend_string("Off");}
					else if(state4==1){LCD_vSend_string("On");}
					LCD_movecursor(2,1);
					LCD_vSend_string("1:on 2:off 3:ret");
					
				}
				else if(room==5)
				{
					LCD_clearscreen();
					LCD_vSend_string("TV state: ");
					if(state5==0){LCD_vSend_string("Off");}
					else if(state5==1){LCD_vSend_string("On");}
					LCD_movecursor(2,1);
					LCD_vSend_string("1:on 2:off 3:ret");
				}
				else if(room==6)
				{
					LCD_clearscreen();
					LCD_vSend_string("1:Adjust temp.");
					LCD_movecursor(2,1);
					LCD_vSend_string("2:control 3:ret");
					unsigned char condChoice= NOTPRESSED;
					do
					{
						condChoice= keypad_u8check_press();
					} while(condChoice == NOTPRESSED);
					condChoice= condChoice-48;
					_delay_ms(500);
					
					SPI_MasterTransmitchar(condChoice);
					_delay_ms(500);
					
					if(condChoice ==3)
					{
						goto ROOM_PAGE1;
					}
					else if(condChoice == 2)
					{
						LCD_clearscreen();
						LCD_vSend_string("Air cond: ");
						if(state6==0){LCD_vSend_string("Off");}
						else if(state6==1){LCD_vSend_string("On");}
						LCD_movecursor(2,1);
						LCD_vSend_string("1:on 2:off 3:ret");
					}
					else if(condChoice == 1)
					{
						LCD_clearscreen();
						LCD_vSend_string("Enter Temp: ");
						unsigned char chosenTemp=0,tempdigit,divident=10;
						for(char i=0; i<2; i++)
						{
							do
							{
								tempdigit= keypad_u8check_press();
							} while (tempdigit == NOTPRESSED);
							LCD_vSend_char(tempdigit);
							tempdigit -=48;
							chosenTemp += tempdigit*divident; 
							divident /=10;
							_delay_ms(500);
						}
						SPI_MasterTransmitchar(chosenTemp);
						_delay_ms(1000);
						goto ROOM_PAGE1;
						
					}
					
				}
				else if(room==7)
				{
					goto ROOM_PAGE1;
				}
				
				unsigned char choice= NOTPRESSED;
				do
				{
					choice= keypad_u8check_press();
				} while(choice == NOTPRESSED);
				choice= choice-48;
				_delay_ms(500);
				
				SPI_MasterTransmitchar(choice);
				_delay_ms(500);
				
				if(choice ==3)
				{
					goto ROOM_PAGE1;
				}
				else if(choice!=1 && choice!=2 &&choice!=3)
				{
					LCD_clearscreen();
					LCD_movecursor(1,3);
					LCD_vSend_string("Wrong Choice!");
					_delay_ms(1000);
					goto ROOM_PAGE1;
				}
				if(room==1){state1 = (choice& 0x01);}
				else if(room==4){state4 = (choice& 0x01);}
				else if(room==5){state5 = (choice& 0x01);}
				else if(room==6){state6 = (choice& 0x01);}
				goto ROOM_PAGE1;
			}
			else
			{
				SPI_MasterTransmitchar(room);
				_delay_ms(500);
				LCD_clearscreen();
				LCD_vSend_string("Room");
				LCD_vSend_char(room+48);
				LCD_vSend_string("state: ");
			
				if(room==1 && state1==0)
				{
					LCD_vSend_string("Off");
				}
				else if(room==1 && state1==1)
				{
					LCD_vSend_string("On ");
				}
			
				else if(room==2 && state2==0)
				{
					LCD_vSend_string("Off");
				}
				else if(room==2 && state2==1)
				{
					LCD_vSend_string("On ");
				}
			
				else if(room==3 && state3==0)
				{
					LCD_vSend_string("Off");
				}
				else if(room==3 && state3==1)
				{
					LCD_vSend_string("On ");
				}
			
			
				LCD_movecursor(2,1);
				LCD_vSend_string("1:on 2:off 3:ret");
			
				unsigned char choice= NOTPRESSED;
				do
				{
					choice= keypad_u8check_press();
				} while(choice == NOTPRESSED);
			
				choice= choice-48;
				
				SPI_MasterTransmitchar(choice);
				_delay_ms(500);
				
				if(choice ==3)
				{
					goto ROOM_PAGE1;
				}
				else if(choice!=1 && choice!=2 &&choice!=3)
				{
					LCD_clearscreen();
					LCD_movecursor(1,3);
					LCD_vSend_string("Wrong Choice!");
					_delay_ms(1000);
					goto ROOM_PAGE1;
				}
				if(room==1){state1 = (choice& 0x01);}
				else if(room==2){state2 = (choice& 0x01);}
				else if(room==3){state3 = (choice& 0x01);}
				else if(room==4){state4 = (choice& 0x01);}
				
				goto ROOM_PAGE1;
			}			
		}	
		else if(op0 == '2')
		{
			//guest mode
			LCD_clearscreen();
			LCD_vSend_string("Guest mode on!");
			LED_vTurnOn('c',1);
			_delay_ms(1000);
			LCD_clearscreen();
ROOM_PAGE2:	LCD_vSend_string("1:ROOM1 2:ROOM2");
			LCD_movecursor(2,1);
			LCD_vSend_string("3:ROOM3 4:ROOM4");
			
			unsigned char room= NOTPRESSED;
			do
			{
				room= keypad_u8check_press();
			} while (room == NOTPRESSED);
			room -=48;
			
			SPI_MasterTransmitchar(room);
			_delay_ms(500);
			
			if(room!=1 && room!=2 && room!=3 && room!=4)
			{
				LCD_clearscreen();
				LCD_movecursor(1,3);
				LCD_vSend_string("Wrong choice");
				goto ROOM_PAGE2;
			}
			LCD_clearscreen();
			LCD_vSend_string("Room");
			LCD_vSend_char(room+48);
			LCD_vSend_string("state: ");
			
			if(room==1 && state1==0)
			{ 	
				LCD_vSend_string("Off");
			}
			else if(room==1 && state1==1)
			{
					LCD_vSend_string("On ");
			}
				
			else if(room==2 && state2==0)
			{
					LCD_vSend_string("Off");
			}
			else if(room==2 && state2==1)
			{
					LCD_vSend_string("On ");
			}
				
			else if(room==3 && state3==0)
			{
					LCD_vSend_string("Off");
			}
			else if(room==3 && state3==1)
			{
					LCD_vSend_string("On ");
			}
				
			else if(room==4 && state4==0)
			{
					LCD_vSend_string("Off");
			}
			else if(room==4 && state4==1)
			{
				LCD_vSend_string("On ");
			}
				
			LCD_movecursor(2,1);
			LCD_vSend_string("1:on 2:off 3:ret");
				
			_delay_ms(300);
			unsigned char choice= NOTPRESSED;
			do
			{
				//_delay_ms(250);
				choice= keypad_u8check_press();
			} while(choice == NOTPRESSED);
			
			choice= choice-48;
			
			SPI_MasterTransmitchar(choice);
			_delay_ms(500);
			
			if(choice ==3)
			{
				goto ROOM_PAGE2;
			}				
			else if(choice!=1 && choice!=2 &&choice!=3)
			{
				LCD_clearscreen();
				LCD_movecursor(1,3);
				LCD_vSend_string("Wrong Choice!");
				_delay_ms(1000);
				goto ROOM_PAGE2;
			}
			
			if(room==1){state1 = (choice& 0x01);}
			else if(room==2){state2 = (choice& 0x01);}
			else if(room==3){state3 = (choice& 0x01);}
			else if(room==4){state4 = (choice& 0x01);}
			goto ROOM_PAGE2;
		}	
		else
		{
			LCD_clearscreen();
			LCD_vSend_string("wrong choice!");
			LCD_movecursor(2,3);
			LCD_vSend_string("try again");
			_delay_ms(1000);
		}	
	
	}	
	
}
/*
ISR(TIMER0_COMP_vect)
{
	timeCounter++;
	if(timeCounter==10000)
	{
		timeCounter=0;
		
	}
	
}
*/