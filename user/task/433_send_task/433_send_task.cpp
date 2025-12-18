#include "433_send_task.h"
#include "delay.h"
_433_send_task::_433_send_task(/* args */)
{
    // 创建任务
    // xTaskCreate(Oled_task::Task, "Task1", 128, NULL, 2, &task_handle);
}
void _433_send_task::Start(const char *name, uint32_t stack_depth, uint32_t priority, uint32_t *time)
{
    xTaskCreate(_433_send_task::Task, name, stack_depth, (void *)time, priority, &task_handle);
}
void _433_send_task::Start(const char *name)
{
    xTaskCreate(_433_send_task::Task, name, 128, NULL, 2, &task_handle);
}
_433_send_task::~_433_send_task()
{
    vTaskDelete(task_handle);
}
void _433_send_task::Task(void *p)
{
    // uint8_t i = 0;
    SY115_Tx sy115_tx(GPIOA,GPIO_PIN_4,Delay_Us);
    sy115_tx.begin();
    while (1)
    {
        // i++;
        // OLED_DrawPixel(i, i, 1);
        // OLED_Update();
        sy115_tx.send(0x12345678,4);
        if (p != NULL)
        {
            vTaskDelay(pdMS_TO_TICKS(*(uint32_t *)p));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(_433_SEND_TASK_TIME));
        }
    }
}
