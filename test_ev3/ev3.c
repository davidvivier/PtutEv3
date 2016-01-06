
/* from http://icube-avr.unistra.fr/en/index.php/Bytecode_USB_communication_between_RPI_and_Lego_EV3 */

/**
 * Host-side USB direct command example : communication test case
 * 
 * Prerequisite : sudo apt-get install libusb-1.0-0-dev
 * Compilation  : gcc -o usbcmd usbcmd.c -lusb-1.0 -lrt
 * Execution    : sudo ./usbcmd
 * 
 * jacques.gangloff@unistra.fr, 25/9/13
 */


#include "ev3.h"
#include "api.h"

//typedef struct struct ev3_t;

ev3_error_t ev3_init( ev3_t **ev3 )  {
  int status;
  
  status = libusb_init( NULL );
  
  if ( status ) 
    return EV3_USB_ERROR;
  
  *ev3 = calloc( 1, sizeof( **ev3 ) );

  return EV3_OK;
}


ev3_error_t ev3_find_and_open( ev3_t *ev3 )  {
  libusb_device                    *dev, **devs;
  struct libusb_device_descriptor  desc;
  int                              i, status;

  if ( libusb_get_device_list( NULL, &devs ) < 0 )
    return EV3_USB_ERROR;
  
  /* Go through device list loooking for an EV3 device */
  for ( i = 0; ( dev = devs[i] ) != NULL; i++ ) {
    
    status = libusb_get_device_descriptor( dev, &desc );

    if ( status >= 0 ) {
      //printf("device found : idVendor = %x, idProduct = %x\n", desc.idVendor, desc.idProduct);

      if (   ( desc.idVendor == VENDOR_LEGO ) &&
            ( desc.idProduct == PRODUCT_EV3 ) )  {
        
        /* Open the device */
        status = libusb_open( dev, &ev3->hdl );
        if ( status < 0 )  {
          libusb_free_device_list( devs, 1 );
          return EV3_USB_ERROR;
        }
        
        /* Detach possible kernel driver bound to interface */
        libusb_detach_kernel_driver( ev3->hdl, EV3_INTERFACE_NUMBER );
        
        /* Claiming the interface */
        status = libusb_claim_interface( ev3->hdl, EV3_INTERFACE_NUMBER );
        if ( status )  {
          libusb_close( ev3->hdl );
          libusb_free_device_list( devs, 1 );
          return EV3_IN_USE;
        }
        
        libusb_free_device_list( devs, 1 );
        return EV3_OK;
      }
    }
  }
  
  libusb_free_device_list( devs, 1 );
  return EV3_NOT_PRESENT;
}

ev3_error_t ev3_close( ev3_t *ev3 )  {
  
  libusb_release_interface( ev3->hdl, EV3_INTERFACE_NUMBER );
  libusb_close( ev3->hdl );
  libusb_exit( NULL );
  if ( ev3 != NULL )
    free( ev3 );
  ev3 = NULL;

  return EV3_OK;
}

ev3_error_t ev3_send_buf( ev3_t *ev3, char *buf, int len )  {
  int  status, transfered = 0;
  
  status = libusb_bulk_transfer( ev3->hdl, EV3_EP_OUT, buf, len, &transfered, EV3_USB_TIMEOUT );
  
  if ( status  )
    return EV3_USB_WRITE_ERROR;
  
  if ( transfered != len )
    return EV3_USB_PARTIAL_TRANS;

  return EV3_OK;
}

ev3_error_t ev3_recv_buf( ev3_t *ev3, char *buf, int len )  {
  int            i, status, transfered = 0;
  unsigned char  tmpbuf[EV3_PACKET_SIZE];
  
  if ( len > EV3_PACKET_SIZE )
    return EV3_USB_OVERFLOW;
  
  status = libusb_bulk_transfer( ev3->hdl, EV3_EP_IN, tmpbuf, EV3_PACKET_SIZE, &transfered, EV3_USB_TIMEOUT );
  //printf("in recv_buf, status = %d -- TIMEOUT = %d, PIPE = %d, OVERFLOW = %d, NO_DEVICE = %d, ERROR = \n",
  //                  status, LIBUSB_ERROR_TIMEOUT, LIBUSB_ERROR_PIPE, LIBUSB_ERROR_OVERFLOW, LIBUSB_ERROR_NO_DEVICE);

  if ( status  )
    return EV3_USB_WRITE_ERROR;
  
  if ( transfered != EV3_PACKET_SIZE )
    return EV3_USB_PARTIAL_TRANS;
  
  for ( i = 0; i < len; i++ )
    buf[i] = tmpbuf[i];

  return EV3_OK;
}


int sendBytes(struct ev3_t *EV314_hdl, int count, ...) {
  va_list ap;
  int j;

  unsigned char bytes[count];

  va_start(ap, count);
  if (VERBOSE)
    printf(" in sendBytes() :\n");
  for (j = 0; j < count; j++) {
    unsigned char byte = va_arg(ap, int);
    if (VERBOSE)
      printf("  byte[%d]=0x%02x\n", j, byte);
    bytes[j] = byte;
  }
  va_end(ap);

  return ev3_send_buf(EV314_hdl, bytes, count);
}





// void main( void )  {
//   //ev3_t           *ev3 = NULL;
//   ev3 = NULL;
//   ev3_error_t     status;
//   int             i;
//   struct timespec ts1, ts2;
  
//   /* See file "c_com.h" in "ev3sources/lms2012/c_com" (https://github.com/mindboards/ev3sources) */
//   unsigned char   ev3_move_32[] = "\x0C\x00\x00\x00\x00\x04\x00\x3A\x83\x01\x00\x00\x00\x60";
  

//   unsigned char motor_start[] = {0x0C, 0x00,  // command size
//                                  0x00, 0x00,  // message counter (?)
//                                  0x80,  // command type
//                                  0x00, 0x00,    // number of global and local variables
//                                  0xA4, 0x00, 0x01, 0x14, 
//                                  0xA6, 0x00, 0x01};




//   unsigned char   ev3_respons[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00";
  
//   unsigned char motor_stop[] = {0x09, 0x00,
//                                 0x00, 0x00,
//                                 0x80,
//                                 0x00, 0x00,
//                                 0xA3, 0x00, 0x01, 0x00};
//   unsigned char input_read[] = {0x0B, 0x00,
//                                 0x00, 0x00,
//                                 0x00,
//                                 0x01, 0x00,
//                                 0x94, 0x00, 0x01, 0x00, 0x00, 0x60};


//   /* Initialize libusb */
//   status = ev3_init( &ev3 );
//   if ( status != EV3_OK )  {
//     fprintf( stderr, "Libusb initialization error.\n" );
//     exit( -1 );
//   }
    
//   /* Look for an EV3 in USB devices list and open it if found */
//   status = ev3_find_and_open( ev3 );
//   if ( status )  {
//     if ( status == EV3_NOT_PRESENT )
//       fprintf( stderr, "EV3 not found. Is it properly plugged in USB port?\n" );
//     else
//       fprintf( stderr, "Error while scanning for EV3.\n" );
//     exit( -2 );
//   }
    
//   /* Move 32 bit constant to global variable (response buffer) */
//   clock_gettime( CLOCK_MONOTONIC, &ts1 );
//   //ev3_send_buf( ev3, ev3_move_32, sizeof( ev3_move_32 ) - 1 );
//   //ev3_send_buf( ev3, motor_start, sizeof( motor_start ) );
//   ev3_send_buf( ev3, input_read, sizeof( input_read ) -1 );
  
//   //motorStart(1);
//   //motorStop(1);

//   /* Read the response */
//   status = ev3_recv_buf( ev3, ev3_respons, sizeof( ev3_respons ) - 1 );
//   clock_gettime( CLOCK_MONOTONIC, &ts2 );
  
//   /* Check the response */
//   printf( "PC->EV3->PC: %dus\n",
//     (int)( ts2.tv_sec - ts1.tv_sec ) * 1000000 + (int)( ts2.tv_nsec - ts1.tv_nsec ) / 1000 );
//   if ( status != EV3_OK )
//     fprintf( stderr, "EV3 USB read error.\n" );
//   else
//   {
//     /* Display the response */
//     printf( "EV3 answer: " );
//     for ( i = 0; i < sizeof( ev3_respons ) - 1; i++ )
//       printf( "0x%02x ", ev3_respons[i] );
//     printf( "\n" );
//   }
  
//   /* Close EV3 USB port */
//   ev3_close( ev3 );
// }

