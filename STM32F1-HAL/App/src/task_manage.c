#include <stdio.h>

#include "task_manage.h"

int task_create(taskDef_t *task)
{
	if (NULL == task)
	{
		return -1;
	}

	int iRet = xTaskCreate(task->task_func,
                        task->task_name,
                        task->stack_depth,
                        &task->task_id,
                        task->task_priority,
                        &task->task_handle);
	if (pdPASS != iRet)
	{
		printf("create task %s failed\r\n", task->task_name);
		return -2;
	}

	return 0;				
}

