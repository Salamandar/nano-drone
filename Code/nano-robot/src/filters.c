#include "filters.h"

#include <stdlib.h>


#define ComplementaryRatio (90)

void complementaryFilter(int angle, int rate, int dt, int *filteredAngle) {
    *filteredAngle = (ComplementaryRatio * (*filteredAngle + dt * rate)
                + (100 - ComplementaryRatio) * angle)/100;
}

// newAngle = angle measured with atan2 using the accelerometer
// newRate = angle measured using the gyro
// looptime = loop time in millis()

// int Complementary2(int newAngle, int newRate,int looptime) {
//     int k=10;
//     int dtc2=int(looptime)/1000.0;

//     x1 = (newAngle -   x_angle2C)*k*k;
//     y1 = dtc2*x1 + y1;
//     x2 = y1 + (newAngle -   x_angle2C)*2*k + newRate;
//     x_angle2C = dtc2*x2 + x_angle2C;

//     return x_angle2C;
// }



int fast_atan2(int x, int y) {
    if (y == 0)
        return (x < 0) * 180;

    if (x == 0)
        if (y > 0)
            return 90;
        else
            return 270;

    int angle;


    if (abs(y) <= abs(x))
        angle =   abs(3667 * x * y) / (64 * x * x + 17 * y * y);
    else
        angle = - abs(3667 * x * y) / (64 * y * y + 17 * x * x) + 90;

    if (x < 0)
        angle = 180 - angle;

    if (y < 0)
        angle = 360 - angle;



    return angle;
}
