#include"pid.h"
#include"sys.h"
s16 umax=3.5,umin=2.5;
float integral = 0;
float err;
//float err_last;
float tempDuty  ;
//float err_last;
s16 abs(s16 x)
{
	if (x<0) return -x; else return x;
}
void PID(float feedBack, float setValue, float* DutyCycle)
{
	u8 index;
	err = setValue - feedBack;
	/*if (feedBack>setValue+0.5) {
		if (abs(err)>5) index=0;
		else {
			index=1;
			if (err<0) {
				integral+=err;
			}
		}
	} else if (feedBack<setValue-0.5) {
		if (abs(err)>5) index=0;
		else {
			index=1;
			if (err>0) {
				integral+=err;
			}
		}
	} else {
		if (abs(err)>5) index=0;
		else {
			index=1;
			
		}
	}*/
	/*tempDuty = *dutyCycle;
	delta = KP*err + KI*errTotal;
	
	delta = delta / FACTOR;
	tempDuty += delta;
	errTotal = err;*/
	integral+=err;
	tempDuty=KP*err+KI*integral;//+KD*(err-err_last);
	
	if (tempDuty > DUTY_MAX)	
		tempDuty = DUTY_MAX;
	if (tempDuty < DUTY_MIN)
		tempDuty = DUTY_MIN;
	
	*DutyCycle = tempDuty;
	//err_last=err;
}
