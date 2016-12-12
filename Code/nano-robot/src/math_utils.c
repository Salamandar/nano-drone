#include "math_utils.h"

#include <stdlib.h>


#define ComplementaryRatio (90)

void complementaryFilter (int angle, int rate, int dt, int *filteredAngle) {
    *filteredAngle = (ComplementaryRatio * (*filteredAngle + dt * rate)
                + (100 - ComplementaryRatio) * angle)/100;
}

void complementaryFilter2(int angle, int rate, int dt, int *filteredAngle) {
    const static int k = 10;
    int y1 = dt * (angle - *filteredAngle) * k*k + y1;
    *filteredAngle = dt * (y1 + (angle - *filteredAngle) * 2*k + rate) + *filteredAngle;
}

int fast_atan2(int x, int y) {
    if (y == 0) {
        return (x < 0) * 180;
    }

    if (x == 0) {
        if (y > 0)
            return 90;
        else
            return 270;
    }

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
