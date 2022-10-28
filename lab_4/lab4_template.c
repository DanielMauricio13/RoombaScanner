/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#define REPLACEME 0



int main(void) {
	button_init();
	lcd_init();
	cyBot_uart_init();
    char msg[] = "Button 0";
    int i;
	
	lcd_clear();
	
	while(1){

	    if(button_getButton()==4){
	        lcd_printf("Button 4");
	        msg[7]='4';
	        for (i=0; i<sizeof(msg)/sizeof(msg[0]); i++){
                    cyBot_sendByte(msg[i]);
            }
	        cyBot_sendByte('\r');
	        //cyBot_sendByte('\n');
	        timer_waitMillis(100);
	    }
	    else if(button_getButton()==3){
            lcd_printf("Button 3");
            msg[7]='3';
            for (i=0; i<sizeof(msg)/sizeof(msg[0]); i++){
                    cyBot_sendByte(msg[i]);
            }
            cyBot_sendByte('\r');
            //cyBot_sendByte('\n');
            timer_waitMillis(100);
        }
	    else if(button_getButton()==2){
            lcd_printf("Button 2");
            msg[7]='2';
            for (i=0; i<sizeof(msg)/sizeof(msg[0]); i++){
                    cyBot_sendByte(msg[i]);
            }
            cyBot_sendByte('\r');
            //cyBot_sendByte('\n');
            timer_waitMillis(100);
        }
	    else if(button_getButton()==1){
            lcd_printf("Button 1");
            msg[7]='1';
            for (i=0; i<sizeof(msg)/sizeof(msg[0]); i++){
                    cyBot_sendByte(msg[i]);
            }
            cyBot_sendByte('\r');
            //cyBot_sendByte('\n');
            timer_waitMillis(100);
        }
	
	}
	
}
