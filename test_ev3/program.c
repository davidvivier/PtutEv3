

#include "program.h"
//#include "flux.h"
//#include "api.h"

int testSensor(int sensor) {
	int i = 0;

	while (i < 10000) {
		printf("i=%d, s1 = %d, s2 = %d, s3 = %d, s4 = %d\n", i, sensorRead(1), sensorRead(2), sensorRead(3), sensorRead(4));
		//sensorRead(sensor);
		i++;
	}

}


int robot() {


	//motorSetSpeed(5, 40);
	//motorSetSpeed(4, 60);

	int done = 0;
	char octet = 0;
	char port = 0;

	char* octets = NULL;

	int i = 0, j = 0;
	
	
	while (!done) {

		j = 0;
		octets = listenSocket();

		do  {
			
			printf("attente début transmission... j=%d octet=%s\n", j, octets);  
			octet = octets[j];
			j++;
			// if (i >= 256) {
			// 	printf("Nb d'itérations atteint, arrêt de la boucle.");
			// 	done = 1;
			// }
			} while(octet != OCTET_DEBUT_TRANSMISSION && (j < 100));
			if (j == 100) {
				printf("erreur : pas d'octet de début de transmission reçu.\n");
				continue;
			}
			// OCTET_DEBUT_TRANSMISSION reçu !
			printf("2-");
			i = j;
			do {
				//octets = listenSocket();
				//if (i >= 10) {
				//	printf("Nb d'itérations atteint, arrêt de la boucle.");
				//	done = 1;
				//}
				//else {

				octet = octets[i];


				// interprétation de l'octet reçu
				printf("interprétation de octet %d = %d <=> ", i, octet);

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
					case OCTET_START_MOTOR:
						printf("OCTET_START_MOTOR\n");

						i++;

						port = octets[i];
						printf(" - motor 0x%02x", port);
						if (port == MOTOR_HEAD) {
							printf("MOTOR_HEAD : vitesse reduite\n");
						}
						motorSetSpeed(MOTOR_HEAD, 5);
						motorStart(port);

						break;
					case OCTET_STOP_MOTOR:
						printf("OCTET_STOP_MOTOR\n"); 

						i++;

						port = octets[i];
						printf(" - motor 0x%02x", port);

						motorStop(port);

						break;
					case OCTET_INVERT_POLARITY:
						printf("OCTET_INVERT_POLARITY\n"); 

						i++;

						port = octets[i];
						printf(" - motor 0x%02x", port);

						setMotorPolarity(port, 0);

						break;
					case OCTET_SET_VITESSE:
						printf("OCTET_SET_VITESSE\n"); 

						i++;

						port = octets[i];

						i++;

						char speed = octets[i];

						printf(" - motor 0x%02x - speed = 0x%02x", port, speed);

						setMotorPolarity(port, 0);

						break;
					case OCTET_DEMANDE_MSG:
						printf("OCTET_DEMANDE_MSG");
						break;
					case OCTET_START_CUSTOM_PROGRAM:
						printf("OCTET_START_CUSTOM_PROGRAM\n");
						programmetest();
						break;
					case OCTET_FIN_TRANSMISSION:
						printf("OCTET_FIN_TRANSMISSION\n");
						
						break;
					default:
						printf("Other\n");
						break;
				}

			//}
			i++;
			if (i > 10) {
				//done = 1;
			}
		} while (octet != OCTET_FIN_TRANSMISSION && i < 16);




		///

		
	}
	return 0;

}




void programmetest () {
    time_t t1,t0;
	int seuilCapteur = 5;
	motorSetSpeed(15, 40);
	motorStart(MOTOR_LEFT | MOTOR_RIGHT);//left 0001 right 0100
	t0=clock();// premier tick
	int diff = 0;
	int seconds = 0;
	do {
		int distance = distanceCapteur(1);
		printf("seuil=%d capteur=%d ", seuilCapteur, distance);
	   if (distance <= seuilCapteur) {
		   motorStop(MOTOR_LEFT | MOTOR_RIGHT);
		   invertMotorPolarity(MOTOR_LEFT);
		   motorStart(MOTOR_LEFT | MOTOR_RIGHT );
		   sleep(1);
		   motorStop(MOTOR_LEFT | MOTOR_RIGHT);
		   invertMotorPolarity(MOTOR_LEFT);
		   motorStart(MOTOR_LEFT | MOTOR_RIGHT);
	   }
	 t1=clock();
	 diff = t1-t0;
	 seconds = diff/500000;	
	 printf("t0=%d t1=%d diff = %d - %d seconds\n ", t0, t1, diff, seconds);
	} while(seconds<10); // (t1-t0)/CLOCKS_PER_SEC nombre de seconde
	
}


// fonction principale
int run() {

		 
	printf("RUN !\n");
	//stopAllMotors();
	//programmetest();

	//testSensor(1);

		robot();
	 				/*
	
	int a = 10000;
	printf("facto(%d)=%d\n", a, facto(a));
	int i = 0;
	motorSetSpeed(MOTOR_LEFT, 15);
	for (i = 0; i < 10; i++) {
		turnTU(MOTOR_LEFT);
	}     //   */

		/*
	motorSetSpeed(4, 15);
	//motorSetSpeed(4, 15);
	motorStop(15);
	motorStart(1+4);
	//motorStart(4);
	//motorStart(MOTOR_LEFT | MOTOR_RIGHT, DEFAULT_SPEED);
	sleep(1);
	
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
	while (i < 1000) {
		//  /*
		posA = getMotorPosition(MOTOR_A);
		posB = getMotorPosition(MOTOR_B);
		posC = getMotorPosition(MOTOR_C);
		printf("  - i=%d posA=%d posB=%d posC=%d\n", i, posA, posB, posC);
			}
		   //   */
		    /*
		int sum = 0;
		for (k = 1; k < 25; k++) {
			printf("k=%d ", k);
			//int pos = getMotorPosition(k);
			int pos = sensorRead(k);
			sum += pos;
			printf("  - pos(%d)=%d,", k, pos);
		}
		printf(" - somme = %d\n", sum);
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
	stopAllMotors();

	//testSensor(SENSOR_1);
	

	return 0;
}

