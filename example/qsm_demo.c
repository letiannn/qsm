/*
 * @Date: 2025-09-26 09:08:16
 * @LastEditTime: 2026-01-09 23:11:33
 * @FilePath: \rt-thread\lt_workspace\rtt_app_at32\rt-thread\components\ccmp\qsm\example\qsm_demo.c
 * @Description: 
 */
#include "qsm.h"
#include "stdint.h"
#include "stdio.h"

#define LOG_TAG "qsm_base_example"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

#define THREAD_PRIORITY   25
#define THREAD_STACK_SIZE 2048
#define THREAD_TIMESLICE  5

static void Test1RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *state_idx = (uint32_t *)user_data;
    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *state_idx);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run time %ld", self->state_idx, qsm->run_tick);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void Test2RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *state_idx = (uint32_t *)user_data;
    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *state_idx);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run time %ld", self->state_idx, qsm->run_tick);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void Test3RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *state_idx = (uint32_t *)user_data;
    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *state_idx);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run time %ld", self->state_idx, qsm->run_tick);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static uint32_t state_idx = 0;

static void qsm_task(void *arg)
{
    QStateMachine_t *qsm = QstateMachineCreate("test_sm");

    while (++state_idx < 500)
    {
        QstateMachineRunOnce(qsm);
        rt_thread_mdelay(10);
        if (state_idx % 100 == 0)
        {
            QstateMachineLoadState(qsm, 1, Test2RunOnce, &state_idx);
        }
        else if (state_idx % 100 == 30)
        {
            QstateMachineLoadState(qsm, 2, Test3RunOnce, &state_idx);
        }
        else if (state_idx % 100 == 60)
        {
            QstateMachineLoadState(qsm, 3, Test1RunOnce, &state_idx);
        }
    }
    QstateMachineDestroy(qsm);
}

static void qsm_base_example(void)
{
    rt_thread_t tid1 = rt_thread_create("thread1",
                                        qsm_task, RT_NULL,
                                        THREAD_STACK_SIZE,
                                        THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
}
MSH_CMD_EXPORT(qsm_base_example, qsm base example);
