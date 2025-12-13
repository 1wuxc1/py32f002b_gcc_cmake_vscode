#ifndef HAL_I2C_H
#define HAL_I2C_H
#ifdef __cplusplus
extern "C" {
#endif
#include "py32f0xx_hal.h"
#include "py32f002bxx_Start_Kit.h"
#include "py32f002b_hal_i2c.h"
/* Private define ------------------------------------------------------------*/
// #define DATA_LENGTH      15                 /* Data length*/
#define I2C_ADDRESS      0xA0               /* Local address 0xA0 */
#define I2C_SPEEDCLOCK   100000             /* Communication speed 100K */
#define I2C_DUTYCYCLE    I2C_DUTYCYCLE_16_9 /* Duty cycle */
void I2cInit(void);
void I2cSendBytes(uint16_t i2c_addr,uint8_t *aTxBuffer, uint16_t data_len);
void I2cSendByte(uint16_t i2c_addr,uint8_t aTxBuffer);

#ifdef __cplusplus
}
#endif
#endif