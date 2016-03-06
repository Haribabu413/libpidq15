# Purpose #

This PID library is intended for use on 8 and 16 bit microcontrollers which have limited cycles in which to perform multiplication and division operations.

The library utilizes pointers which allows the software writer the ability to easily run multiple control loops on different processes using the same code base.

# Libraries #

This library uses the [libmathq15](https://github.com/slightlynybbled/libmathq15) library.

# Usage #

## Minimum Implementation ##

1. Include this library and the [libmathq15](https://github.com/slightlynybbled/libmathq15) library into your project.
2. Declare a 'pidStruct' type variable for the item you wish to control.
3. Use 'pid_init()' to initialize the structure
4. Repeatedly call 'pid_loop()' with updated 'measured' and 'setpoint' data at regular intervals.  The interval is to be set based on application requirements.

## Options ##

If you wish to change the gains, use the 'pid_setGains()' function

To change the maximum or minimum output value, use 'setMaxOutput()' and/or 'setMinOutput()'

If you find that the proportional gain is at max (32767) and you still wish to have more proportional gain, then use 'pid_setMaxError()'.  This will amplify the proportional gain significantly.  Try halving the value of kp and the max error.

If you wish to change the mode of the loop from closed-loop to open loop, or open loop back to closed loop, use the 'pid_setMode()' function.

## Tuning ##

1. Set kp to 1000, ki and kd to 0.  The kp value will almost assuredly be too low.
2. Increase kp in reasonable intervals - double works for me - until the application responds quickly and adequately to setpoint changes.  This is a judgement call, but there should be no oscillation.
3. Slowly increase ki in relatively small steps.  I like to start at a value of '10' and double that value for each check.  Select a value that allows the controlled variable to move to zero error without oscillation.

Up to this point, you have a working PI loop.  You probably don't need the kd term, but if you do, there are so many different opinions and it adds a level of complexity to tuning that I simply do not wish to tackle at this point.  I will leave tuning kd to the reader to educate him/herself.


