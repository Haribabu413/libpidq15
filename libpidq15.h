#ifndef _LIB_PID_Q15_H
#define _LIB_PID_Q15_H

#include "libmathq15.h"

typedef enum _pidmode {
    ePID_CLOSED_LOOP, ePID_OPEN_LOOP
}pidmode_t;

typedef struct{
    q15_t kp, ki, kd;
    q15_t iLast, measLast, outLast;
    q15_t maxOutput, minOutput, maxError;
    pidmode_t mode;
}pidq15_t;

void pid_init(pidq15_t* pidStruct);
void pid_setGains(pidq15_t* pidStruct, q15_t kp, q15_t ki, q15_t kd);
void pid_setMaxOutput(pidq15_t* pidStruct, q15_t maxOutput);
void pid_setMinOutput(pidq15_t* pidStruct, q15_t minOutput);
void pid_setMaxError(pidq15_t* pidStruct, q15_t maxError);
void pid_setMode(pidq15_t* pidStruct, pidmode_t mode);
q15_t pid_loop(pidq15_t* pidStruct, q15_t measured, q15_t setpoint);

#endif
