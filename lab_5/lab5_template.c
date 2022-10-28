/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */


//#include <stdio.h>
//#include <stdint.h>
//#include <string.h>
//#include <stdlib.h>
//#include <inc/tm4c123gh6pm.h>

#include "button.h"
#include "timer.h"
#include "lcd.h"
//#include <math.h>

#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

//#include "cyBot_Scan.h"  // For scan sensors


//#warning "Possible unimplemented functions"
#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {
	button_init();
	lcd_init();
	init_button_interrupts();

	//cyBOT_init_Scan(0b0111);

    cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization
	SYSCTL_RCGCGPIO_R |= 0b000010;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R &= ~0xFF;     // Force 0's in the desired locations       //0xFFFF_FF00
    GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the desired locations        //0x0000_0011
    GPIO_PORTB_DEN_R |= 0b00000011;
    GPIO_PORTB_DIR_R &= ~0b1;      // Force 0's in the desired locations
    GPIO_PORTB_DIR_R |= 0b00000010;      // Force 1's in the desired locations

    cyBot_uart_init_last_half();  // Complete the UART device initialization part of configuration


    char msg[8];
    int i;

	while(1)
	{

	    //int cybot_get = cyBot_getByte();
	    if (button_event){
	        button_event=0;

	        lcd_printf("Button %d", button_num);

	        cyBot_sendByte('\r');
	        sprintf(msg, "Button %d", button_num);

            for (i=0; i<strlen(msg); i++){
                    cyBot_sendByte(msg[i]);
            }
            timer_waitMillis(100);
	    }



	
	}
	
}
