#include "qsm.h"
#include "qsm_mgr.h"

#define LOG_TAG "qsm_netlink_mgr_example"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

#define THREAD_PRIORITY   25
#define THREAD_STACK_SIZE 2048
#define THREAD_TIMESLICE  5

static uint32_t idx = 0;

static void TNetWorkIdle(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkInit(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkLink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkWork(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkRelink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);

typedef enum eNetWorkStateTag
{
    eNetWorkState_Idle = 0,
    eNetWorkState_Init,
    eNetWorkState_Link,
    eNetWorkState_Work,
    eNetWorkState_Relink,
    eNetWorkState_Max,
} eNetWorkState_t;

QsmMgrTbl_t g_net_qsmmgr_tbl[] = {
    { eNetWorkState_Idle, TNetWorkIdle, eNetWorkState_Init, eNetWorkState_Idle },
    { eNetWorkState_Init, TNetWorkInit, eNetWorkState_Link, eNetWorkState_Idle },
    { eNetWorkState_Link, TNetWorkLink, eNetWorkState_Work, eNetWorkState_Init },
    { eNetWorkState_Work, TNetWorkWork, eNetWorkState_Relink, eNetWorkState_Relink },
    { eNetWorkState_Relink, TNetWorkRelink, eNetWorkState_Idle, eNetWorkState_Idle },
    { eNetWorkState_Max, NULL, 0, 0 },
};

static void TNetWorkIdle(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *(uint32_t *)user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run %ld", self->state_idx, *(uint32_t *)user_data);
        if (*(uint32_t *)user_data % 100 == 1)
        {
            QsmMgrLoadSucc(qsm->parent_);
        }
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void TNetWorkInit(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *(uint32_t *)user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run %ld", self->state_idx, *(uint32_t *)user_data);
        if (*(uint32_t *)user_data % 100 == 1)
        {
            QsmMgrLoadSucc(qsm->parent_);
        }
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void TNetWorkLink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *(uint32_t *)user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run %ld", self->state_idx, *(uint32_t *)user_data);
        if (*(uint32_t *)user_data % 100 == 1)
        {
            QsmMgrLoadSucc(qsm->parent_);
        }
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void TNetWorkWork(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *(uint32_t *)user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run %ld", self->state_idx, *(uint32_t *)user_data);
        if (*(uint32_t *)user_data % 100 == 1)
        {
            QsmMgrLoadFail(qsm->parent_);
        }
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void TNetWorkRelink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        LOG_I("%d create %ld", self->state_idx, *(uint32_t *)user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        LOG_I("%d run %ld", self->state_idx, *(uint32_t *)user_data);
        if (*(uint32_t *)user_data % 100 == 1)
        {
            QsmMgrLoadFail(qsm->parent_);
        }
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        LOG_I("%d destroy", self->state_idx);
    }
}

static void qsm_task(void *arg)
{
    QsmMgr_t *netlink_qsm = QsmMgrCreate("net_qsm", g_net_qsmmgr_tbl, eNetWorkState_Max, &idx);
    QsmMgrStart(netlink_qsm);
    while (++idx)
    {
        QsmMgrRunOnce(netlink_qsm);
        rt_thread_mdelay(10);
				if(idx == 0xFFFFFFF0)
				{
						LOG_I("run over time %ld", idx);
						break;
				}
    }
    QsmMgrDestroy(netlink_qsm);
}

static void qsm_mgr_example(void)
{
    rt_thread_t tid1 = rt_thread_create("thread1",
                                        qsm_task, RT_NULL,
                                        THREAD_STACK_SIZE,
                                        THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
}
MSH_CMD_EXPORT(qsm_mgr_example, qsm mgr example);


