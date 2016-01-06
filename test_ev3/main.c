

#include "ev3.h"
#include "api.h"
#include "program.h"


int main( void )  {

  robot();
  return 0;
  //ev3_t           *ev3 = NULL;
  ev3 = NULL;
  ev3_error_t     status;
  int             i;
  struct timespec ts1, ts2;
  
  /* See file "c_com.h" in "ev3sources/lms2012/c_com" (https://github.com/mindboards/ev3sources) */
  unsigned char   ev3_move_32[] = "\x0C\x00\x00\x00\x00\x04\x00\x3A\x83\x01\x00\x00\x00\x60";
  

  unsigned char motor_start[] = {0x0C, 0x00,  // command size
                                 0x00, 0x00,  // message counter (?)
                                 0x80,  // command type
                                 0x00, 0x00,    // number of global and local variables
                                 0xA4, 0x00, 0x01, 0x14, 
                                 0xA6, 0x00, 0x01};




  unsigned char   ev3_respons[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00";
  
  unsigned char motor_stop[] = {0x09, 0x00,
                                0x00, 0x00,
                                0x80,
                                0x00, 0x00,
                                0xA3, 0x00, 0x01, 0x00};
  unsigned char input_read[] = {0x0B, 0x00, // command size
                                0x00, 0x00, // message counter
                                0x00,       // command type
                                0x01, 0x00, // variables
                                0x9A, 0x00,  0x01, 0x00, 0x00, 0x60   };
                          //    cmd   layer  port  type  mode  res (%)
                        //      0x94, 0x00, 0x01, 0x00, 0x00, 0x60};

  unsigned char input_read_si[] = {0x0A, 0x00, // command size
                                   0x00, 0x00, // message counter
                                   0x00,       // command type
                                   0x01, 0x00, // variables
                                   0x9D, 0x00,  0x01, 0x00, 0x00 };
                          //       cmd   layer  port  type  mode  res (%)}

  unsigned char get_raw[] = {0x0B, 0x00, // command size
                             0x00, 0x00, // message counter
                             0x00,       // command type
                             0x01, 0x00, // variables
                             11, 0x00,  0x01, 0x00, 0x00, 0x00, 0x00  };
                    //       cmd      layer  port  type  }




  /* Initialize libusb */ 
  status = ev3_init( &ev3 );
  if ( status != EV3_OK )  {
    fprintf( stderr, "Libusb initialization error.\n" );
    exit( -1 );
  }
  

  #ifdef USE_EV3

  /* Look for an EV3 in USB devices list and open it if found */
  status = ev3_find_and_open( ev3 );
  if ( status )  {
    if ( status == EV3_NOT_PRESENT )
      fprintf( stderr, "EV3 not found. Is it properly plugged in USB port?\n" );
    else
      fprintf( stderr, "Error while scanning for EV3. (try with sudo)\n" );
    exit( -2 );
  }

  #endif
    
  /* Move 32 bit constant to global variable (response buffer) */
  clock_gettime( CLOCK_MONOTONIC, &ts1 );
  //ev3_send_buf( ev3, ev3_move_32, sizeof( ev3_move_32 ) - 1 );
  //ev3_send_buf( ev3, motor_start, sizeof( motor_start ) );
  //ev3_send_buf( ev3, input_read, sizeof( input_read ) -1 );
  //ev3_send_buf( ev3, input_read_si, sizeof( input_read_si ) -1 );
  //ev3_send_buf( ev3, get_raw, sizeof( get_raw ) -1 );

  //motorStart(2);
  //motorStop(2);

  run();

  //int r = sensorRead(1);
  //  printf("r=%d\n", r);
  /* Read the response */
  //status = ev3_recv_buf( ev3, input_read, sizeof( input_read) - 1 );
  clock_gettime( CLOCK_MONOTONIC, &ts2 );
  
  /* Check the response */
  printf( "PC->EV3->PC: %dus\n",
    (int)( ts2.tv_sec - ts1.tv_sec ) * 1000000 + (int)( ts2.tv_nsec - ts1.tv_nsec ) / 1000 );
  if ( status != EV3_OK )
    fprintf( stderr, "EV3 USB read error.\n" );
  else
  {
    /* Display the response */
    printf( "EV3 answer: " );
    for ( i = 0; i < sizeof( input_read ) - 1; i++ )
      printf( "0x%02x ", input_read  [i] );
    printf( "\n" );
  }
  
  /* Close EV3 USB port */
  ev3_close( ev3 );
}
