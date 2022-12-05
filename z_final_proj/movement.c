#include "movement.h"

volatile char stopFlag;

float turn_clockwise(oi_t *sensor_T, int degrees){
    float sum=0;
    stopFlag = '\0';

    if (degrees<0){
        oi_setWheels(50, -50); // turn left (right wheel forwards)
        while (sum > degrees) {
            oi_update(sensor_T);
            sum -= sensor_T->angle;

            //cliff sensors
            if (sensor_T->cliffLeft){
                stopFlag = '1';
                return sum/10;
            }
            if (sensor_T->cliffFrontLeft){
                stopFlag = '2';
                return sum/10;
            }
            if (sensor_T->cliffFrontRight){
                stopFlag = '3';
                return sum/10;
            }
            if (sensor_T->cliffRight){
                stopFlag = '4';
                return sum/10;
            }
            lcd_init();
            lcd_printf("%f", sum);

        }
    }
    else{
        oi_setWheels(-50, 50); // turn right (left wheel forwards)
        while (sum < degrees) {
            oi_update(sensor_T);
            sum -= sensor_T->angle;

            //cliff sensors
            if (sensor_T->cliffLeft){
                stopFlag = '1';
                return sum/10;
            }
            if (sensor_T->cliffFrontLeft){
                stopFlag = '2';
                return sum/10;
            }
            if (sensor_T->cliffFrontRight){
                stopFlag = '3';
                return sum/10;
            }
            if (sensor_T->cliffRight){
                stopFlag = '4';
                return sum/10;
            }
            lcd_init();
            lcd_printf("%f", sum);
        }
    }
    oi_setWheels(0, 0);
    return sum;//return degrees turned
}


float move_forwards(oi_t *sensor_F, int centimeters, float a, float b){
    stopFlag = '\0';
    centimeters = centimeters*10;
    float sum=0;
    int count=0, dir=1;
    float laserDist = 0;
	
    servo_move(120);
	timer_waitMillis(100);

    if (centimeters>0){ // move forwards
        oi_setWheels(150, 150);

        while (sum < centimeters) {
            oi_update(sensor_F);
            sum += sensor_F->distance;

            //bump sensors
            if (sensor_F->bumpLeft && sensor_F->bumpRight) {
                stopFlag = 'B';
                return sum/10;
            }
            if (sensor_F->bumpLeft) {
                stopFlag = 'L';
                return sum/10;
            }
            if (sensor_F->bumpRight) {
                stopFlag = 'R';
                return sum/10;
            }

            //cliff sensors
            if (sensor_F->cliffLeft){
                stopFlag = '1';
                return sum/10;
            }
            if (sensor_F->cliffFrontLeft){
                stopFlag = '2';
                return sum/10;
            }
            if (sensor_F->cliffFrontRight){
                stopFlag = '3';
                return sum/10;
            }
            if (sensor_F->cliffRight){
                stopFlag = '4';
                return sum/10;
            }


            //sweep sensor
			if (dir>0){ //moving right (angle decreases)
                servo_move(count);
                count--;
            }
            else if (dir<0){
                servo_move(count);
                count++;
            }
			
			if (count >= 120){
				dir = -1;
			}
			else if (count <= 60){
				dir = 1;
			}
			

            laserDist = 0; //using laserDist value for laserADC to conserve memory
            laserDist += laser_read();
            laserDist += laser_read();
            laserDist += laser_read();
            laserDist /= 3;
			
            laserDist = a * pow(laserDist, b);

            if (laserDist < 30){ //(30 cm arbitrarily chosen)
                stopFlag = 'O'; //O for Object (seen)
				return sum/10;
            }

            lcd_init();
            lcd_printf("%f", laserDist);
        }
    }
    else{ //move backwards
        oi_setWheels(-150, -150);

        while (sum > centimeters) {
            oi_update(sensor_F);
            sum += sensor_F->distance;
            lcd_init();
            lcd_printf("%f", sum);
        }
    }

    oi_setWheels(0, 0); //set wheels to 0
    return sum/10; //sum in cm
}







