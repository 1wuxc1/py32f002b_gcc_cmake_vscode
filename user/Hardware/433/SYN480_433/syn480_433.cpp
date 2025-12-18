#include "syn480_433.h"

// 全局指针
SYN480_Rx_ISR* g_pRx433 = nullptr;

SYN480_Rx_ISR::SYN480_Rx_ISR(GPIO_TypeDef* port, uint16_t pin) {
    _port = port;
    _pin = pin;
    _state = RX_WAIT_SYNC;
    g_pRx433 = this; // 把自己注册给全局指针
    
    // 初始化 GPIO 为输入
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    // ... 其他时钟判断
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    MX_TIM14_Init();
}

// 核心逻辑：每 50us 被调用一次
// 极简状态机，尽量少用 if-else，提高速度
void SYN480_Rx_ISR::tickISR() {
    // 读取当前电平 (1 或 0)
    uint8_t pinLevel = (_port->IDR & _pin) ? 1 : 0;
    
    _counter++; // 持续时间累加

    // 状态机
    if (_state == RX_WAIT_SYNC) {
        // 等待同步头：必须是长时间低电平，然后跳变到高电平
        if (pinLevel == 1) { 
            // 它是上升沿 (High now, was Low)
            // 检查刚才的低电平是否足够长
            if (_counter > TICK_SYNC_MIN) {
                // 找到同步头，准备接收数据
                _state = RX_READ_DATA;
                _raw_data = 0;
                _bit_index = 0;
                _counter = 0; // 重置计数器，开始测第一个高电平
            } else {
                // 噪音，重置
                _counter = 0;
            }
        }
    } 
    else if (_state == RX_READ_DATA) {
        // 我们需要测量：高电平宽度 -> 低电平宽度
        // 这里的逻辑简化：只检测"下降沿"来判断高电平宽度
        
        static uint32_t high_width = 0;

        // 如果当前变低了 (下降沿)，说明刚才那段高电平结束了
        if (pinLevel == 0 && high_width == 0) {
             high_width = _counter; // 记录高电平时间
             _counter = 0;          // 重置，开始测低电平
        }
        // 如果当前变高了 (上升沿)，说明刚才那段低电平结束了 -> 一个Bit结束
        else if (pinLevel == 1 && high_width != 0) {
            // 一个完整的 Bit 收完了 (High + Low)
            // 根据 high_width 判断是 0 还是 1
            
            // 50us per tick. 
            // Bit0 High ~350us (7 ticks). Range: 3-11
            // Bit1 High ~1050us (21 ticks). Range: 15-28
            
            if (high_width > 3 && high_width < 12) {
                // Bit 0
                _raw_data <<= 1;
            } else if (high_width > 15 && high_width < 28) {
                // Bit 1
                _raw_data = (_raw_data << 1) | 1;
            } else {
                // 错误的脉宽，复位
                _state = RX_WAIT_SYNC;
                high_width = 0;
                return;
            }

            _bit_index++;
            high_width = 0; // 清除，准备下一个位
            _counter = 0;   // 重置，准备测下一个高电平

            // 收满 24 位
            if (_bit_index >= 24) {
                _valid_data = _raw_data;
                _has_new_data = true;
                _state = RX_WAIT_SYNC; // 回到等待同步
            }
        }
    }
}

bool SYN480_Rx_ISR::available() {
    return _has_new_data;
}

uint32_t SYN480_Rx_ISR::read() {
    _has_new_data = false;
    return _valid_data;
}

extern "C" void Rx_Tick_Wrapper() {
    if (g_pRx433) g_pRx433->tickISR();
}