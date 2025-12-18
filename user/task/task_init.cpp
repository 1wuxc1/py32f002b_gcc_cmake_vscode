#include "task_init.h"
uint32_t _433_send_task_time = 1000;
uint32_t _433_receive_task_time = 100;

task_init::task_init(/* args */)
{
    _433_send_task _433_send;
    _433_send.Start("433_send_task", 128, 1, &_433_send_task_time);
    // _433_receive_task _433_receive;
    // _433_receive.Start("433_receive_task", 128, 1, &_433_receive_task_time);

    // 创建任务
    vTaskStartScheduler();
    while (1)
    {
        /* code */
    }
    
}