#include "hal_i2c.h"

I2C_HandleTypeDef hi2c1;
void I2C_Error_Handler(void)
{ 
    while (1)
    {
    }
    
}
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  if(hi2c->Instance == I2C)  // 确认是哪个I2C外设
  {
    // 1. 启用I2C外设时钟
    __HAL_RCC_I2C_CLK_ENABLE();
    
    // 2. 启用GPIO时钟（根据实际硬件连接修改端口）
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    // 3. 配置SCL和SDA引脚（开漏输出+上拉是I2C协议要求）
    GPIO_InitStruct.Pin       = GPIO_PIN_4 | GPIO_PIN_3;  // 假设使用PA9(SCL)和PA10(SDA)
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;           // 复用开漏输出模式
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;      // 高速
    GPIO_InitStruct.Pull      = GPIO_PULLUP;               // 上拉电阻
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;              // 复用为I2C功能
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
  }
}

void I2cInit(void)
{
    hi2c1.Instance = I2C1;                                 /* I2C */
    hi2c1.Init.ClockSpeed = I2C_SPEEDCLOCK;               /* I2C communication speed */
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE;                 /* I2C duty cycle */
    hi2c1.Init.OwnAddress1 = I2C_ADDRESS;                 /* I2C address */
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* Prohibit broadcast calls */
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* Allow clock extension */
    if(HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        I2C_Error_Handler();
    }

}
void I2cSendBytes(uint16_t i2c_addr,uint8_t *aTxBuffer, uint16_t data_len)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, (uint8_t *)aTxBuffer, data_len, 0xFFFF) != HAL_OK)
    {
        I2C_Error_Handler();
    }
}
void I2cSendByte(uint16_t i2c_addr,uint8_t aTxBuffer)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, (uint8_t *)&aTxBuffer, 1, 0xFFFF) != HAL_OK)
    {
        I2C_Error_Handler();
    }
}

