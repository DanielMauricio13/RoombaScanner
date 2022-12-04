


#include "sensor.h"
#include "Timer.h"
#include "button.h"
#include "lcd.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>


extern volatile int uart_event;
extern volatile char uart_data;
extern volatile char stopFlag;

const float a_las=3906882.5, b_las=-1.618058; //set this up in a different file (lab 8 main)
const float degToRad = 3.141592653/180;
const int minObjDist = 55; //minimum distance detected by ping sensor for something to be considered an "object"


float dist;
char puttyIn[21] = "";



struct tallObj { //contains data for all tall objects
    int startAng;
    int stopAng;
    float objDist;
    float width;
    int actualAng;
};
struct tallObj tObject[5];

       //measures 180 degrees, stores object amount, location, and size
int measure_data(){
    int i, j, laserADC, midAng;
    float laserDist;


    int isObject = 0;
    int numObjects = 0;

    tObject[0].stopAng=0;

    for (j=0; j<=180; j+=2){

        servo_move(j);

        laserADC = 0;
        for (i=0; i<3; i++){
            laserADC += laser_read();
        }
        laserADC /= 3;

        //print angle
        strcpy(puttyIn,"");
        sprintf(puttyIn, "%d\t", j);
        uart_sendStr(puttyIn);


        //get/print ir distance
        laserDist = a_las * pow(laserADC, b_las);
        sprintf(puttyIn, "%f\r\n", laserDist);
        uart_sendStr(puttyIn);


        if ( (laserDist<minObjDist) && (isObject==0) ){ //detects object
            isObject=1;
            tObject[numObjects].startAng = j;
        }
        if ( (laserDist>minObjDist) && (isObject== 1) ){ //detects lack of object
            tObject[numObjects].stopAng = j-2;
            isObject=0;
            if ((tObject[numObjects].stopAng - tObject[numObjects].startAng) > 3){
                        //verifies object exists and is not a sensor scuffing data
                numObjects++;
            }

        }
    }//finish measuring


    uart_sendStr("\r\n\nObject\tAngle\tDistance\tWidth\r\n"); //header of object info


    //prints objects
    for (i=0; i<numObjects;i++){ //numObjects begins reading useful data at i=1
        midAng = ((tObject[i].stopAng + tObject[i].startAng)/2); //middle angle of object
        servo_move(midAng);
        timer_waitMillis(800);
        tObject[i].objDist = ping_read();
        timer_waitMillis(100);


        tObject[i].width = tObject[i].objDist * 2 * sin( (tObject[i].stopAng - tObject[i].startAng)*degToRad/2 ); //measures width of object (cm)


        strcpy(puttyIn,"");
        sprintf(puttyIn, "%d\t", i);
        uart_sendStr(puttyIn); //sends object #

        sprintf(puttyIn, "%d\t", midAng);
        uart_sendStr(puttyIn); //sends middle angle

        sprintf(puttyIn, "%f\t", tObject[i].objDist);
        uart_sendStr(puttyIn); //sends distance away

        sprintf(puttyIn, "%f\r\n", tObject[i].width);
        uart_sendStr(puttyIn); //sends linear width

        tObject[i].actualAng = ((atan( ((tObject[i].objDist+5.0)*cos(midAng*degToRad)) /
                                      (11.0+(tObject[i].objDist+5.0)*sin(midAng*degToRad)) )/degToRad) + 0.5);
                //center of bot (COB) / center of sensor (COS)
                //5 is cm distance between COS and actual measuring point
                //11 is cm distance between COB/COS
    }
    uart_sendStr("Done\r\n\n");
	return numObjects;
}







void main(){
    init_sensors();
    timer_init();
    lcd_init();
    button_init();

    uart_init();
    uart_interrupt_init();
    uart_event=0;


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);



    int numChars=0;
	int amountScanned;
	float amountMoved;
    char whatDo;
    int amountDo;

	amountMoved = 0;
    while(1){

        if (uart_event){ //if putty detects a keypress
            uart_event = 0;

            puttyIn[numChars] = uart_data;
            numChars++;

            uart_sendChar(uart_data);

            if (uart_data == '\r'){ //once send character is detected, begin operation (send character is newline)

				amountDo = atoi(puttyIn); //converts numbers in the string to an int until something that isn't a number is reached
				whatDo = puttyIn[numChars-2]; //converts last user input before enter key into a command

				uart_sendChar('\r');
				uart_sendChar('\n');

                switch (whatDo){
                case ('w'): //w is move forwards (cm)
                        sprintf(puttyIn, "Moving forwards for %d cm\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountMoved = move_forwards(sensor_data, amountDo, a_las, b_las);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('s'): //s is move backwards (cm)
                        sprintf(puttyIn, "Moving backwards for %d cm\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountDo *= -1;
                        amountMoved = move_forwards(sensor_data, amountDo, a_las, b_las);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('a'): //a is rotate left (degrees)
                        sprintf(puttyIn, "Turning left for %d degrees\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountDo *= -1;
                        amountMoved = turn_clockwise(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('d'): //d is rotate right (degrees)
                        sprintf(puttyIn, "Turning right for %d degrees\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountMoved = turn_clockwise(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('c'): //calibrate
                        uart_sendStr("Calibrating\r\n\n");
                        servo_calibrate();
                        break;

                case ('f'):
                    oi_free(sensor_data);
                    break;

                default:
                    uart_sendStr("Invalid Input\r\n\n");
                }//end of switch
				
				
				if (whatDo != 'c'){ //do not measure after calibrating the sensor
					//scan 180 degrees and send to putty
					uart_sendStr("Ang\tDist (ping)\tDist (laser)\r\n"); //header of measurement

					servo_move(0);
					timer_waitMillis(1000);

					amountScanned = measure_data();
					
					//send amount moved
					uart_sendStr("START amount moved: ");
					sprintf(puttyIn, "%f", amountMoved);
					uart_sendStr(puttyIn);
					uart_sendStr("END amount moved/n/r");


                    //stop flag has been changed due to
					if (stopFlag == 'L'){
					    uart_sendStr("START bumped left END");
					}
					if (stopFlag == 'R'){
                        uart_sendStr("START bumped right END");
                    }
					if (stopFlag == 'B'){
                        uart_sendStr("START bumped both END");
                    }
					if (stopFlag == '1'){
                        uart_sendStr("START cliff left left END");
                    }
					if (stopFlag == '2'){
                        uart_sendStr("START cliff mid left END");
                    }
					if (stopFlag == '3'){
                        uart_sendStr("START cliff mid right END");
                    }
					if (stopFlag == '4'){
                        uart_sendStr("START cliff right right END");
                    }
					// do we want a message that returns if an object is seen in front of the roomba while driving?


					for (numChars = 0; numChars<amountScanned; numChars++){ //funny memory conservation
					    uart_sendStr("START obj:");
						sprintf(puttyIn, " %d", numChars);
						uart_sendStr(puttyIn);
						sprintf(puttyIn, " %d", tObject[numChars].actualAng);
						uart_sendStr(puttyIn);
						sprintf(puttyIn, " %f", tObject[numChars].objDist);
						uart_sendStr(puttyIn);
						sprintf(puttyIn, " %f", tObject[numChars].width);
						uart_sendStr(puttyIn);
						uart_sendStr(" END\n\r");
					}
					numChars=0; //reset
				}
				
				
				
				
            }
        }





    }



}




