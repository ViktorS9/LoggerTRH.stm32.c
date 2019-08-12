#include "stm32f10x.h"
#include <stdlib.h>

#include <string.h>

#include "microrl.h" 
#include "terminal_misc.h"

#include "task_a.h"
#include "task_b.h"
#include "task_c.h"


GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

uint16_t tmp = 0;
uint16_t t = 0;
uint32_t cnt = 0;
uint8_t * p = 0;



// create microrl object and pointer on it

microrl_t rl;

microrl_t * prl = &rl; 

uint8_t tmp123 = 0;




void task_a_body(void *par)
{
   //-- this is a first created application task, so it needs to perform
   //   all the application initialization.
   appl_init();

   //-- and then, let's get to the primary job of the task
   //   (job for which task was created at all)


  init ();
  // call init with ptr to microrl instance and print callback
  microrl_init (prl, print);
  // set callback for execute
  microrl_set_execute_callback (prl, execute);

#ifdef _USE_COMPLETE
  // set callback for completion
  microrl_set_complete_callback (prl, complet);
#endif
  // set callback for Ctrl+C
//  microrl_set_sigint_callback (prl, sigint);






   for(;;)
   {

  microrl_insert_char (prl, get_char());


//      if (GPIOD->ODR & LED1){
//         GPIOD->BSRRH = LED1;
//      } else {
//         GPIOD->BSRRL = LED1;
//      }

      //printf("task a\n");
//      tn_task_sleep(500);
      tn_task_sleep(50);

   }
}


/**
 * Application init: called from the first created application task
 */
void appl_init(void)
{
   //-- configure LED port pins
   {
//      RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable Port D clock
//
//      //-- Set pin 12, 13, 14, 15 as general purpose output mode (pull-push)
//      GPIOD->MODER |= (0
//            | GPIO_MODER_MODER12_0
//            | GPIO_MODER_MODER13_0
//            | GPIO_MODER_MODER14_0
//            | GPIO_MODER_MODER15_0
//            ) ;
//
//      // GPIOD->OTYPER |= 0; //-- No need to change - use pull-push output
//
//      GPIOD->OSPEEDR |= (0
//            | GPIO_OSPEEDER_OSPEEDR12 // 100MHz operations
//            | GPIO_OSPEEDER_OSPEEDR13
//            | GPIO_OSPEEDER_OSPEEDR14
//            | GPIO_OSPEEDER_OSPEEDR15 
//            );
//
//      GPIOD->PUPDR = 0; // No pull up, no pull down
   }

   //-- initialize various on-board peripherals, such as
   //   flash memory, displays, etc.
   //   (in this sample project there's nothing to init)

   //-- initialize various program modules
   //   (in this sample project there's nothing to init)


   //-- create all the rest application tasks
   tn_task_create(
         &task_b,
         task_b_body,
         TASK_B_PRIORITY,
         task_b_stack,
         TASK_B_STK_SIZE,
         NULL,
         (TN_TASK_CREATE_OPT_START)
         );

   tn_task_create(
         &task_c,
         task_c_body,
         TASK_C_PRIORITY,
         task_c_stack,
         TASK_C_STK_SIZE,
         NULL,
         (TN_TASK_CREATE_OPT_START)
         );
}



