#ifndef __SYN480_433_H__
#define __SYN480_433_H__

// #include "py32f0xx_hal.h"
#include "timer.h"

// 状态定义
enum RxState {
    RX_WAIT_SYNC = 0,
    RX_READ_DATA
};

class SYN480_Rx_ISR {
private:
    GPIO_TypeDef* _port;
    uint16_t _pin;
    
    volatile RxState _state;
    volatile uint32_t _counter;     // 脉宽计数器
    volatile uint32_t _raw_data;    // 正在接收的数据
    volatile uint8_t  _bit_index;   // 当前收到第几位
    volatile uint32_t _valid_data;  // 接收完成的有效数据
    volatile bool     _has_new_data;// 数据就绪标志

    // 协议参数 (基于 50us 一个 tick)
    // Sync > 8ms -> 8000us / 50 = 160 ticks
    const uint16_t TICK_SYNC_MIN = 160; 
    // Bit0 High ~350us -> 7 ticks (范围 4-12)
    // Bit1 High ~1050us -> 21 ticks (范围 15-30)
    
public:
    SYN480_Rx_ISR(GPIO_TypeDef* port, uint16_t pin);
    
    // 这个函数放在定时器中断里跑
    void tickISR(); 

    // 主循环调用，检查是否有数据
    bool available();
    uint32_t read();
};

// 声明一个全局指针，方便 C 语言的中断函数调用 C++ 对象
extern SYN480_Rx_ISR* g_pRx433; 

#endif