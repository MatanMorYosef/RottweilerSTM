#ifndef __EXTI_H_
#define __EXTI_H_

#include "stm32f407xx.h"


#define PI 										3.1416
#define dAngle								360.0 / 3600.0

extern volatile int dir;
extern volatile double angle;

void EXTI_Config(void);
int checkDirection(void);

#endif
