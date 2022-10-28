
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
    timer_init();
    cyBot_uart_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    right_calibration_value = 290500;//264250;
    left_calibration_value = 1256500;//1246000;


    int i, j;
    char printStatement[] = "Ang\tDist (ping)\tDist (laser)";
    char ang[10] = "\0";
    char ping_dist[6];
    char laser_raw[3];
    char laser_dist[6];
    float laser_ADC_VAL;


    const int minObjDist = 70; //minimum distance detected by ping sensor for something to be considered an "object"
    int startAng[20];
    int stopAng[20];
    float objDist[20];
    int isObject = 0;
    int numObjects = 0;


    cyBOT_Scan_t scanData;
    cyBOT_Scan_t* scanDataPtr = &scanData;



    while (1){

        int cybot_get = cyBot_getByte();

        if (cybot_get == 'c'){//calibrate
            cyBOT_SERVO_cal();
        }

        if (cybot_get == 'p'){
            isObject = 0;
            numObjects=1;
            stopAng[0]=0;

            /*for (i=0; i<sizeof(printStatement)/sizeof(printStatement[0]); i++){
                    cyBot_sendByte(printStatement[i]);
            }
            cyBot_sendByte('\r');
            cyBot_sendByte('\n');


            cyBOT_Scan(0, scanDataPtr);
            timer_waitMillis(100);
            */

            // go from 0 to 180 degrees
            for (j=0; j<=0; j+=2){
                cyBOT_Scan(j, scanDataPtr);
                //timer_waitMillis(100);

                //print angle
                ang[0] = '\0'; ang[1] = '\0'; ang[2] = '\0';
                sprintf(ang, "%d", j);
                for (i=0; i<sizeof(ang)/sizeof(ang[0]); i++){
                     cyBot_sendByte(ang[i]);
                }
                cyBot_sendByte('\t');

                //print ping distance
                sprintf(ping_dist, "%f", scanData.sound_dist);
                for (i=0; i<sizeof(ping_dist)/sizeof(ping_dist[0]); i++){
                    cyBot_sendByte(ping_dist[i]);
                }
                cyBot_sendByte('\t');
                cyBot_sendByte('\t');


                //print ir raw val
                laser_ADC_VAL = 8939147 * pow(scanData.IR_raw_val, -1.7308);
                sprintf(laser_dist, "%f", laser_ADC_VAL);
                for (i=0; i<sizeof(laser_dist)/sizeof(laser_dist[0]); i++){
                    cyBot_sendByte(laser_dist[i]);
                }
                cyBot_sendByte('\r');
                cyBot_sendByte('\n');
                lcd_printf("ping: %f\nlaser: %f", scanData.sound_dist, laser_ADC_VAL);


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


            /*cyBot_sendByte('\r'); cyBot_sendByte('\n'); cyBot_sendByte('\r'); cyBot_sendByte('\n'); //prints out 2 newlines

            char printStatement2[] = "Object\tAngle\tDistance\tWidth";
            for (i=0; i<sizeof(printStatement2)/sizeof(printStatement2[0]); i++){ //27 chars in header
                    cyBot_sendByte(printStatement2[i]);
            }
            cyBot_sendByte('\r');
            cyBot_sendByte('\n');*/
        }




    }




}









