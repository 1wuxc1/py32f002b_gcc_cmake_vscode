#ifndef __SY115_433_H__
#define __SY115_433_H__

#include "py32f0xx_hal.h" // 确保包含PY32的HAL库

// 协议时序参数 (基于 EV1527, 单位: 微秒)
// 通常 1个时钟周期(LCK) 约为 300us - 400us
#define EV1527_LCK  350 

class SY115_Tx {
private:
    GPIO_TypeDef* _port;
    uint16_t _pin;

    // 微秒级延时函数，需要用户根据主频实现或外部传入
    void (*_delay_us)(uint32_t); 

    // 私有辅助函数：发送逻辑0 和 逻辑1
    void sendBit0();
    void sendBit1();
    void sendPreamble();

public:
    // 构造函数
    SY115_Tx(GPIO_TypeDef* port, uint16_t pin, void (*delay_func)(uint32_t));

    // 初始化 GPIO
    void begin();

    // 发送数据函数
    // data: 24位数据 (通常是 20位地址 + 4位按键值)
    // repeat: 重发次数 (无线信号通常建议发送 3-5 次以防丢包)
    void send(uint32_t data, uint8_t repeat = 4);
};

#endif