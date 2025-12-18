#ifndef __433_RECEIVE_TASK_H__
#define __433_RECEIVE_TASK_H__




#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#define OLED_TASK_TIME 1000

class _433_receive_task
{
public:
    TaskHandle_t task_handle;
    _433_receive_task(/* args */);
    void Start(const char *name, uint32_t stack_depth, uint32_t priority, uint32_t *time);
    void Start(const char *name);
    ~_433_receive_task();

private:
    static void Task(void *p);
};

#endif