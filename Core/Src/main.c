/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>  // Required for va_list, va_start, va_end
#include <csp/csp.h>
#include <csp/interfaces/csp_if_lo.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FAT_w_task */
osThreadId_t FAT_w_taskHandle;
uint32_t FAT_w_task_Buffer[ 512 ];
osStaticThreadDef_t FAT_w_task_ControlBlock;
const osThreadAttr_t FAT_w_task_attributes = {
  .name = "FAT_w_task",
  .cb_mem = &FAT_w_task_ControlBlock,
  .cb_size = sizeof(FAT_w_task_ControlBlock),
  .stack_mem = &FAT_w_task_Buffer[0],
  .stack_size = sizeof(FAT_w_task_Buffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FAT_r_task */
osThreadId_t FAT_r_taskHandle;
uint32_t Task_FAT_r_Buffer[ 512 ];
osStaticThreadDef_t Task_FAT_r_ControlBlock;
const osThreadAttr_t FAT_r_task_attributes = {
  .name = "FAT_r_task",
  .cb_mem = &Task_FAT_r_ControlBlock,
  .cb_size = sizeof(Task_FAT_r_ControlBlock),
  .stack_mem = &Task_FAT_r_Buffer[0],
  .stack_size = sizeof(Task_FAT_r_Buffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for communTask */
osThreadId_t communTaskHandle;
uint32_t communTaskBuffer[ 512 ];
osStaticThreadDef_t communTaskControlBlock;
const osThreadAttr_t communTask_attributes = {
  .name = "communTask",
  .cb_mem = &communTaskControlBlock,
  .cb_size = sizeof(communTaskControlBlock),
  .stack_mem = &communTaskBuffer[0],
  .stack_size = sizeof(communTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for cmdHandleTask */
osThreadId_t cmdHandleTaskHandle;
uint32_t cmdHandleTaskBuffer[ 512 ];
osStaticThreadDef_t cmdHandleTaskControlBlock;
const osThreadAttr_t cmdHandleTask_attributes = {
  .name = "cmdHandleTask",
  .cb_mem = &cmdHandleTaskControlBlock,
  .cb_size = sizeof(cmdHandleTaskControlBlock),
  .stack_mem = &cmdHandleTaskBuffer[0],
  .stack_size = sizeof(cmdHandleTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for commandQueue */
osMessageQueueId_t commandQueueHandle;
uint8_t commandQueueBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t commandQueueControlBlock;
const osMessageQueueAttr_t commandQueue_attributes = {
  .name = "commandQueue",
  .cb_mem = &commandQueueControlBlock,
  .cb_size = sizeof(commandQueueControlBlock),
  .mq_mem = &commandQueueBuffer,
  .mq_size = sizeof(commandQueueBuffer)
};
/* Definitions for dataQueue */
osMessageQueueId_t dataQueueHandle;
uint8_t dataQueueBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t dataQueueControlBlock;
const osMessageQueueAttr_t dataQueue_attributes = {
  .name = "dataQueue",
  .cb_mem = &dataQueueControlBlock,
  .cb_size = sizeof(dataQueueControlBlock),
  .mq_mem = &dataQueueBuffer,
  .mq_size = sizeof(dataQueueBuffer)
};
/* Definitions for TTQ */
osMessageQueueId_t TTQHandle;
uint8_t TTQBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t TTQControlBlock;
const osMessageQueueAttr_t TTQ_attributes = {
  .name = "TTQ",
  .cb_mem = &TTQControlBlock,
  .cb_size = sizeof(TTQControlBlock),
  .mq_mem = &TTQBuffer,
  .mq_size = sizeof(TTQBuffer)
};
/* Definitions for errorQueue */
osMessageQueueId_t errorQueueHandle;
uint8_t errorQueueBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t errorQueueControlBlock;
const osMessageQueueAttr_t errorQueue_attributes = {
  .name = "errorQueue",
  .cb_mem = &errorQueueControlBlock,
  .cb_size = sizeof(errorQueueControlBlock),
  .mq_mem = &errorQueueBuffer,
  .mq_size = sizeof(errorQueueBuffer)
};
/* Definitions for myFileMutex */
osMutexId_t myFileMutexHandle;
osStaticMutexDef_t myFileMutexControlBlock;
const osMutexAttr_t myFileMutex_attributes = {
  .name = "myFileMutex",
  .cb_mem = &myFileMutexControlBlock,
  .cb_size = sizeof(myFileMutexControlBlock),
};
/* Definitions for myPrintMutex */
osMutexId_t myPrintMutexHandle;
osStaticMutexDef_t myPrintMutexControlBlock;
const osMutexAttr_t myPrintMutex_attributes = {
  .name = "myPrintMutex",
  .cb_mem = &myPrintMutexControlBlock,
  .cb_size = sizeof(myPrintMutexControlBlock),
};
/* Definitions for file_semaphore */
osSemaphoreId_t file_semaphoreHandle;
osStaticSemaphoreDef_t file_semaphoreControlBlock;
const osSemaphoreAttr_t file_semaphore_attributes = {
  .name = "file_semaphore",
  .cb_mem = &file_semaphoreControlBlock,
  .cb_size = sizeof(file_semaphoreControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartTask_FAT_w(void *argument);
void StartTask_FAT_r(void *argument);
void StartTask_communTask(void *argument);
void StartTask_cmdHandle(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // Initialize CSP
  csp_init();  // Initialize the CSP protocol stack

  // Initialize CSP buffer system
  csp_buffer_init();

  // Configure the CSP routing table to use the loopback interface
  csp_rtable_set(0, 0, &csp_if_lo, CSP_NO_VIA_ADDRESS);  // Route all packets to the loopback interface

  // Create mutex before mounting
  myFileMutexHandle = osMutexNew(&myFileMutex_attributes);
  if(myFileMutexHandle == NULL)
  {
      Error_Handler();
  }

  // Mount filesystem
  FATFS fs;
  FRESULT fres;

  /* Use "0:" to specify drive number 0 for RAM disk */
  fres = f_mount(&fs, "0:", 1);
  if (fres == FR_NO_FILESYSTEM) {
      printf("No filesystem found. Formatting...\r\n");

      // Define a static work buffer to avoid stack overflow
      #define MKFS_WORK_BUFFER_SIZE  4096
      static BYTE mkfs_work_buffer[MKFS_WORK_BUFFER_SIZE];

      /*
       * Filesystem Formatting and RAM Disk Configuration
       * ------------------------------------------------
       * Problem:
       *  - `f_mkfs()` was failing with `FR_MKFS_ABORTED` because the volume size was too small after
       *    reserving 63 sectors for partitioning (when not using the super floppy format).
       *
       * Solution:
       * 1. Use Super Floppy Format (`FM_SFD`):
       *    - This avoids reserving 63 sectors by starting the volume at sector 0 (no partition table).
       *    - This ensures the full number of sectors is available for the filesystem.
       *
       * 2. Increase Sector Count:
       *    - FAT requires a minimum of 128 sectors. The RAM disk was increased to 150 sectors to satisfy this requirement.
       *
       * 3. Sector Size:
       *    - The sector size is set to 512 bytes, which is standard for FAT filesystems.
       *    - With 150 sectors of 512 bytes, the total size of the RAM disk is ~75KB, sufficient for FAT12/FAT16.
       *
       * `f_mkfs()` Call:
       *  - "0:" specifies the RAM disk.
       *  - `FM_FAT | FM_SFD` selects the FAT type automatically and uses the super floppy format.
       *  - `0` lets FatFs choose the optimal cluster size.
       *  - `mkfs_work_buffer` is a 4096-byte buffer used for formatting.
       *
       */

      // Format the RAM disk with best FAT
      fres = f_mkfs("0:", FM_FAT | FM_SFD, 0, mkfs_work_buffer, MKFS_WORK_BUFFER_SIZE);
      if (fres == FR_OK) {
          printf("Format successful. Mounting again...\r\n");
          fres = f_mount(&fs, "0:", 1);
          if (fres == FR_OK) {
              printf("f_mount succeeded after formatting.\r\n");
          } else {
              printf("f_mount error after formatting (%i)\r\n", fres);
              Error_Handler();
          }
      } else {
          printf("f_mkfs error (%i)\r\n", fres);
          Error_Handler();
      }
  }
  else if (fres != FR_OK)
  {
      printf("f_mount error (%i)\r\n", fres);
      Error_Handler();
  }
  else
  {
      printf("f_mount succeeded.\r\n");
  }

  /* Optional: Test File Operations */
  FIL file;
  UINT bw;
  FRESULT res;

  // Create and Write to a file
  res = f_open(&file, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
  if (res == FR_OK) {
      char data[] = "Hello, RAM Disk!";
      res = f_write(&file, data, sizeof(data), &bw);
      if (res == FR_OK && bw == sizeof(data)) {
          printf("Write successful.\r\n");
      } else {
          printf("Write failed (%i)\r\n", res);
      }
      f_close(&file);
  } else {
      printf("f_open failed (%i)\r\n", res);
  }

  // Read from the file
  res = f_open(&file, "0:/test.txt", FA_READ);
  if (res == FR_OK) {
      char read_data[50];
      UINT br;
      res = f_read(&file, read_data, sizeof(read_data)-1, &br);
      if (res == FR_OK) {
          read_data[br] = '\0'; // Null-terminate the string
          printf("Read data: %s\r\n", read_data);
      } else {
          printf("Read failed (%i)\r\n", res);
      }
      f_close(&file);
  } else {
      printf("f_open failed (%i)\r\n", res);
  }

  // Delete the file
  res = f_unlink("0:/test.txt");
  if (res == FR_OK) {
      printf("Deleted test.txt successfully.\r\n");
  } else {
      printf("Deleting test.txt failed (%i)\r\n", res);
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of myFileMutex */
  myFileMutexHandle = osMutexNew(&myFileMutex_attributes);

  /* creation of myPrintMutex */
  myPrintMutexHandle = osMutexNew(&myPrintMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of file_semaphore */
  file_semaphoreHandle = osSemaphoreNew(1, 0, &file_semaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of commandQueue */
  commandQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &commandQueue_attributes);

  /* creation of dataQueue */
  dataQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &dataQueue_attributes);

  /* creation of TTQ */
  TTQHandle = osMessageQueueNew (16, sizeof(uint16_t), &TTQ_attributes);

  /* creation of errorQueue */
  errorQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &errorQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of FAT_w_task */
  FAT_w_taskHandle = osThreadNew(StartTask_FAT_w, NULL, &FAT_w_task_attributes);

  /* creation of FAT_r_task */
  FAT_r_taskHandle = osThreadNew(StartTask_FAT_r, NULL, &FAT_r_task_attributes);

  /* creation of communTask */
  communTaskHandle = osThreadNew(StartTask_communTask, NULL, &communTask_attributes);

  /* creation of cmdHandleTask */
  cmdHandleTaskHandle = osThreadNew(StartTask_cmdHandle, NULL, &cmdHandleTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// These functions redirect output to UART
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}

// Thread safe print function
void safePrint(const char* format, ...) {
    va_list args;

    // Acquire the print mutex before printing
    if (osMutexAcquire(myPrintMutexHandle, osWaitForever) == osOK) {
        va_start(args, format);
        vprintf(format, args);  // Use vprintf to handle variable arguments
        va_end(args);

        // Release the print mutex after printing
        osMutexRelease(myPrintMutexHandle);
    } else {
        // Handle mutex acquisition failure (optional)
        printf("Failed to acquire print mutex!\n\r");
    }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask_FAT_w */
/**
* @brief Function implementing the FAT_w_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_FAT_w */
void StartTask_FAT_w(void *argument)
{
  /* USER CODE BEGIN StartTask_FAT_w */
  FIL file;
  FRESULT fres;
  UINT bw; // Bytes written

  char write_data[] = "Hello, this is a FAT test!\n";

  /* Infinite loop */
  for(;;)
  {
    // Wait for the mutex before accessing the file
    if (osMutexAcquire(myFileMutexHandle, osWaitForever) == osOK)
    {
      // Open or create the file for writing
      fres = f_open(&file, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
      if (fres == FR_OK) {
        // Write data to the file
        fres = f_write(&file, write_data, sizeof(write_data) - 1, &bw);
        if (fres == FR_OK && bw == sizeof(write_data) - 1) {
          safePrint("FAT_w_task: Data written successfully.\r\n");
        } else {
          safePrint("FAT_w_task: Write error! Error code: %d\r\n", fres);
        }

        // Close the file after writing
        f_close(&file);
      } else {
        safePrint("FAT_w_task: f_open failed! Error code: %d\r\n", fres);
      }

      // Release the mutex after accessing the file
      osMutexRelease(myFileMutexHandle);

      // Signal the read task that the file is ready after mutex release
      osSemaphoreRelease(file_semaphoreHandle);
    }

    // Delay before the next write operation
    osDelay(5000); // Write every 5 seconds
  }
  /* USER CODE END StartTask_FAT_w */
}

/* USER CODE BEGIN Header_StartTask_FAT_r */
/**
* @brief Function implementing the FAT_r_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_FAT_r */
void StartTask_FAT_r(void *argument)
{
  /* USER CODE BEGIN StartTask_FAT_r */
  FIL file;
  FRESULT fres;
  UINT br; // Bytes read
  char read_data[128]; // Buffer to hold read data

  /* Infinite loop */
  for(;;)
  {
    // Wait for the semaphore signal from the write task
    if (osSemaphoreAcquire(file_semaphoreHandle, osWaitForever) == osOK)
    {
      // After receiving the semaphore, acquire the mutex before reading the file
      if (osMutexAcquire(myFileMutexHandle, osWaitForever) == osOK)
      {
        // Open the file for reading
        fres = f_open(&file, "0:/test.txt", FA_READ);
        if (fres == FR_OK) {
          // Read data from the file
          fres = f_read(&file, read_data, sizeof(read_data) - 1, &br);
          if (fres == FR_OK) {
            read_data[br] = '\0'; // Null-terminate the string
            safePrint("FAT_r_task: Data read: %s\r\n", read_data);
          } else {
            safePrint("FAT_r_task: Read error! Error code: %d\r\n", fres);
          }

          // Close the file after reading
          f_close(&file);
        } else {
          safePrint("FAT_r_task: f_open failed! Error code: %d\r\n", fres);
        }

        // Release the mutex after file access
        osMutexRelease(myFileMutexHandle);
      }
    }

    // Delay before the next read operation
    osDelay(5000); // Read every 5 seconds
  }
  /* USER CODE END StartTask_FAT_r */
}

/* USER CODE BEGIN Header_StartTask_communTask */
/**
* @brief This task Receives commands and places them in the Command Queue.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_communTask */
void StartTask_communTask(void *argument)
{
  /* USER CODE BEGIN StartTask_communTask */
    csp_packet_t *packet;
  /* Infinite loop */
  for(;;)
  {
	/* Wait for a packet from CSP */
	packet = csp_buffer_get(100);  // Get buffer for CSP communication (100 bytes)
	if (packet == NULL) {
	  osDelay(100);  // No packet available, wait
	  continue;
	}

	/* Simulate received command (e.g., command ID 1) */
	uint16_t command = 1;  // For example, command ID 1
	osMessageQueuePut(commandQueueHandle, &command, 0, osWaitForever);  // Send command to Command Queue

	/* Free the CSP packet */
	csp_buffer_free(packet);

	osDelay(1000);  // Delay for next communication
  }
  /* USER CODE END StartTask_communTask */
}

/* USER CODE BEGIN Header_StartTask_cmdHandle */
/**
* @brief Function implementing the cmdHandleTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_cmdHandle */
void StartTask_cmdHandle(void *argument)
{
  /* USER CODE BEGIN StartTask_cmdHandle */
    uint16_t command;
  /* Infinite loop */
  for(;;)
  {
	/* Wait for a command in the Command Queue */
	if (osMessageQueueGet(commandQueueHandle, &command, NULL, osWaitForever) == osOK) {
	  /* Process the command and dispatch to payload tasks */
	  switch (command) {
		  case 1:
			  safePrint("Command 1 received, starting PayloadTask1.\n");
			  //osThreadNew(PayloadTask1, NULL, NULL);
			  break;
		  case 2:
			  safePrint("Command 2 received, starting PayloadTask2.\n");
			  //osThreadNew(PayloadTask2, NULL, NULL);
			  break;
		  default:
			  safePrint("Unknown command received: %lu\n", command);
			  break;
	  }
	}

	osDelay(100);  // Delay to avoid CPU overuse
  }
  /* USER CODE END StartTask_cmdHandle */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
