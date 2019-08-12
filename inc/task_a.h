#ifndef TASK_A_H
#define TASK_A_H

#include "tn.h"

//-- stack sizes of user tasks
#define TASK_A_STK_SIZE    (TN_MIN_STACK_SIZE + 96)

//-- user task priorities
#define TASK_A_PRIORITY    7

//-- Allocate arrays for stacks: stack for idle task
//   and for interrupts are the requirement of the kernel;
//   others are application-dependent.
//
//   We use convenience macro TN_STACK_ARR_DEF() for that.
TN_STACK_ARR_DEF(task_a_stack, TASK_A_STK_SIZE);

//-- task structures
struct TN_Task task_a;

void task_a_body(void *par);
void appl_init(void);

#endif
