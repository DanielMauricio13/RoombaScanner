

    //short objects (bump)
    //holes (cliff)
    //tall objects (cyBOT_Scan )


/*
cyBOT_Scan

OI_Sensort
    distance

    angle

    bumpLeft
    bumpRight

    cliffFrontLeft
    cliffFrontRight
*/

#include <math.h>
#include <cyBot_uart.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inc/tm4c123gh6pm.h>
#include <lcd.h>
//#include <open_interface.h>
#include <Timer.h>
#include <movement.h>
#include <cyBot_Scan.h>

int main()
{
    //inits
    cyBot_uart_init();
    lcd_init();
    cyBOT_init_Scan(0b0011);


    char msg[] = "Got an m";
    int i, j;
    char printStatement[] = "Degrees\tDistance (cm)";
    char val[10] = "\0";
    char val2[6];
    cyBOT_Scan_t scanData;

    const int minObjDist = 70; //minimum distance detected by ping sensor for something to be considered an "object"
    int startAng[20];
    int stopAng[20];
    float objDist[20];
    int isObject = 0;
    int numObjects = 0;

    cyBOT_Scan_t* scanDataPtr = &scanData;


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);



    while (1){

        int cybot_get = cyBot_getByte();

        switch (cybot_get){
            case ('m'):
                for (i=0; i<sizeof(msg)/sizeof(msg[0]); i++){
                        cyBot_sendByte(msg[i]);
                }
                cyBot_sendByte('\r');
                cyBot_sendByte('\n');
                oi_setWheels(0, 0);
                break;

            case ('w'):
                oi_setWheels(200, 200);
                break;

            case ('a'):
                oi_setWheels(200, -200);
                break;

            case ('s'):
                oi_setWheels(-200, -200);
                break;

            case ('d'):
                oi_setWheels(-200, 200);
                break;

            case ('c'): //calibrate
                cyBOT_SERVO_cal();
                break;

                //case for scanning surroundings
            case ('p'):
                oi_setWheels(0, 0);
                isObject = 0;
                numObjects=1;
                stopAng[0]=0;

                for (i=0; i<sizeof(printStatement)/sizeof(printStatement[0]); i++){
                        cyBot_sendByte(printStatement[i]);
                }
                cyBot_sendByte('\r');
                cyBot_sendByte('\n');


                cyBOT_Scan(0, scanDataPtr);
                timer_waitMillis(100);

                // go from 0 to 180 degrees
                for (j=0; j<=180; j+=2){
                    cyBOT_Scan(j, scanDataPtr);
                    timer_waitMillis(100);

                    //typecast degree to char and print degree val
                    sprintf(val, "%d", j);
                    for (i=0; i<sizeof(val)/sizeof(val[0]); i++){
                         cyBot_sendByte(val[i]);
                    }
                    cyBot_sendByte('\r');
                    cyBot_sendByte('\t');

                    //typecast distance val to char and print distance val
                    sprintf(val2, "%f", scanData.sound_dist);
                    for (i=0; i<sizeof(val2)/sizeof(val2[0]); i++){
                         cyBot_sendByte(val2[i]);
                   }
                    cyBot_sendByte('\r');
                    cyBot_sendByte('\n');

                    if ( (scanData.sound_dist<minObjDist) && (isObject==0) && ((j-stopAng[numObjects-1])>2) ){ //2 is max angle of whatever
                        isObject=1;
                        startAng[numObjects]=j;
                        objDist[numObjects]=scanData.sound_dist;
                    }
                    if ( (scanData.sound_dist>minObjDist) && (isObject==1) && ((j - startAng[numObjects-1])>2) ){
                        isObject=0;
                        stopAng[numObjects]=j;
                        numObjects++;
                    }
                } //finish measuring degrees/distance


                cyBot_sendByte('\r'); cyBot_sendByte('\n'); cyBot_sendByte('\r'); cyBot_sendByte('\n'); //prints out 2 newlines

                char printStatement2[] = "Object\tAngle\tDistance\tWidth";
                for (i=0; i<sizeof(printStatement2)/sizeof(printStatement2[0]); i++){ //27 chars in header
                        cyBot_sendByte(printStatement2[i]);
                }
                cyBot_sendByte('\r');
                cyBot_sendByte('\n');


                //prints objects
                for (i=1; i<(numObjects+1);i++){ //numObjects begins reading useful data at i=1
                    float linWidth = objDist[i] * sin( (stopAng[i] - startAng[i])*3.14159/180 );

                    sprintf(val, "%d", i);
                    cyBot_sendByte(val[0]); cyBot_sendByte('\t'); //sends object #

                    sprintf(val, "%d", startAng[i]);
                    for (j=0; j<sizeof(val)/sizeof(val[0]); j++){
                         cyBot_sendByte(val[j]);
                    } cyBot_sendByte('\t'); //sends angle start

                    sprintf(val2, "%f", objDist[i]);
                    for (j=0; j<sizeof(val2)/sizeof(val2[0]); j++){
                         cyBot_sendByte(val2[j]);
                    } cyBot_sendByte('\t'); cyBot_sendByte('\t'); //sends distance away


                    sprintf(val2, "%f", linWidth);
                    for (j=0; j<sizeof(val2)/sizeof(val2[0]); j++){
                         cyBot_sendByte(val2[j]);
                    } cyBot_sendByte('\r'); cyBot_sendByte('\n'); //sends linear width

                }



                break; //end case p (its a long one)


            default:
                lcd_putc(cybot_get);
                oi_setWheels(0, 0);
        }


    }

    oi_free(sensor_data);



}









