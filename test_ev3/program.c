

#include "program.h"
//#include "flux.h"
//#include "api.h"

int testSensor(int sensor) {
	int i = 0;

	while (i < 10000) {
		printf("i=%d,value = %d%%\n", i, sensorRead(sensor));
		i++;
	}

}


int robot() {

	int done = 0;
	char octet = 0;
	int i = 0;
	a();
	while (!done) {

		do  {
			octet = listenSocket();
			printf("attente début transmission... octet=%d\n", octet);  
			i++;
			if (i >= 10) {
				printf("Nb d'itérations atteint, arrêt de la boucle.");
				done = 1;
			}
		} while(octet != OCTET_DEBUT_TRANSMISSION && (done == 0));

		// OCTET_DEBUT_TRANSMISSION reçu !
		printf("2-");
		i = 0;
		do {
			octet = listenSocket();
			if (i >= 10) {
				printf("Nb d'itérations atteint, arrêt de la boucle.");
				done = 1;
			}
			else {

				// interprétation de l'octet reçu
				printf("interprétation de octet = %d <=> ", octet);

				switch (octet) {
					case OCTET_STOP:
						printf("OCTET_STOP\n");
						stopAllMotors();
						break;
					case OCTET_AVANCER:
						printf("OCTET_AVANCER\n");
						forward(DEFAULT_SPEED);
						break;
					case OCTET_RECULER:
						printf("OCTET_RECULER\n");
						backward(DEFAULT_SPEED);
						break;

					case OCTET_TOURNER_DROITE:
						printf("OCTET_TOURNER_DROITE\n");

						break;
					case OCTET_TOURNER_GAUCHE:
						printf("OCTET_TOURNER_GAUCHE\n");
						break;

					case OCTET_TETE_FACE:
						printf("OCTET_TETE_FACE\n");
						teteEnFace();
						break;
					case OCTET_TETE_GAUCHE:
						printf("OCTET_TETE_GAUCHE\n");

						break;
					case OCTET_TETE_DROITE:
						printf("OCTET_TETE_DROITE\n");
						break;

					case OCTET_START_CUSTOM_PROGRAM:
						printf("OCTET_START_CUSTOM_PROGRAM\n");
						
						break;
					default:
						printf("Other\n");
						break;
				}

			}
		} while (octet != OCTET_FIN_TRANSMISSION && (done == 0));




		///

		
	}
	return 0;

}



// fonction principale
int run() {

		 
	printf("RUN !\n");

		/*
	motorSetSpeed(1, 15);
	motorSetSpeed(4, 15);

	motorStart(1+4, DEFAULT_SPEED);
	//motorStart(4, DEFAULT_SPEED);
	//motorStart(MOTOR_LEFT | MOTOR_RIGHT, DEFAULT_SPEED);
	sleep(5);
	
	motorStop(15);
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



					 /*
	// test getMotorPosition
	int i = 0, posA = 0, posB = 0, posC = 0;
	int k = 0;
	while (i < 10000) {
		/*
		posA = getMotorPosition(MOTOR_A);
		posB = getMotorPosition(MOTOR_B);
		posC = getMotorPosition(MOTOR_C);
		printf("  - i=%d posA=%d posB=%d posC=%d\n", i, posA, posB, posC);
		*/
		/*for (k = 0; k < 16; k++) {
			printf("  - pos(%d)=%d", k, getMotorPosition(k));
		}
		printf("\n");
		i++;
	}				//   */
 
						/*
	// test getMotorSpeed
	motorStart(MOTOR_A, DEFAULT_SPEED);
	sleep(1);
	getMotorSpeed(MOTOR_A);
	sleep(1);
	getMotorSpeed(MOTOR_A);
	sleep(1);
	motorStop(MOTOR_A);

	while (i < 10000) {

		for (k = 0; k < 16; k++) {
			printf("  - speed(%d)=%d", k, getMotorSpeed(k));
		}
		printf("\n");
		i++;
	}	

					//		*/
						/*
	// test MotorSTOP
	motorStart(MOTOR_A, DEFAULT_SPEED);
	sleep(2);
	motorSTOP(MOTOR_A);
	
					//      */


					/*
	// tests de polarité
	int motor = MOTOR_C;
	setMotorPolarity(motor, 0);
	motorStart(motor, DEFAULT_SPEED);
	sleep(1);
	motorStop(motor);
	sleep(1);
	//setMotorPolarity(MOTOR_A, 0);
	//motorStart(MOTOR_A, DEFAULT_SPEED);
	//sleep(1);
	motorStop(MOTOR_A);
					//    */

						/*
	// tests forward & backward
	forward(DEFAULT_SPEED);
	sleep(4);
	//backward(DEFAULT_SPEED);
	//sleep(4);
	stopAllMotors();
					//		*/

						/*
	// tests setMotorPosition
	setMotorPosition(MOTOR_A, 180, 5, 15);
	sleep(3);
	//setMotorPosition(MOTOR_A, 270, 5, 15);
	//sleep(3);
	//setMotorPosition(MOTOR_A, 0, 5, 15);

					//		*/


	// test envoi octet
	//sendSocket(42);











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
