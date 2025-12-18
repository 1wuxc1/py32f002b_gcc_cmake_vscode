#include "hal_ltim.h"

LPTIM_HandleTypeDef LPTIMConf = {0};
RCC_PeriphCLKInitTypeDef LPTIM_RCC = {0};
RCC_OscInitTypeDef OSCINIT = {0};

void LtimInit_ErrorHandler(void)
{
    while (1)
    {
    }
}
/**
 * @brief This function handles LPTIM interrupt.
 */
void LPTIM1_IRQHandler(void)
{
    HAL_LPTIM_IRQHandler(&LPTIMConf);
}
void LtimInit(void)
{

    /* LSI Clock Configure */
    OSCINIT.OscillatorType = RCC_OSCILLATORTYPE_LSI;          /* LSI */
    OSCINIT.LSIState = RCC_LSI_ON;                            /* LSI ON */
    OSCINIT.LSICalibrationValue = RCC_LSICALIBRATION_32768Hz; /* LSI Set 32768Hz */
    /* RCC Configure */
    if (HAL_RCC_OscConfig(&OSCINIT) != HAL_OK)
    {
        LtimInit_ErrorHandler();
    }

    LPTIM_RCC.PeriphClockSelection = RCC_PERIPHCLK_LPTIM;   /* Select peripheral clock: LPTIM */
    LPTIM_RCC.LptimClockSelection = RCC_LPTIMCLKSOURCE_LSI; /* Select LPTIM clock source: LSI */
    /* Peripheral clock initialization */
    if (HAL_RCCEx_PeriphCLKConfig(&LPTIM_RCC) != HAL_OK)
    {
        LtimInit_ErrorHandler();
    }
    /* Enable LPTIM Clock */
    __HAL_RCC_LPTIM_CLK_ENABLE();

    // 配置LPTIM参数
    LPTIMConf.Instance = LPTIM1;
    LPTIMConf.Init.Prescaler = LPTIM_PRESCALER_DIV16;   // 128分频
    LPTIMConf.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE; // 立即更新模式

    // 初始化LPTIM
    if (HAL_LPTIM_Init(&LPTIMConf) != HAL_OK)
    {
        LtimInit_ErrorHandler();
    }

    // 启动LPTIM定时器，设置自动重载值以实现1ms中断
    HAL_LPTIM_SetContinue_Start_IT(&LPTIMConf, 1); // 自动重载值设为0
    HAL_NVIC_SetPriority(LPTIM1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
}
// uint8_t i = 0;
// int j = 0;
// void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
// {
//     if (hlptim->Instance == LPTIM1)
//     {
//         j++;
//         if(j > 1000)
//         {
//             j = 0;
//             i++;
//         OLED_ShowNum(0, 0, i, 3, 12);
//         }
        
//     }
// }
// LPTIM中断回调函数
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    if (hlptim->Instance == LPTIM1)
    {

        // 使用 FromISR 版本的函数
        // 注意：需要确保在 FreeRTOS 启动后再使能中断

        // 如果调度器已经运行
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        {
            if (xTaskIncrementTick() != pdFALSE)
            {
                // 请求上下文切换
                portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
            }
        }
    }
}
// void vPortSetupTimerInterrupt(void)
// {
//     // LPTIM 已经在 LtimInit() 中配置好了
//     // 这里不需要额外操作
// }
void vAssertCalled(const char *pcFile, uint32_t ulLine)
{
    /* 用户可以在此添加断言失败的处理代码 */
    taskDISABLE_INTERRUPTS();
    while (1)
        ;
}

