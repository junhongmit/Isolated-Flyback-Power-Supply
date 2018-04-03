#ifndef __PID_H
#define __PID_H 

#include "sys.h"
#define DUTY_MAX 0.67
#define DUTY_MIN 0

#define KP			0 
#define KI			0.005
#define KD          0


typedef signed long pidDataType;
extern s16 umax,umin;
void PID(float feedBack, float setValue, float* Current);
#endif
