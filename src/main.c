/*****************************************************************************
Copyright (c) 2019, Shulaev Viktor
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "stm32f10x.h"
#include <stdlib.h>

#include "tn.h"


#include "task_a.h"
#include "task_b.h"
#include "task_c.h"

extern void disk_timerproc (void);



//-- idle task stack size, in words
#define IDLE_TASK_STACK_SIZE          (TN_MIN_STACK_SIZE + 32)

//-- interrupt stack size, in words
#define INTERRUPT_STACK_SIZE          (TN_MIN_STACK_SIZE + 64)



//-- system frequency ??????????????????????????????
#define SYS_FREQ           SystemCoreClock        //168000000L

//-- kernel ticks (system timer) frequency
#define SYS_TMR_FREQ       1000

//-- system timer period (auto-calculated)
#define SYS_TMR_PERIOD              \
   (SYS_FREQ / SYS_TMR_FREQ)





//-- Allocate arrays for stacks: stack for idle task
//   and for interrupts are the requirement of the kernel;
//   others are application-dependent.
//
//   We use convenience macro TN_STACK_ARR_DEF() for that.

TN_STACK_ARR_DEF(idle_task_stack, IDLE_TASK_STACK_SIZE);
TN_STACK_ARR_DEF(interrupt_stack, INTERRUPT_STACK_SIZE);


GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;


/**
 * Hardware init: called from main() with interrupts disabled
 */
void hw_init(void)
{
   //-- init system timer
   SysTick_Config(SYS_TMR_PERIOD);
}

//-- idle callback that is called periodically from idle task
void idle_task_callback (void)
{
}

//-- create first application task(s)
void init_task_create(void)
{
   //-- task A performs complete application initialization,
   //   it's the first created application task
   tn_task_create(
         &task_a,                   //-- task structure
         task_a_body,               //-- task body function
         TASK_A_PRIORITY,           //-- task priority
         task_a_stack,              //-- task stack
         TASK_A_STK_SIZE,           //-- task stack size (in words)
         NULL,                      //-- task function parameter
         TN_TASK_CREATE_OPT_START   //-- creation option
         );

}



int main(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


//  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

  while (1)
  {


   //-- unconditionally disable interrupts
   tn_arch_int_dis();

   //-- init hardware
   hw_init();

   //-- call to tn_sys_start() never returns
   tn_sys_start(
         idle_task_stack,
         IDLE_TASK_STACK_SIZE,
         interrupt_stack,
         INTERRUPT_STACK_SIZE,
         init_task_create,
         idle_task_callback
         );

   //-- unreachable
  return 1;


/*
	cnt++;
	if(cnt > 0xF000)
	{
		cnt = 0;
		USART_SendData(USART1, 0x55);
	};


	if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) )
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	};
*/

  }
}


/**
 * system timer ISR
 */
void SysTick_Handler(void)
{
	tn_tick_int_processing();


//	tmp123++;
//	if( tmp123 & 1 )
//	{
//		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //зажигаем
//	}else{
//		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //гасим
//	};



	Timer++;	/* Increment performance counter */

	disk_timerproc();	/* Disk timer process */

}

