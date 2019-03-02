#include "stm32f10x.h"
#include <stdlib.h>

#include "tn.h"

#include <string.h>

#include "microrl.h" 
#include "terminal_misc.h"




#include "ff.h"
#include "diskio.h"
#include "rtc_stm32f1.h"





//-- idle task stack size, in words
#define IDLE_TASK_STACK_SIZE          (TN_MIN_STACK_SIZE + 32)

//-- interrupt stack size, in words
#define INTERRUPT_STACK_SIZE          (TN_MIN_STACK_SIZE + 64)

//-- stack sizes of user tasks
#define TASK_A_STK_SIZE    (TN_MIN_STACK_SIZE + 96)
#define TASK_B_STK_SIZE    (TN_MIN_STACK_SIZE + 96)
#define TASK_C_STK_SIZE    (TN_MIN_STACK_SIZE + 96)

//-- user task priorities
#define TASK_A_PRIORITY    7
#define TASK_B_PRIORITY    6
#define TASK_C_PRIORITY    5



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

TN_STACK_ARR_DEF(task_a_stack, TASK_A_STK_SIZE);
TN_STACK_ARR_DEF(task_b_stack, TASK_B_STK_SIZE);
TN_STACK_ARR_DEF(task_c_stack, TASK_C_STK_SIZE);

//-- task structures

struct TN_Task task_a;
struct TN_Task task_b;
struct TN_Task task_c;







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





//ff
extern void disk_timerproc (void);

DWORD AccSize;				/* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;

char Line[256];				/* Console input buffer */
BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */

FATFS FatFs;				/* File system object for each logical drive */
FIL File[2];				/* File objects */
DIR Dir;					/* Directory object */

volatile UINT Timer;



DWORD get_fattime (void);

DWORD get_fattime (void)
{
	RTCTIME rtc;

	/* Get local time */
	///if (!rtc_gettime(&rtc)) return 0;

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}



void appl_init(void);

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

void task_b_body(void *par)
{
char	buff[1024];		// буфер для чтения/записи

	FRESULT result;

	// смонтировать диск
	FATFS FATFS_Obj;

	result = f_mount(&FATFS_Obj, "0", 1);
	if (result != FR_OK)
	{
		//printf("Ошибка монтирования диска %d\r\n", result);
	}


	// считаем файлы в корневом каталоге
	DIR dir;
	FILINFO fileInfo;
	int nFiles = 0;

	result = f_opendir(&dir, "/");
	if (result == FR_OK)
	{
		while (((result = f_readdir(&dir, &fileInfo)) == FR_OK) && fileInfo.fname[0])
		{
			nFiles++;
		}
	}
	f_closedir(&dir);


	// открываем файл readme.txt для чтения
	FIL file;
	UINT nRead, nWritten;

	result = f_open(&file, "readme.txt", FA_OPEN_EXISTING | FA_READ);
	if (result == FR_OK)
	{
		f_read(&file, &buff, 1024, &nRead);
		f_close(&file);
	}

	// создаем файл write.txt
	result = f_open(&file, "write.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if (result == FR_OK)
	{
		f_write(&file, &buff, nRead, &nWritten);
		f_close(&file);
	}




   for(;;)
   {



//	if( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) )
//	{
//		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //зажигаем
//	}else{
//		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //гасим
//	};

//      if (GPIOD->ODR & LED2){
//         GPIOD->BSRRH = LED2;
//      } else {
//         GPIOD->BSRRL = LED2;
//      }

      //printf("task b\n");
//      tn_task_sleep(1000);
      tn_task_sleep(100);
   }
}

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

/**
 * Hardware init: called from main() with interrupts disabled
 */
void hw_init(void)
{
   //-- init system timer
   SysTick_Config(SYS_TMR_PERIOD);
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


	tmp123++;
	if( tmp123 & 1 )
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //зажигаем
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //гасим
	};



	Timer++;	/* Increment performance counter */

	disk_timerproc();	/* Disk timer process */

}