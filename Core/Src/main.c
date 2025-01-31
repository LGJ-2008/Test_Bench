/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "diskio.h"
#include "pre_sen.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t Pre_Buf[16]={0x00};






uint8_t Pre_init_OK[] = {0x01, 0x01};



char Pre_Decimal_digits_OK[]="Pressure Sensor：小数位初始化完成";
char Pre_Decimal_digits_Fail[]="Pressure Sensor：小数位初始化失败";

char Pre_Set_Unit_OK[]="Pressure Sensor：单位初始化完成";
char Pre_Set_Unit_Fail[]="Pressure Sensor：单位初始化失败";



char FS_res_OK[]="Fatfs,初始化完成";
char FS_res_Fail[]="Fatfs,初始化失败";







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
  MX_USB_DEVICE_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  static uint8_t Pre_Set_Decimal_digits[]={0x01, 0x06, 0x00, 0x01, 0x00, 0x01, 0x19, 0xCA};//设置小数点后精确到一位
  static uint8_t Pre_Set_Unit[]={0x01, 0x06, 0x00, 0x02, 0x00, 0x05, 0xE8, 0x09};//设置单位：N
  static uint8_t Pre_get_Value[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};//读取传感器数值



  // HAL_UART_Transmit(&huart2, Pre_Set_Decimal_digits, sizeof(Pre_Set_Decimal_digits),10);
  // HAL_UART_Receive(&huart2,Pre_Buf,8,100);
  // if (memcmp(Pre_Buf,Pre_Set_Decimal_digits,8)==0) {
  //   CDC_Transmit_FS(Pre_Decimal_digits_OK, sizeof(Pre_Decimal_digits_OK));
  // } else {
  //   CDC_Transmit_FS(Pre_Decimal_digits_Fail, sizeof(Pre_Decimal_digits_Fail));
  // }
  //
  //
  //
  //
  // HAL_UART_Transmit(&huart2, Pre_Set_Unit, sizeof(Pre_Set_Unit),10);
  // HAL_UART_Receive(&huart2,Pre_Buf,8,100);
  // if (memcmp(Pre_Buf,Pre_Set_Unit,8)==0) {
  //   CDC_Transmit_FS(Pre_Set_Unit_OK, sizeof(Pre_Set_Unit_OK));
  // } else {
  //   CDC_Transmit_FS(Pre_Set_Unit_Fail, sizeof(Pre_Set_Unit_Fail));
  // }

  HAL_Delay(2000);




  FATFS *fs;//新建文件系统对象
  FRESULT fres;//
  FIL file;  //新建文件对象

  fs = malloc(sizeof (FATFS));   /* Get work area for the volume */

  fres = f_mount(fs, "", 1);
  if (fres == FR_OK)
  {
    CDC_Transmit_FS(FS_res_OK, sizeof(FS_res_OK));
  } else
  {
    CDC_Transmit_FS(FS_res_Fail, sizeof(FS_res_Fail));
    while (1);
  }



  fres=f_open(&file, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (fres == FR_OK)
  {
    CDC_Transmit_FS(FS_res_OK, sizeof(FS_res_OK));
  } else
  {
    CDC_Transmit_FS(FS_res_Fail, sizeof(FS_res_Fail));
    while (1);
  }


  char wtext[] = "Hello, FatFs!\r\n";
  UINT bw;
  fres = f_write(&file, wtext, sizeof(wtext), &bw);
  if (fres == FR_OK)
  {
    CDC_Transmit_FS(FS_res_OK, sizeof(FS_res_OK));
  } else
  {
    CDC_Transmit_FS(FS_res_Fail, sizeof(FS_res_Fail));
  }


  fres = f_close(&file);
  if (fres == FR_OK)
  {
    CDC_Transmit_FS(FS_res_OK, sizeof(FS_res_OK));
  } else
  {
    CDC_Transmit_FS(FS_res_Fail, sizeof(FS_res_Fail));
    while (1);
  }




  pre_init();

  uint8_t clear[] ={0x01, 0x06, 0x00, 0x11, 0x00, 0x01, 0x18, 0x0F};


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    uint8_t buffer[10]={0x00};
    HAL_UART_Transmit(Pressure_UART_handler, Pre_get_Value, sizeof(Pre_get_Value), 100);
    HAL_UART_Receive(Pressure_UART_handler, buffer, 7, 100);


    uint8_t value[2];
    memcpy(value, &buffer[3], 2);

    CDC_Transmit_FS(value, sizeof(value));

    char result[5];

    snprintf(result, 5, "%02x%02x", value[0], value[1]);

    // uint8_t result = sprintf(value, 2, "%02x%02x", value[0], value[1]);
    // result=strtol(result, NULL, 16);
    // char Value[5]="0";
    // sprintf(Value, "%ld", result);

    uint8_t Result=0;
    Result = strtol(result, NULL, 16);

    CDC_Transmit_FS(&Result, sizeof(Result));

    static int i=0;
    i++;
    if (i%10==0)
    {
      pre_send_reback(clear);
    }


    HAL_Delay(300);






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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
