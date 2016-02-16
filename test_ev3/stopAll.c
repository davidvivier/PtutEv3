


#include "ev3.h"
#include "api.h"


int main () {
	ev3 = NULL;
	ev3_error_t     status;
	int i;	
	


  /* Initialize libusb */ 
  status = ev3_init( &ev3 );
  if ( status != EV3_OK )  {
    fprintf( stderr, "Libusb initialization error.\n" );
    exit( -1 );
  }
  

  

  /* Look for an EV3 in USB devices list and open it if found */
  status = ev3_find_and_open( ev3 );
  if ( status )  {
    if ( status == EV3_NOT_PRESENT )
      fprintf( stderr, "EV3 not found. Is it properly plugged in USB port?\n" );
    else
      fprintf( stderr, "Error while scanning for EV3. (try with sudo)\n" );
    exit( -2 );
  }


	int stopAllMotors();


    ev3_close( ev3 );


return 0;
}
