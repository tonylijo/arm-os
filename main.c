#include "LPC214x.h"
#include "task.h"
#include "VIClowlevel.h"
#define PLOCK 0x400

volatile TCB task_array[MAX_TASKS];
volatile unsigned int current_tid = 0;
volatile TCB *current_task = &task_array[0];
volatile unsigned int current_max_task = 1;
extern void set_timer_int_init();

void init(void);
void task1()
{  
  int i;
  while(1) {
    for(i = 0; i < 1000000; i++);
    IOSET0 ^= 0x00000100;
    for(i = 0; i < 1000000; i++);
    IOCLR0 ^= 0x00000100;
  }
}

void task2()
{ 
  int i;
  while(1) {
    for(i = 0; i < 1000000; i++);
    IOSET0 ^= 0x00000200;
    for(i = 0; i < 1000000; i++);
    IOCLR0 ^= 0x00000200;
  }
}
void task3()
{ 
  int i;
  while(1) {
    for(i = 0; i < 1000000; i++);
    IOSET0 ^= 0x00000400;
    for(i = 0; i < 1000000; i++);
    IOCLR0 ^= 0x00000400;
  }
}

void task4()
{ 
  int i;
  while(1) {
    for(i = 0; i < 1000000; i++);
    IOSET0 ^= 0x00000800;
    for(i = 0; i < 1000000; i++);
    IOCLR0 ^= 0x00000800;
  }
}

void init(void)
{
    PLLCFG=0x24;                //set multiplier/divider values
    PLLFEED=0xaa;
    PLLFEED=0x55;
    PLLCON=0x01;                //enable PLL
    PLLFEED=0xaa;
    PLLFEED=0x55;
    while(!(PLLSTAT & PLOCK));  //wait for the PLL to lock to set frequency
    PLLCON=0x3;                 //connect the PLL as the clock source
    PLLFEED=0xaa;
    PLLFEED=0x55;
    MAMCR=0x02;                 //enable MAM
    MAMTIM=0x04;                //set number of clocks for flash memory fetch
    VPBDIV=0x01;                //set peripheral clock(pclk) to system clock(cclk)
}

int main(void)
{
    int i;
    IODIR0 = 0x00001f00;
    IOCLR0 = 0x00001f00;                    //LED off
    PINSEL0 = 0x80000000;
    init();

    task_create(task2);
    task_create(task1);
    task_create(task3);
    task_create(task4);
    set_timer_int_init();
    enableIRQ();
    while(1){
      for(i = 0; i < 1000000; i++);
      IOSET0 ^= 0x00001f00;
      for(i = 0; i < 1000000; i++);
      IOCLR0  ^= 0x00001f00;
    }
}
