#include "LPC214x.h"
#include "task.h"
void timer0_isr(void)   __attribute__ ((naked));
void FIQ_Routine(void)   __attribute__ ((interrupt("FIQ")));
void SWI_Routine(void)   __attribute__ ((interrupt("SWI")));
void UNDEF_Routine(void) __attribute__ ((interrupt("UNDEF")));


/*initialise timer zero*/
void set_timer_int_init()
{
  T0TCR = 0x02;                        //reset counter
  T0IR = 0xFF;                         
  T0MCR = 0x0003;                      //interrupt and reset on MR0
  T0MR0 = 0x10000000;
  //T0MR0 = 0x03938700;                    //comparet-hit count

  //  VICIntSelect &= ~(TMR0_Channel_Mask);
  VICVectCntl0 = 0x00000024;          //use it for Timer 0 Interrupt.
  VICVectAddr0 = (unsigned) timer0_isr; //set interrupt vector in 0
  VICIntEnable = 0x00000010;            //enable TIMER0 interrupt
  T0TCR = 0x01;                         //enable Timer0
}

/*timer0_isr: interept service routine to service timer interrupt*/
void timer0_isr(void)
{
  SAVE_CONTEXT();              //saves the context of the currently running task
  scheduler();                 //scheduling algorithm -round robin hear.
  T0IR = 0x01;                //clear interrupt.
  VICVectAddr = 0;            //end of interrupt - dummy write.
  RESTORE_CONTEXT();          //restore the context of the next task.
}

/*fast interrupt service routine 
  will go into a infinate loop if 
  it occures*/
void FIQ_Routine(void) 
{
    while (1); 
}

/*
TODO:
should implement later 
used to get privilage of certain procedures.
*/
void SWI_Routine(void)
{
    while (1); 
}

void UNDEF_Routine(void) 
{
	while (1);	
}
