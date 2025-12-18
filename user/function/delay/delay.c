#include "delay.h"

// 微秒延时函数 (针对 24MHz 主频)
// udelay: 延时多少微秒
void Delay_Us(uint32_t udelay)
{
  // 24MHz / 1,000,000 = 24 个周期/微秒
  // 假设循环依然耗时约 8 个周期
  // 乘数因子 = 24 / 8 = 3
  
  __IO uint32_t Delay = udelay * 3; 

  do
  {
    __NOP();
  } while (Delay--);
}

void Delay_ms(uint32_t mdelay)
{
  __IO uint32_t Delay = mdelay * (24000000U / 8U / 1000U);
  do
  {
    __NOP();
  } while (Delay--);
}
