#include"task.h"
extern volatile TCB task_array[MAX_TASKS];
extern volatile unsigned int current_tid ;
extern volatile TCB *current_task ;
extern volatile unsigned int current_max_task ;

void init_tasks(void) 
{
  int i;
  int j;
  for(i = 0; i < MAX_TASKS; i++) {
    for(j = 0; j < 15; j++ ) {
      task_array[i].regs[j] = 0;
    }
    task_array[i].tid = -1;

  }
  task_array[0].tid = 0;
}

void task_create(void (*fun) (void)) 
{
  task_array[current_max_task].regs[13] = (unsigned int)(0x400076d0 - (current_max_task * 40));
  task_array[current_max_task].regs[14] = (unsigned int) fun;
  task_array[current_max_task].regs[15] = (unsigned int) fun;
  task_array[current_max_task].cpsr = 0x2000001f;
  task_array[current_max_task].tid = current_max_task++;
}

void scheduler() 
{
    current_tid++;
    if(current_tid == (current_max_task+1)) {
      current_tid = 0;
    }
    current_task = &(task_array[current_tid]);
}
