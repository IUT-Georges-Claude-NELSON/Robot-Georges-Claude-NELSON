#ifndef QEI_H
#define	QEI_H
#include <math.h>
#define PI 3.14159265359

#define FREQ_ECH_QEI 250


void InitQEI1();
void InitQEI2();
void QEIUpdateData();
void SendPositionData();

#endif	/* QEI_H */

