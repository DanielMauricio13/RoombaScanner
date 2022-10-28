/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Describtion: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
//sizeof(msg)/sizeof(msg[0])

int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the the LCD screen.  This also clears the screen. 

	// Clear the LCD screen and print "Hello, world" on the LCD
	//lcd_puts("\nHello, world");

	char * spaces = "                    Microcontrollers are lots of fun!";
	//char * msg = "";

	//strcat(spaces, msg);

	int length = 19;
	int offset=0;

	while(true){
	    if (length < 0){
	        length = 19;
	        offset = 0;
	    }
	    lcd_init();


	    lcd_printf("%.*s", length, spaces + offset);

	    if(spaces[length+offset] == '\0'){
	        length --;
	    }

	    offset ++;
        timer_waitMillis(150);
	}


	// lcd_puts("Hello, world");// Replace lcd_printf with lcd_puts
                                 // step through in debug mode
                                 // and explain to TA how it
                                 // works


    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
    // NOTE: For time fuctions see Timer.h

	return 0;
}
