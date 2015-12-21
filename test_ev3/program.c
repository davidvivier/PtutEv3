

#include "program.h"
#include "flux.h"

int testSensor(int sensor) {

	int i = 0;

	while (i < 10000) {
		printf("i=%d,value = %d%%\n", i, sensorRead(sensor));
		i++;
	}

}


int run() {
	/*
	printf("RUN !\n");
	
	motorStart(MOTOR_B);

	sleep(2);
	
	motorStop(MOTOR_B);
				//   */	
	
					/*
	int octet = 1;
	while (octet != 0) {
		//octet = listenSocket();
		printf("octet=%d\n", octet);
		if (octet == 3) {
			motorStart(MOTOR_A);
			sleep(1);
			motorStop(MOTOR_A);
		}
		else if (octet == 4) {
			motorStop(MOTOR_A);
		}   
	}     //    */

					// /*
	// test getMotorPosition
	int i = 0, pos = 0;
	while (i < 10000) {
		pos = getMotorPosition(MOTOR_A);
		printf("i=%d pos=%d", i, pos);
	}

	//					/*
	// test getMotorSpeed
	motorStart(MOTOR_A);
	sleep(1);
	getMotorSpeed(MOTOR_A);
	sleep(1);
	getMotorSpeed(MOTOR_A);
	sleep(1);
	motorStop(MOTOR_A);

					//		*/
						/*
	// test MotorSTOP
	motorStart(MOTOR_A);
	sleep(2);
	MotorSTOP(MOTOR_A);
	
					//      */


				/*
	// tests de polarité

	motorStart(MOTOR_A);
	sleep(3);
	motorStop(MOTOR_A);
	sleep(1);
	setMotorPolarity(MOTOR_A, -1);
	motorStart(MOTOR_A);
	sleep(5);
	motorStop(MOTOR_A);
					//    */

						/*
	// tests forward & backward
	forward();
	sleep(4);
	backward();
	sleep(4);
	stopAllMotors();
					//		*/

			//			/*
	// tests setMotorPosition
	setMotorPosition(MOTOR_A, 180, 5, 15);
	sleep(3);
	//setMotorPosition(MOTOR_A, 270, 5, 15);
	//sleep(3);
	//setMotorPosition(MOTOR_A, 0, 5, 15);

					//		*/


	/*
	motorStart(MOTOR_LEFT);

	sleep(2);

	motorStop(MOTOR_LEFT);
	motorStart(MOTOR_RIGHT);

	sleep(2);

	motorStart(MOTOR_HEAD);

	sleep(2);

	motorStop(MOTOR_RIGHT);

	sleep(1);

	motorStop(MOTOR_LEFT);

	motorStop(MOTOR_HEAD);

	*/
	//stopAllMotors();

	//testSensor(SENSOR_1);
	

	return 0;
}
