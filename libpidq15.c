#include "libpidq15.h"

void pid_init(pidq15_t* pidStruct){
    pidStruct->measLast = 0;
    pidStruct->iLast = 0;
    
    /* intitialize the pid variables to the most reasonable starting values */
    pidStruct->kp = 16384;
    pidStruct->ki = 0;
    pidStruct->kd = 0;
    
    pidStruct->maxOutput = 32767;
    pidStruct->minOutput = 0;
    pidStruct->maxError = 32767;
    
    pidStruct->mode = ePID_AUTO;
    
    return;
}

void pid_setGains(pidq15_t* pidStruct, q15_t kp, q15_t ki, q15_t kd){
    pidStruct->kp = kp;
    pidStruct->ki = ki;
    pidStruct->kd = kd;
    
    return;
}

void pid_setMaxOutput(pidq15_t* pidStruct, q15_t maxOutput){
    pidStruct->maxOutput = maxOutput;
    
    return;
}

void pid_setMinOutput(pidq15_t* pidStruct, q15_t minOutput){
    pidStruct->minOutput = minOutput;
    
    return;
}

void pid_setMaxError(pidq15_t* pidStruct, q15_t maxError){
    pidStruct->maxError = q15_abs(maxError);
    
    return;
}

void pid_setMode(pidq15_t* pidStruct, pidmode_t mode){
    pidStruct->mode = mode;
    
    return;
}

q15_t pid_loop(pidq15_t* pidStruct, q15_t measured, q15_t setpoint){
    q15_t output;
    
    /* determine if the loop is in manual or auto */
    if(pidStruct->mode == ePID_MANUAL){
        output = setpoint;
        pidStruct->iLast = output;
        pidStruct->measLast = measured;
    }else{
        /* error = setpoint - measured */
        q15_t error = q15_add(setpoint, -measured);
        
        /* when the maxError is anything besides the default 'full', then 
         * scale the error value in order to amplify the p gain, effectively
         * getting higher gains than would normally be possible in a Q1.15
         * numeric system */
        if(pidStruct->maxError != 32767){
            error = q15_div(error, pidStruct->maxError);
        }

        /* p = error * pGain */
        q15_t p = q15_mul(error, pidStruct->kp);

        /* i = iLast + (error * ki) */
        q15_t i = q15_add(pidStruct->iLast, q15_mul(error, pidStruct->ki));

        /* d = (measLast - measured) * kd 
         * often this is implemented as d = (error - errorLast) * kd, but
         * this method is equivalent and removes derivative kick when
         * setpoint is suddenly changed */
        q15_t d = q15_mul(pidStruct->kd, q15_add(-measured, pidStruct->measLast));

        /* output = p + i + d */
        output = q15_add(p, i);
        output = q15_add(output, d);

        /* place limits on the output */
        if(output > pidStruct->maxOutput){
            output = pidStruct->maxOutput;
        }else if(output < pidStruct->minOutput){
            output = pidStruct->minOutput;
        }else{
            /* save the last i ONLY if the output is within a proper 
             * range in order to protect from integral windup */
            pidStruct->iLast = i;
        }

    }

    pidStruct->measLast = measured;    
    pidStruct->outLast = output;
    
    return output;
}