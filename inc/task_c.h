#ifndef TASK_C_H
#define TASK_C_H

#include "tn.h"

//-- stack sizes of user tasks
#define TASK_C_STK_SIZE    (TN_MIN_STACK_SIZE + 96)

//-- user task priorities
#define TASK_C_PRIORITY    5

//-- Allocate arrays for stacks: stack for idle task
//   and for interrupts are the requirement of the kernel;
//   others are application-dependent.
//
//   We use convenience macro TN_STACK_ARR_DEF() for that.
TN_STACK_ARR_DEF(task_c_stack, TASK_C_STK_SIZE);

//-- task structures
struct TN_Task task_c;

void task_c_body(void *par);

#endif
