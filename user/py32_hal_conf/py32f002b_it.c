/**
  ******************************************************************************
  * @file    py32f002b_it.c
  * @author  MCU Application Team
  * @brief   Interrupt Service Routines.
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
#include "py32f002b_it.h"
extern TIM_HandleTypeDef htim14;

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
// extern ADC_HandleTypeDef hadc;

/******************************************************************************/
/*          Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    
    
    // 死循环以便调试
    while (1)
    {
        // 可以添加LED闪烁或其他调试指示
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
/**
  * @brief This function handles System service call via SWI instruction.
  */
 #ifndef SVC_Handler
// void SVC_Handler(void)
// {
//     // 调用 FreeRTOS 的 SVC 处理函数

// }
#endif

/**
  * @brief This function handles Pendable request for system service.
  */
#ifndef PendSV_Handler
// void PendSV_Handler(void)
// {
//     // 调用 FreeRTOS 的 PendSV 处理函数
// }
#endif

void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* PY32F002B Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file.                                          */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
// void ADC_COMP_IRQHandler(void)
// {
//   HAL_ADC_IRQHandler(&hadc);
// }
void TIM14_IRQHandler(void)
{
  // 调用 HAL 库的统一处理函数
  // 这个函数内部会自动清除标志位，并调用 HAL_TIM_PeriodElapsedCallback
  HAL_TIM_IRQHandler(&htim14);
}



/************************ (C) COPYRIGHT Puya *****END OF FILE******************/