

#include "open_interface.h"
#include "Timer.h"
#include "movement.h"


void main() {

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); // should turn the iRobot Create's "Dirt Detect" LED to blue

    int sides = 4;
    int i;

   /* turn_clockwise(sensor_data, 90);
        move_forward(sensor_data, 25);
        turn_clockwise(sensor_data, -90);*/

    //for (i=0; i<sides; i++){
        move_forward(sensor_data, 200);
        //turn_clockwise(sensor_data, -90);
    //}




    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);
}
