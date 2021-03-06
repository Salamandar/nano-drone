#pragma once

#include "hardware/hardware.h"

#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configCPU_CLOCK_HZ              (RCC_CLOCK_FREQ_HZ)
#define configTICK_RATE_HZ              ((TickType_t) 1000)
#define configMAX_PRIORITIES            ((unsigned portBASE_TYPE)3)
#define configMINIMAL_STACK_SIZE        ((unsigned short)32)
#define configTOTAL_HEAP_SIZE           ((size_t)(3 * 1024))
#define configMAX_TASK_NAME_LEN         (2)
#define configUSE_TRACE_FACILITY        0
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         1
#define configUSE_MUTEXES               1
#define configQUEUE_REGISTRY_SIZE       8
#define configCHECK_FOR_STACK_OVERFLOW  0
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_MALLOC_FAILED_HOOK    0
// void vApplicationMallocFailedHook(void);
#define configUSE_APPLICATION_TASK_TAG  0
#define configUSE_COUNTING_SEMAPHORES   1
#define configGENERATE_RUN_TIME_STATS   0
// #define configSUPPORT_DYNAMIC_ALLOCATION 0
// #define configSUPPORT_STATIC_ALLOCATION 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

/* Software timer definitions. */
#define configUSE_TIMERS                0
#define configTIMER_TASK_PRIORITY       (2)
#define configTIMER_QUEUE_LENGTH        10
#define configTIMER_TASK_STACK_DEPTH    (configMINIMAL_STACK_SIZE * 4)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskCleanUpResources   0
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelayUntil         0
#define INCLUDE_vTaskDelay              1
#define INCLUDE_xTaskGetSchedulerState  1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

// Definitions that map FreeRTOS Port interrupts to libopencm3 vectors
// If you need to handle these yourself, comment these out and be sure to call them
#define vPortSVCHandler         sv_call_handler
#define xPortPendSVHandler      pend_sv_handler
// #define xPortSysTickHandler     sys_tick_handler
