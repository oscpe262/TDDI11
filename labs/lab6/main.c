#include <mtc.h>
#include <dos.h>
#include <libepc.h>

#include "serial.h"

/* Function prototypes for the threads */
void OutBound(void) ;
void InBound(void) ;
void DisplayElapsedTime(void) ;

PRIVATE void SerialInit(void) ;

QUEUE *inbound_queue ;

int main()
{
  inbound_queue = QueueCreate(sizeof(char), 20) ;
  
  ClearScreen(0x07) ;
  SetCursorVisible(TRUE) ;
  SerialInit() ;
  
  MtCCoroutine(OutBound()) ;
  MtCCoroutine(InBound()) ;
  MtCCoroutine(DisplayElapsedTime());
  
  return 0 ;
}

PRIVATE void SerialInit(void)
{
  /* Disable interrupts during initialization */
  disable() ;
  
  /* 9600 baud */
  outportb(SER_LCR, SER_LCR_DLAB) ;
  outportb(SER_DLO, 12) ;
  outportb(SER_DHI, 0) ;
  
  /* 8 data bits, even parity, 1 stop bit */
  outportb(SER_LCR, 0x1B) ;
  
  /* Enable only receiver data ready interrupts */
  outportb(SER_IER, 0x01) ;
  
  /* Request to send, data terminal ready, enable interrupts */
  outportb(SER_MCR, SER_MCR_RTS|SER_MCR_DTR|SER_MCR_OUT2) ;
  
  /* Store address of ISR in IDT */
  SetISR(IRQ2INT(SER_IRQ), SerialISR) ;
  
  /* Unmask thogle.com/e UART's IRQ line */
  outportb(0x21, inportb(0x21) & ~SER_MSK) ;
  
  /* Re-enable interrupts */
  enable() ;
}

void DisplayElapsedTime(void)
{

  DWORD32 cTime;
  WORD16 seconds;
  WORD16 minutes;
  WORD16 hours;
  WINDOW *w;

  w = WindowCreate("Session duration", 22, 24, 0, 79);

  if (!w)
    return;
  
  for ( ; ; ) {
    
    cTime = Milliseconds();
    seconds = cTime/1000;
    minutes = seconds/60;
    hours = minutes/60;
      
    SetCursorPosition(23,36);
    PutUnsigned(hours%24, 10, 2);
    PutString(":");
    PutUnsigned(minutes%60, 10, 2);
    PutString(":");
    PutUnsigned(seconds%60, 10, 2);

    cTime = Milliseconds()+999;
    while (Now_Plus(0) < cTime)
      MtCYield();
  }
} 
