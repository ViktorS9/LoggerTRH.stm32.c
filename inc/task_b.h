#ifndef TASK_B_H
#define TASK_B_H

#include "tn.h"

#include "ff.h"
#include "diskio.h"

//-- stack sizes of user tasks
#define TASK_B_STK_SIZE    (TN_MIN_STACK_SIZE + 96)

//-- user task priorities
#define TASK_B_PRIORITY    6

//-- Allocate arrays for stacks: stack for idle task
//   and for interrupts are the requirement of the kernel;
//   others are application-dependent.
//
//   We use convenience macro TN_STACK_ARR_DEF() for that.
TN_STACK_ARR_DEF(task_b_stack, TASK_B_STK_SIZE);

//-- task structures
struct TN_Task task_b;

volatile UINT Timer;

void task_b_body(void *par);

#endif
