#include "timer.h"
TIM_HandleTypeDef htim14;
extern void Rx_Tick_Wrapper(void);
// 初始化定时器：50us 中断
void MX_TIM14_Init(void)
{
    //   TIM_MasterConfigTypeDef sMasterConfig = {0};

    // 1. 开启时钟
    __HAL_RCC_TIM14_CLK_ENABLE();

    htim14.Instance = TIM14;
    // 24MHz / 24 = 1MHz (1us 一个计数)
    htim14.Init.Prescaler = 24 - 1;
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
    // 计数到 50 溢出 -> 50us
    // 如果你非要 10us，把这里改成 10 - 1
    htim14.Init.Period = 50 - 1;
    htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
    {
        // Error_Handler();
    }

    // 2. 开启中断
    HAL_NVIC_SetPriority(TIM14_IRQn, 1, 0); // 优先级不要太高，以免卡死 FreeRTOS tick
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
    HAL_TIM_Base_Start_IT(&htim14);
}

// 启动定时器

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 必须判断是哪个定时器进来的，因为所有定时器都会调这个函数
    if (htim->Instance == TIM14)
    {
        // 调用你的 433 接收处理逻辑
        Rx_Tick_Wrapper();
    }
}