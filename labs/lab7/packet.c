#include <os_cpu.h>
#include <os_cfg.h>
#include <ucos_ii.h> /* os_c??.h must be included before ! */
#include <libepc.h>

#include "inbound.h"
#include "elapsed.h"
#include "serial.h"
#include "count.h"
#include <ctype.h>

static OS_EVENT* SendLock = NULL;

void ReceivePackets(void)
{
  SerialInit() ;

  char ccstring[5];
  int char_count = 0;
  
  for (;;)
  {
    int type, byte, bytes ;
    BYTE8 *bfr ;
    
    if (SerialGet() != 0xFF)
      continue ;
    
    switch (type = SerialGet())
      {
      default:
        continue ;
      case 1:
      case 2:
      case 3:
        break ;
      }
    
    bytes = SerialGet();
    bfr = (BYTE8 *) malloc(bytes) ;
    if (!bfr)
    {
      for (;;)
        ;
    }

    if (type == 1)
      char_count += bytes;
    
    for (byte = 0; byte < bytes; byte++)
      {
	bfr[byte] = SerialGet() ;
      }
    switch (type)
      {
      case 1:
        PostText(bfr) ;
	FormatUnsigned(ccstring, char_count, 10, 5, ' ');
	SendPacket(3,ccstring,5);
        break ;
      case 2:
        PostTime(bfr) ;
        break ;
      case 3:
        PostCount(bfr);
	break;
      }
  }
}

void SendPacket(int type, BYTE8 *bfr, int bytes)
{
  int i = 0;

  if (!SendLock)
    SendLock = OSSemCreate(1);

  OSSemPend(SendLock, 0, NULL);

  //header
  SerialPut((BYTE8)0xFF);
  SerialPut((BYTE8)type);
  SerialPut((BYTE8)bytes);

  //data
  for (; i < bytes; ++i ) {
    SerialPut((BYTE8)bfr[i]);
  }

  OSSemPost(SendLock);
}

