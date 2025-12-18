#include "sy115_433.h"

// 构造函数：绑定引脚和延时函数
SY115_Tx::SY115_Tx(GPIO_TypeDef* port, uint16_t pin, void (*delay_func)(uint32_t)) {
    _port = port;
    _pin = pin;
    _delay_us = delay_func;
}

// 初始化 GPIO 为推挽输出
void SY115_Tx::begin() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 开启对应的 GPIO 时钟 (这里需要根据实际Port判断，或者在外部main中开启)
    // 建议在 main.c 中统一开启时钟，或者在这里添加判断逻辑
    if (_port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (_port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    // if (_port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE(); // PY32F002B 可能只有 A/B

    // 配置为推挽输出
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET); // 默认拉低
    GPIO_InitStruct.Pin = _pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

// 发送前导码 (Preamble): High 1LCK, Low 31LCK
void SY115_Tx::sendPreamble() {
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    _delay_us(EV1527_LCK);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    _delay_us(EV1527_LCK * 31);
}

// 发送逻辑 0: High 1LCK, Low 3LCK
void SY115_Tx::sendBit0() {
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    _delay_us(EV1527_LCK);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    _delay_us(EV1527_LCK * 3);
}

// 发送逻辑 1: High 3LCK, Low 1LCK
void SY115_Tx::sendBit1() {
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
    _delay_us(EV1527_LCK * 3);
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
    _delay_us(EV1527_LCK);
}

// 核心发送函数
void SY115_Tx::send(uint32_t data, uint8_t repeat) {
    for (uint8_t r = 0; r < repeat; r++) {
        // 1. 发送同步前导码
        sendPreamble();

        // 2. 发送24位数据 (高位在前 MSB First)
        for (int i = 23; i >= 0; i--) {
            if ((data >> i) & 0x01) {
                sendBit1();
            } else {
                sendBit0();
            }
        }
        
        // 可选：帧间隔，防止连发太快接收端解不过来
        // _delay_us(EV1527_LCK * 10); 
    }
    // 发送完毕确保拉低
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
}

