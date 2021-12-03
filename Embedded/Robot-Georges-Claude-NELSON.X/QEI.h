#ifndef QEI_H
#define	QEI_H
#include <math.h>
#define PI 3.14159265359

#define FREQ_ECH_QEI 250
#define DISTROUES 0.2812
#define POINT_TO_METER 0.000016336  //(WHEEL_DIAMETER * PI / 8192.0)
#define POSITION_DATA 0x0061
#define NUMBER_OF_POINTS 8192

// (distroue * pi)/ numbre de points = point_to_meter


void InitQEI1();
void InitQEI2();
void QEIUpdateData();
void SendPositionData();

#endif	/* QEI_H */

