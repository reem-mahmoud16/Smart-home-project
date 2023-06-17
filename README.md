# Smart-home-project

A smart home system with 2 AVR (atmega32) microcontrollers, communicating with SPI protocol.
Used drivers: DIO, SPI, Timer, EEPROM, ADC, LCD, Keybad, LED.

***project description:

In this project, two microcontrollers are used. the first(master) is user-interfacing one, while the second(slave) controls the system. 
In the user screen, first you will be asked to determine whether you are an adminstrator or a guest. 
At each case if you are using the system for the first time, you will bw asked to create your own 4-digit password. For later sessions you will be asked to enter the password. Note that you have only 3 chances to enter your password correctly. 

Once you have started your session successfully, you must take the time limit of the session into consideration, which in this vesion is 90 seconds.

you can see below 2 LEDs for indicating the current system-user type (whether an admin or a guest). 
Also there is a third LED for the case of blocked state(which happens whenever the user finishes his password typing tries while signing in.

**For adminstrator session:

    User has the following facilities of controling the system:
    1- control the 4 home rooms' lambs.
    2- control the TV.
    3- control the air conditioner (you can change the critical temp. at which the air conditioner is automatically turned on once the home's temp. exceeds it).
       
**For guest session:

    User only has the permission to control the 4 home rooms' lambs.
