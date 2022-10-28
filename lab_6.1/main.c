


#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "movement.h"
#include <cyBot_Scan.h>
#include "inc/tm4c123gh6pm.h"


extern volatile int uart_event;
extern volatile char uart_data;

int minAngWhere;
int minAngWid;
int stupidDistanmce;
char puttyIn[51] = "";


       //input r/l angles, cybot data variable and pointer, and power data from laser calibration
void measure_data(int rightScan, int leftScan,  cyBOT_Scan_t scanData, cyBOT_Scan_t* scanDataPtr, float a_las, float b_las){
    int i, j, laserADC;
    float laser_dist_num;
    scanDataPtr = &scanData;

    const int minObjDist = 60; //minimum distance detected by ping sensor for something to be considered an "object"
    int startAng[8];
    int stopAng[8];
    float objDist[8];
    int isObject = 0;
    int numObjects = 0;

    minAngWhere = 90; //this is so scuffed, idk why


    int midAng;
    float linWidth;


    stopAng[0]=0;

    for (j=rightScan; j<=leftScan; j+=2){
        if (uart_event & (uart_data == 't')){
            break;
        }
        laserADC = 0;
        for (i=0; i<3; i++){
            cyBOT_Scan(j, scanDataPtr);
            laserADC += scanData.IR_raw_val;
        }
        laserADC /= 3;

        //print angle
        strcpy(puttyIn,"");
        sprintf(puttyIn, "%d\t", j);
        uart_sendStr(puttyIn);

        //print distance (ping)
        sprintf(puttyIn, "%f\t", scanData.sound_dist);
        uart_sendStr(puttyIn);


        //get/print ir distance
        laser_dist_num = a_las * pow(laserADC, b_las);
        sprintf(puttyIn, "%f\r\n", laser_dist_num);
        uart_sendStr(puttyIn);


        if ( (laser_dist_num<minObjDist) && (isObject==0) ){ //detects object
            isObject=1;
            startAng[numObjects+1]=j;
            objDist[numObjects+1]=scanData.sound_dist;
        }
        if ( (laser_dist_num>minObjDist) && (isObject== 1) ){ //detects lack of object
            stopAng[numObjects+1]=j-2;
            isObject=0;
            if ((stopAng[numObjects+1]-startAng[numObjects+1]) > 3){ //verifies object exists and is not a sensor scuffing data
                numObjects++;
            }

        }
    }//finish measuring


    uart_sendStr("\r\n\nObject\tAngle\tDistance\tWidth\r\n"); //header of object info

    if (numObjects>=1){
        minAngWhere = (stopAng[1] + startAng[1])/2;
        minAngWid = stopAng[1] - startAng[1];
    }


    //prints objects
    for (i=1; i<=numObjects;i++){ //numObjects begins reading useful data at i=1
        midAng = ((stopAng[i] + startAng[i])/2); //middle angle of object
        timer_waitMillis(100);
        cyBOT_Scan(midAng, scanDataPtr);
        objDist[i] = scanData.sound_dist;
        timer_waitMillis(500);


        linWidth = objDist[i] * 2 * sin( (stopAng[i] - startAng[i])*3.14159/360 ); //measures width of object (cm)


        strcpy(puttyIn,"");
        sprintf(puttyIn, "%d\t", i);
        uart_sendStr(puttyIn); //sends object #

        sprintf(puttyIn, "%d\t", midAng);
        uart_sendStr(puttyIn); //sends middle angle

        sprintf(puttyIn, "%f\t", objDist[i]);
        uart_sendStr(puttyIn); //sends distance away

        sprintf(puttyIn, "%f\r\n", linWidth);
        uart_sendStr(puttyIn); //sends linear width

        if ((stopAng[i]-startAng[i]) < minAngWid){  //if the current object width is smaller than the previous
            minAngWhere = midAng;                   //sets point to mid angle of (current) smallest width object
            minAngWid = stopAng[i]-startAng[i];
        }
    }
    uart_sendStr("Done\r\n\n");
    //return minAngWhere;
}







void main(){
    timer_init();
    lcd_init();
    uart_init();
    uart_interrupt_init();
    uart_event=0;

    int dataPoints=20;
    float x[22], y[22], sumX=0, sumX2=0, sumY=0, sumXY=0, A;

    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t scanData;
    cyBOT_Scan_t* scanDataPtr = &scanData;
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    right_calibration_value = 269500;
    left_calibration_value = 1214500;



    float a=140841.515625, b=-1.167015;
    int i, j;


    int numChars=0;
    char whatDo;
    int amountDo;
    int leftScan, rightScan;




    while(1){

        if (uart_event){ //if putty detects a keypress
            uart_event = 0;

            puttyIn[numChars] = uart_data;
            numChars++;

            uart_sendChar(uart_data);

            if (uart_data == '\r'){ //if enter key pressed      enter distance/angle and wasd char

                amountDo = atoi(puttyIn); //converts numbers in the string to an int until something that isn't a number is reached
                whatDo = puttyIn[numChars-2]; //converts last user input before enter key into a command

                numChars=0; //reset
                uart_sendChar('\r');
                uart_sendChar('\n');
                //uart_sendChar(whatDo);

                switch (whatDo){
                case ('w'): //w is move forwards (cm)
                        sprintf(puttyIn, "Moving forwards for %d cm\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        move_noBump(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('s'): //s is move backwards (cm)
                        sprintf(puttyIn, "Moving backwards for %d cm\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountDo *= -1;
                        move_noBump(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('a'): //a is rotate left (degrees)
                        sprintf(puttyIn, "Turning left for %d degrees\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        amountDo *= -1;
                        turn_clockwise(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('d'): //d is rotate right (degrees)
                        sprintf(puttyIn, "Turning right for %d degrees\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        turn_clockwise(sensor_data, amountDo);
                        uart_sendStr("Done\r\n\n");
                        break;

                case ('j'): //laser calibaration
                    sprintf(puttyIn, "Calibrating Laser Value\r\n");
                    uart_sendStr(puttyIn);

                    for(i=1;i<=dataPoints;i++){//measure points, wait, move, wait
                        x[i] = 0;
                        y[i] = 0;
                        for(j=0; j<3; j++){
                            cyBOT_Scan(90, scanDataPtr);
                            x[i] += scanData.IR_raw_val;
                            y[i] += scanData.sound_dist;
                        }
                        x[i] /= 3;
                        y[i] /= 3;


                        timer_waitMillis(100);
                        move_noBump(sensor_data, -1);
                        timer_waitMillis(300);
                    }

                    // Calculating Required Sum
                    sumX=0, sumX2=0, sumY=0, sumXY=0;
                    for(i=1;i<=dataPoints;i++){
                        sumX += log(x[i]);
                        sumX2 += log(x[i])*log(x[i]);
                        sumY += log(y[i]);
                        sumXY += log(x[i])*log(y[i]);
                    }

                    // Calculating A and b
                    b = (dataPoints*sumXY-sumX*sumY)/(dataPoints*sumX2-sumX*sumX);
                    A = (sumY - b*sumX)/dataPoints;

                    // Transformation of A to a
                    a = exp(A);

                    //equation: a*x^b
                    sprintf(puttyIn, "equation: %f*x^%f\r\n", a, b);
                    uart_sendStr(puttyIn);
                    uart_sendStr("Done\r\n\n");
                    break;

                case ('m'): //measure (input is arc length in degrees)
                        sprintf(puttyIn, "Arc Length: %d\r\n", amountDo);
                        uart_sendStr(puttyIn);
                        leftScan = (90+amountDo/2);
                        rightScan = (90-amountDo/2);


                        uart_sendStr("Ang\tDist (ping)\tDist (laser)\r\n"); //header of measurement

                        cyBOT_Scan(rightScan, scanDataPtr);
                        timer_waitMillis(300);

                        measure_data(rightScan, leftScan, scanData, scanDataPtr, a, b);

                        //midAngSmall = measure_data(rightScan, leftScan, scanData, scanDataPtr, a, b);
                        turn_clockwise(sensor_data, (90-minAngWhere));
                        cyBOT_Scan(90, scanDataPtr);
                        while (scanData.sound_dist > 10){

                            if (uart_event & (uart_data == 't')){
                                break;
                            }

                            stupidDistanmce = scanData.sound_dist*0.5;
                            move_forward(sensor_data, (stupidDistanmce));

                            measure_data(70, 120, scanData, scanDataPtr, a, b);
                            turn_clockwise(sensor_data, (90-minAngWhere));
                            cyBOT_Scan(90, scanDataPtr);
                        }

                        break; //end of measure case

                case ('c'): //calibrate
                        uart_sendStr("Calibrating\r\n\n");
                        cyBOT_SERVO_cal();
                        break;

                case ('f'):
                    oi_free(sensor_data);
                    break;

                default:
                    uart_sendStr("Invalid Input\r\n\n");
                }

            }
        }





    }



}





