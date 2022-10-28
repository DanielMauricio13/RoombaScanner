/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _RESET 1
#define _PART1 0
#define _PART2 0
#define _PART3 0

#include "timer.h"
#include "lcd.h"
#include "uart.h"

#if _RESET
//#include "Simulation_User Files_resetSimulation.h"
#endif

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

extern volatile int uart_event;
extern volatile char uart_data;


void main()
{

    timer_init();
    uart_init();
    lcd_init();
    uart_interrupt_init();
    int numChars=0;
    char string[21] = "";
    int amount;
    uart_event=0;

    while(1){
        //char cybot_get = uart_receive();
        if (uart_event){
            uart_event = 0;

            string[numChars] = uart_data;
            numChars++;

            uart_sendChar(uart_data);

            //timer_waitMillis(100);

            if ((numChars == 20) | (uart_data == '\r')){
                amount = numChars;
                string[numChars]='\0';
                if (uart_data == '\r'){
                    amount=numChars-1;
                    string[numChars-1]='\0';
                }
                lcd_printf("%s\n\n%d", string, amount);
                numChars=0;
                uart_sendChar('\r');
                uart_sendChar('\n');
            }
        }
    }

}





