#ifndef __HAL_LTIM_H__
#define __HAL_LTIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "py32f0xx_hal.h"
#include "py32f002bxx_Start_Kit.h"
#include "py32f002b_hal_lptim.h"
#include "oled.h"

#include "FreeRTOS.h"
#include "task.h"

/* MPU includes. */
#include "mpu_wrappers.h"
#include "mpu_syscall_numbers.h"

/* Portasm includes. */
#include "portasm.h"


void LtimInit(void);
void vAssertCalled( const char *pcFile, uint32_t ulLine );

#ifdef __cplusplus
}
#endif

#endif