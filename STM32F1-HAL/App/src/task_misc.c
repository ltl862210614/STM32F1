#include "FreeRTOS.h"
#include "task_manage.h"

//#define TASK_MISC_NAME "taskMisc"
//#define TASK_MISC_STACK ( configMINIMAL_STACK_SIZE * 2 )
//#define TASK_MISC_PRIORITY ( tskIDLE_PRIORITY + 2 )

static void task_misc(void* param);

taskDef_t gtTaskMisc = {
    .task_id = TASK_ID_MISC,
    .task_name = "taskMisc",
    .stack_depth = 256,
    .task_handle = NULL,
    .task_priority = 1,
    .task_func = task_misc,
};

void task_misc_init(void)
{
    task_create(&gtTaskMisc);
}

void task_misc_deInit(void)
{
	vTaskDelete(gtTaskMisc.task_handle);
}

static void task_misc(void* param)
{
	UNUSED(param);
	
	while (1)
	{
		
		vTaskDelay(2000/portTICK_RATE_MS);
	}
}
