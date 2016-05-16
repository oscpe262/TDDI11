#include <os_cpu.h>
#include <os_cfg.h>
#include <ucos_ii.h>
#include <libepc.h>
#include <dos.h>

PRIVATE OS_EVENT *count_queue = NULL;

PRIVATE void CountCreateQ(void)
{
  if (!count_queue) {
    static void *count_q[20];
    count_queue = OSQCreate(count_q, ENTRIES(count_q));
  }
}

void PostCount(BYTE8 *bfr)
{
  CountCreateQ();
  OSQPost(count_queue, bfr);
}

void InboundCount(void)
{
  WINDOW *cc;

  cc = WindowCreate("Bytes Received", 11, 13, 30, 50); 
  CountCreateQ();

  BYTE8 err;
  char *bfr;

  for (;;) {
    bfr = OSQPend(count_queue, 0, &err);
    WindowSetCursor(cc, 0, 6);
    WindowPutString(cc, bfr);
    free(bfr);
  }

}
