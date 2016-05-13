#include <os_cpu.h>
#include <os_cfg.h>
#include <ucos_ii.h> /* os_c??.h must be included before ! */
#include <libepc.h>

#include "inbound.h"
#include "elapsed.h"
#include "serial.h"

static OS_EVENT* SendLock = NULL;
int gchar = 0;

void ReceivePackets(void)
{
  SerialInit() ;
  
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
        break ;
    }
    
    bytes = SerialGet();
    bfr = (BYTE8 *) malloc(bytes) ;
    if (!bfr)
    {
      for (;;)
        ;
    }
    
    for (byte = 0; byte < bytes; byte++)
    {
      bfr[byte] = SerialGet() ;
      gchar++; //sdasdasd
    }
    switch (type)
      {
      case 1:
        PostText(bfr) ;
        break ;
      case 2:
        PostTime(bfr) ;
        break ;
      case 3: //adsadas
	PostCC(gchar);
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

void InboundChars(void)
{
  WINDOW *w ;
  
  w = (WINDOW *) WindowCreate("Chars Rec.", 11, 13, 30, 48 ) ;
  if (!w)
    return ;
  
  for (;;)
  {   
    WindowSetCursor(w, 0, 1) ;
    WindowPutString(w, gchar) ;
  }
}

void PostCC(int i)
{
  
}
