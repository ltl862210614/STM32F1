#ifndef __TASK_MANAGE_H__
#define __TASK_MANAGE_H__

#include "FreeRTOS.h"
#include "task.h"

#define UNUSED(para) (void)(para)

typedef enum {
    TASK_ID_WDOG,
    TASK_ID_MISC,
    TASK_ID_UPGRADE,

    TASK_ID_MAX
}taskId_e;

typedef struct _taskDef_t
{
    uint32_t          task_id;
    const char        *task_name;
    uint16_t          stack_depth;
    TaskHandle_t      task_handle;
    uint32_t          task_priority;
    void              (*task_func)(void *);	
}taskDef_t;

int task_create(taskDef_t *task);
#endif
