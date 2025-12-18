#ifndef __DELAY_H__
#define __DELAY_H__ 
#ifdef __cplusplus
extern "C" {
#endif
#include "py32f0xx_hal.h" // 确保包含PY32的HAL库
void Delay_Us(uint32_t udelay);
void Delay_ms(uint32_t mdelay);

#ifdef __cplusplus
}
#endif
#endif