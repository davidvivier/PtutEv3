
#ifndef INCLUDED_API_H
#define INCLUDED_API_H

#include "ev3.h"

#include "bytecodes.h"

#define TRUE 1
#define FALSE 0

#define MOTOR_A 1
#define MOTOR_B 2
#define MOTOR_C 3
#define MOTOR_D 4


#define MOTOR_LEFT MOTOR_A
#define MOTOR_RIGHT MOTOR_D

#define MOTOR_HEAD MOTOR_B



#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 3
#define SENSOR_4 4

#define SENSOR_ULTRASOUND SENSOR_2

#define VERBOSE TRUE

#define DEFAULT_SPEED 0x14

#define motorSpeed(motor) motorSpeed(motor, DEFAULT_SPEED)
//#define forward(void) forward(DEFAULT_SPEED)
//#define backward(void) backward(DEFAULT_SPEED)

/*
int motor_1;
int motor_2;
int motor_3;
int motor_4;
*/

int wall(int range);
int wall();

#endif