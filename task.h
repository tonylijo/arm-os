#ifndef __TASK_H
#define __TASK_H

typedef struct {
  unsigned int regs[16];
  unsigned int cpsr;
  int tid;
} TCB;

#define SAVE_CONTEXT() asm volatile("subs lr,lr,#4\n\t" \
                                    "stmfd sp!,{lr}\n\t"\
				    "stmfd sp!,{r0} \n\t"\
				    "mrs r0,cpsr\n\t"\
				    "orr r0,r0,#0xc0\n\t"\
				    "msr cpsr_c,r0\n\t"\
				    "ldmfd sp!,{r0} \n\t"\
				    "ldr lr,=current_task \n\t"\
				    "ldr lr,[lr] \n\t"\
				    "stmia lr,{r0-r14}^ \n\t"\
				    "add lr,lr,#60\n\t"\
				    "ldmfd sp!,{r0}\n\t"\
				    "stmia lr!,{r0} \n\t"\
				    "mrs r0,spsr\n\t"\
				    "str r0,[lr,#0] \n\t"\
				    )

#define RESTORE_CONTEXT()     asm volatile("ldr lr,=current_task\n\t"\
                                           "ldr lr,[lr] \n\t"\
                                           "ldr r0,[lr,#64]\n\t"\
                                           "msr spsr,r0\n\t"\
					   "ldmia lr,{r0-r14}^\n\t"\
					   "nop\n\t"\
					   "nop \n\t"\
					   "nop \n\t"\
					   "ldr lr,[lr,#60] \n\t"\
					   "subs pc,lr,#0\n\t"\
                                           )
#define MAX_TASKS 10
#endif 
