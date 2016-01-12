
#ifndef INCLUDED_API_H
#define INCLUDED_API_H

#include "ev3.h"

#include "bytecodes.h"

#define TRUE 1
#define FALSE 0

#define MOTOR_A 1
#define MOTOR_B 2
#define MOTOR_C 4
#define MOTOR_D 8


#define MOTOR_LEFT MOTOR_A
#define MOTOR_RIGHT MOTOR_C

#define MOTOR_HEAD MOTOR_B


//#define USE_EV3


#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 4
#define SENSOR_4 8

#define SENSOR_ULTRASOUND SENSOR_2


// youenn
#define OCTET_DEBUT_TRANSMISSION 1
#define OCTET_FIN_TRANSMISSION 127

#define OCTET_ERREUR 4

// youenn
#define OCTET_STOP 100 // stop moteurs

// youenn
#define OCTET_AVANCER 10
#define OCTET_RECULER 20


#define OCTET_START_MOTOR 90	
							// + motor + speed
#define OCTET_STOP_MOTOR 91

// youenn
#define OCTET_TOURNER_DROITE 40
#define OCTET_TOURNER_GAUCHE 30

#define OCTET_SET_VITESSE 50 

#define OCTET_INVERT_POLARITY 60

#define OCTET_TETE_FACE 81
#define OCTET_TETE_GAUCHE 82
#define OCTET_TETE_DROITE 83


#define OCTET_DEMANDE_MSG 3


#define OCTET_START_CUSTOM_PROGRAM 70




#define VERBOSE FALSE

#define SIMULATION FALSE

#define DEFAULT_SPEED 0x40

#define motorSpeed(motor) motorSpeed(motor, DEFAULT_SPEED)
//#define forward(void) forward(DEFAULT_SPEED)
//#define backward(void) backward(DEFAULT_SPEED)

/*
int motor_1;
int motor_2;
int motor_3;
int motor_4;
*/

int motorStart(unsigned char motor);

int motorSetSpeed(unsigned char motor, int speed);


int motorStop(int motor);
int stopAllMotors();

int invertMotorPolarity(int motor);

int distanceCapteur();




int motorSTOP(int motor);
int isRunning(int motor);


int setMotorPolarity(int motor, int polarity);

int forward();
int backward();

int tournerGauche(int speed);
int tournerDroite(int speed);

int teteEnFace();

int sensorRead(int port);

int getMotorPosition(int motor);
int setMotorPosition(int motor, int wantedPosition, int tolerance, int speed);
int addDegreesToMotor(int motor, int degrees, int tolerance, int speed);
int getMotorSpeed(int motor);


int wall(int range);
//#define wall() wall(DEFAULT_SPEED)
//int wall();


void turnTU(int motor);

#endif