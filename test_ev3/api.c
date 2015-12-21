
#include "api.h"


int motors[5];

void f() {
	printf("eflef\n");
}

/*
int motorStart(int motor) {
  return motorStart(motor, 0x14);
                  //  0x14 = 16x1 + 4 = 20
}*/

int motorStart(unsigned char motor, int speed) {
  int success = send(ev3, 14, 0x0C, 0x00,
                              0x00, 0x00,
                              0x80,
                              0x00, 0x00,
                              0xA4, 0x00, motor,  speed,
                              0xA6, 0x00, motor);

  if (success != EV3_OK) {
    return FALSE;
  }
  motors[motor] = TRUE;
  return TRUE;
}



int motorStop(int motor) {
  if (VERBOSE)
    printf(" in motorStop(%d)\n", motor);
  int success = send(ev3, 11, 0x09, 0x00,
                              0x00, 0x00,
                              0x80,
                              0x00, 0x00,
                              opOUTPUT_STOP, 0x00,  motor,  0x00);
                      //                     layer  motor   brake

  if (success != EV3_OK) {
    return FALSE;
  }
  motors[motor] = FALSE;
  return TRUE;
}

int motorSTOP(int motor) {
  while (getMotorSpeed(motor) > 1) {
    motorStop(motor);
  }
  return 0;
}

int isRunning(int motor) {
  if (motor < 1 || motor > 4)
    return FALSE;
  return motors[motor];
}

int stopAllMotors() {
  motorStop(MOTOR_A);
  motorStop(MOTOR_B);
  motorStop(MOTOR_C);
  motorStop(MOTOR_D);
}

/*
1 : forward
-1 : backward
0 : invert polarity
*/
int setMotorPolarity(int motor, int polarity) {
  if (polarity < -1 || polarity > 1)
    return -1;
  int success = send(ev3, 11, 0x09, 0x00,
                              0x00, 0x00,
                              0x80,
                              0x00, 0x00,
                              opOUTPUT_POLARITY, 0x00,  motor,  polarity);
  return success;
}


int forward(int speed) {
  int success;
  // polarités à adapter
  success += setMotorPolarity(MOTOR_A, 1);
  success += setMotorPolarity(MOTOR_D, -1);
  success += motorStart(MOTOR_A, speed);
  success += motorStart(MOTOR_D, speed);
  return success;
}

int backward(int speed) {
  int success;
  success += setMotorPolarity(MOTOR_A, -1);
  success += setMotorPolarity(MOTOR_D, 1);
  success += motorStart(MOTOR_A, speed);
  success += motorStart(MOTOR_D, speed);
  return success;
}

int sensorRead(int port) {
  if (VERBOSE)
    printf(" in sensorRead(%d)\n", port);
  int success = send(ev3, 13, 0x0B, 0x00,
				  			  0x00, 0x00,
				  			  0x00,
				  			  0x01, 0x00,
				  			  0x9A, 0x00, port, 0x00, 0x00, 0x60);

  if (success != 0) {
  	printf("   error in sensorRead(%d) when sending bytes : %d\n", port, success);
  	return -1;
  }
  unsigned char res[13];
  success = ev3_recv_buf( ev3, res, sizeof(res) - 1 );
  if (success != 0) {
  	printf("   error in sensorRead(%d) when receiving bytes : %d\n", port, success);
  }
  int i = 0;
  if (VERBOSE) {
    printf( "EV3 answer: " );
      for ( i = 0; i < sizeof( res ) - 1; i++ )
        printf( "0x%02x ", res[i] );
      printf( "\n" );
  }

  return res[5];
}

/*
Is there a wall within the specified range ?
*/
int wall(int range) {
  if (sensorRead(SENSOR_ULTRASOUND) <= range) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

getMotorPosition(int motor) {
  if (VERBOSE)
      printf(" in getMotorPosition(%d)\n", motor);

  int success = send(ev3, 15,  0x0D,  0x00,
                          0x00, 0x00,
                          0x00,
                          0x01, 0x00,
                          opINPUT_DEVICE, LC0(READY_SI), LC0(0), LC0(16+motor-1), LC0(0), LC0(0), LC0(1), LV0(0));
 if (success != 0) {
    printf("   error in getMotorPosition(%d) when sending bytes : %d\n", motor, success);
    return -1;
  }

  unsigned char res[15];
  success = ev3_recv_buf( ev3, res, sizeof(res) - 1 );
  if (success != 0) {
    printf("   error in getMotorPosition(%d) when receiving bytes : %d\n", motor, success);
  }
  int i = 0;
  if (VERBOSE) {
    printf( "EV3 answer: " );
      for ( i = 0; i < sizeof( res ) - 1; i++ )
        printf( "0x%02x ", res[i] );
      printf( "\n" );
  }

  return 0; 
}

/*

Sets the position of the motor to the wanted position.

parameters :
  motor - the id of the motor
  wantedposition - the wanted position in degrees
  tolerance - the range from wantedPosition within the position is accepted. must be > 0.
*/
int setMotorPosition(int motor, int wantedPosition, int tolerance, int speed) {

  if (VERBOSE) 
    printf("in setMotorPosition(motor=%d, wantedPosition=%d, tolerance=%d, speed=%d) :\n", motor, wantedPosition, tolerance, speed);
  wantedPosition = wantedPosition % 360;

  // à modifier
  //   faire en sorte que le moteur fasse le plus court chemin.
  
  int success;
  int currentPosition = 0;
  while (1) {
    currentPosition = getMotorPosition(motor);
    if (currentPosition < (wantedPosition - tolerance)) {
      success = motorStart(motor, speed);
      if (!success) {
        break;
      }
    }
    else if (currentPosition > (wantedPosition + tolerance)) {
      success = motorStart(motor, speed);
      if (!success)
        break;
    }
    else {
      // position is within the wanted range. Work is done.
      break;
    }

  } 
  printf("  loop exited, stopping motor.\n");
  motorStop(motor);
  return success;
}


int addDegreesToMotor(int motor, int degrees, int tolerance, int speed) {

  setMotorPosition(motor, getMotorPosition(motor) + degrees, tolerance, speed);
  /*
  int startPosition = getMotorPosition(motorStop);
  int targetPosition = startPosition + degrees;

  int currentPosition = getMotorPosition(motor);

  if (degrees < 0)
    setMotorPolarity(motor, -1);

  while (currentPosition < ) {

  }*/

}



int getMotorSpeed(int motor) {

if (VERBOSE)
      printf(" in getMotorSpeed(%d)\n", motor);

  int success = send(ev3, 15,  0x0D,  0x00,
                          0x00, 0x00,
                          0x00,
                          0x01, 0x00,
                          opINPUT_DEVICE, LC0(READY_SI), LC0(0), LC0(16+motor-1), LC0(0), LC0(2), LC0(1), LV0(0));
 if (success != 0) {
    printf("   error in getMotorSpeed(%d) when sending bytes : %d\n", motor, success);
    return -1;
  }

  unsigned char res[15];
  success = ev3_recv_buf( ev3, res, sizeof(res) - 1 );
  if (success != 0) {
    printf("   error in getMotorSpeed(%d) when receiving bytes : %d\n", motor, success);
  }
  int i = 0;
  if (VERBOSE) {
    printf( "EV3 answer: " );
      for ( i = 0; i < sizeof( res ) - 1; i++ )
        printf( "0x%02x ", res[i] );
      printf( "\n" );
  }


  return -1;
}
