/**
 * Host-side USB direct command example : communication test case
 * 
 * Prerequisite : sudo apt-get install libusb-1.0-0-dev
 * Compilation  : gcc -o usbcmd usbcmd.c -lusb-1.0 -lrt
 * Execution    : sudo ./usbcmd
 * 
 * jacques.gangloff@unistra.fr, 25/9/13
 */
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>

#define EV3_USB_TIMEOUT             2000    // Milliseconds

/* These informations can be obtained with lsusb -v */
#define EV3_INTERFACE_NUMBER        0
#define EV3_CONFIGURATION_NB        1
#define EV3_EP_OUT                  0x01
#define EV3_EP_IN                   0x81
#define EV3_PACKET_SIZE             0x400

/* Error codes */
#define EV3_OK                      0
#define EV3_USB_ERROR               1
#define EV3_NOT_PRESENT             2
#define EV3_CONFIGURATION_ERROR     3
#define EV3_IN_USE                  4
#define EV3_USB_WRITE_ERROR         5
#define EV3_USB_READ_ERROR          6
#define EV3_USB_PARTIAL_TRANS       7
#define EV3_USB_OVERFLOW            9


enum ev3_usb_ids {
  VENDOR_LEGO   =                   0x0694,
  PRODUCT_EV3   =                   0x0005
};

struct ev3_t {
  struct libusb_device_handle       *hdl;
};

typedef unsigned char               ev3_error_t;
typedef struct ev3_t                ev3_t;


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

  if ( status  )
    return EV3_USB_WRITE_ERROR;
  
  if ( transfered != EV3_PACKET_SIZE )
    return EV3_USB_PARTIAL_TRANS;
  
  for ( i = 0; i < len; i++ )
    buf[i] = tmpbuf[i];

  return EV3_OK;
}

void main( void )  {
  ev3_t           *ev3 = NULL;
  ev3_error_t     status;
  int             i;
  struct timespec ts1, ts2;
  
  /* See file "c_com.h" in "ev3sources/lms2012/c_com" (https://github.com/mindboards/ev3sources) */
  unsigned char   ev3_move_32[] = "\x0C\x00\x00\x00\x00\x04\x00\x3A\x83\x01\x00\x00\x00\x60";
  unsigned char   ev3_respons[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00";
  
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
      fprintf( stderr, "Error while scanning for EV3.\n" );
    exit( -2 );
  }
    
  /* Move 32 bit constant to global variable (response buffer) */
  clock_gettime( CLOCK_MONOTONIC, &ts1 );
  ev3_send_buf( ev3, ev3_move_32, sizeof( ev3_move_32 ) - 1 );
  
  /* Read the response */
  status = ev3_recv_buf( ev3, ev3_respons, sizeof( ev3_respons ) - 1 );
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
    for ( i = 0; i < sizeof( ev3_respons ) - 1; i++ )
      printf( "0x%02x ", ev3_respons[i] );
    printf( "\n" );
  }
  
  /* Close EV3 USB port */
  ev3_close( ev3 );
}
