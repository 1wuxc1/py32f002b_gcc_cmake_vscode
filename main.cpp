/**
 ******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by Puya under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hal_i2c.h"
#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hal_ltim.h"

void Delay(uint32_t mdelay)
{
  __IO uint32_t Delay = mdelay * (24000000U / 8U / 1000U);
  do
  {
    __NOP();
  } while (Delay--);
}
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// extern I2C_HandleTypeDef hi2c1;
// ADC_ChannelConfTypeDef        sConfig;
// volatile uint16_t             aADCxConvertedData;
// TIM_HandleTypeDef             TimHandle;
// TIM_MasterConfigTypeDef       sMasterConfig;
// ADC_AnalogWDGConfTypeDef      ADCAnalogWDGConfig;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task1(void *pvParameters)
{
  uint8_t i = 0;
  while (1)
  {
    i++;
    OLED_ShowNum(0, 0, i, 3, 12);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
/**
 * @brief  Main program.
 * @retval int
 */
extern "C" {
int main(void)
{
  SystemInit();
  Delay(2000);

  HAL_Init();  // 初始化 HAL 和 SysTick
  I2cInit();   // 初始化 I2C
  OLED_Init(); // 初始化 OLED
  LtimInit();
  xTaskCreate(Task1, "Task1", 128, NULL, 2, NULL);

  vTaskStartScheduler();

  while (1)
  {
  }
}
}

/**
 * @brief  ADC Analog watchdog callback
 * @param  hadc：ADC handle
 * @retval None
 */
// void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
// {
//   BSP_LED_On(LED_GREEN);
// }

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void APP_ErrorHandler(void)
{
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
