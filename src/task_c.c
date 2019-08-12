#include "stm32f10x.h"
#include <stdlib.h>

#include "task_c.h"

void task_c_body(void *par)
{
   for(;;)
   {
//      if (GPIOD->ODR & LED3){
//         GPIOD->BSRRH = LED3;
//      } else {
//         GPIOD->BSRRL = LED3;
//      }

      //printf("task c\n");
      tn_task_sleep(1500);
   }
}

