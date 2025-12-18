#include "433_receive_task.h"
#include "syn480_433.h"
#include "hal_i2c.h"

_433_receive_task::_433_receive_task(/* args */)
{
    // 创建任务
    // xTaskCreate(_433_receive_task::Task, "Task1", 128, NULL, 2, &task_handle);
}
void _433_receive_task::Start(const char *name, uint32_t stack_depth, uint32_t priority, uint32_t *time)
{
    xTaskCreate(_433_receive_task::Task, name, stack_depth, (void *)time, priority, &task_handle);
}
void _433_receive_task::Start(const char *name)
{
    xTaskCreate(_433_receive_task::Task, name, 128, NULL, 2, &task_handle);
}
_433_receive_task::~_433_receive_task()
{
    vTaskDelete(task_handle);
}
void _433_receive_task::Task(void *p)
{
    SYN480_Rx_ISR rx_data(GPIOA, GPIO_PIN_4);
    I2cInit();   // 初始化 I2C
    OLED_Init(); // 初始化 OLED

    while (1)
    {
        // i++;
        // OLED_DrawPixel(i, i, 1);
        // OLED_Update();
        if (rx_data.available() == true)
        {
            OLED_ShowNum(0, 0, rx_data.read(), 8, 12);
        }
        else
        {
            
        }

        if (p != NULL)
        {
            vTaskDelay(pdMS_TO_TICKS(*(uint32_t *)p));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(OLED_TASK_TIME));
        }
    }
}
