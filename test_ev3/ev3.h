
#ifndef INCLUDED_EV3_H
#define INCLUDED_EV3_H
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>

#define EV3_USB_TIMEOUT             2000   // Milliseconds

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





typedef unsigned char               ev3_error_t;
typedef struct ev3_t                ev3_t;



struct ev3_t {
  struct libusb_device_handle       *hdl;
};


enum ev3_usb_ids {
  VENDOR_LEGO   =                   0x0694,
  PRODUCT_EV3   =                   0x0005
};




ev3_t* ev3;

ev3_error_t ev3_init( ev3_t **ev3 );


ev3_error_t ev3_find_and_open( ev3_t *ev3 );

ev3_error_t ev3_close( ev3_t *ev3 );

ev3_error_t ev3_send_buf( ev3_t *ev3, char *buf, int len );

ev3_error_t ev3_recv_buf( ev3_t *ev3, char *buf, int len );


int sendBytes(struct ev3_t *EV314_hdl, int count, ...);




// from https://github.com/mindboards/ev3sources/blob/master/lms2012/c_com/source/c_com.h

#include  <sys/types.h>
#include <dirent.h>

#include "lmstypes.h"



typedef   UWORD     CMDSIZE;
typedef   UWORD     MSGCNT;


typedef   struct                        //!< Common command struct
{
  CMDSIZE CmdSize;
  MSGCNT  MsgCnt;
  UBYTE   Cmd;
  UBYTE   PayLoad[];                    //!< Pay load is DIRCMD or SYSCMD
}
COMCMD;



typedef   struct                        //!< Direct command struct
{
  UBYTE   Globals;
  UBYTE   Locals;
  UBYTE   Code[];
}
DIRCMD;





#endif