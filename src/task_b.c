#include "stm32f10x.h"
#include <stdlib.h>
#include "ff.h"
#include "diskio.h"
#include "rtc_stm32f1.h"
#include "task_b.h"

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

