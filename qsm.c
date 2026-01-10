/*
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2026-01-09 23:08:28
 * @FilePath: \rt-thread\lt_workspace\rtt_app_at32\rt-thread\components\ccmp\qsm\qsm.c
 * @Description: 
 */
#include "qsm.h"

static QExectStateRst_t QStateMachineSetCurrState(QStateMachine_t *self, QStateObj_t *state)
{
    QSM_ASSERT(self != RT_NULL);
    self->curr_state = state;
    return QExectStateRst_Success;
}

static QStateObj_t *QStateMachineGetCurrState(QStateMachine_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    return self->curr_state;
}

static QExectStateRst_t QStateMachineSetNextState(QStateMachine_t *self, QStateObj_t *state)
{
    QSM_ASSERT(self != RT_NULL);
    self->next_state = state;
    return QExectStateRst_Success;
}

static QStateObj_t *QStateMachineGetNextState(QStateMachine_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    return self->next_state;
}

static QExectStateRst_t QStateMachineSetExectState(QStateMachine_t *self, QExectState_t exect_state)
{
    QSM_ASSERT(self != RT_NULL);
    self->exect_state = exect_state;
    return QExectStateRst_Success;
}

static QExectState_t QStateMachineGetExectState(QStateMachine_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    return self->exect_state;
}

static QStateObj_t *QStateObjCreate(QStateMachine_t *machine, uint8_t state_idx, QstateObjRunOnceFptr method, void *user_data)
{
    QSM_ASSERT(machine != RT_NULL);
    QSM_ASSERT(method != RT_NULL);

    QStateObj_t *self = (QStateObj_t *)rt_mp_alloc(machine->state_mpool, RT_WAITING_FOREVER);
    if (self == RT_NULL)
    {
        return RT_NULL;
    }

    self->parent_ = machine;
    self->state_idx = state_idx;
    self->RunOnce = method;
    self->user_data = user_data;

    return self;
}

static void QstateObjDestory(QStateObj_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    if (self != RT_NULL)
    {
        rt_mp_free(self);
    }
}

QStateMachine_t *QstateMachineCreate(char *name)
{
    QStateMachine_t *self = (QStateMachine_t *)rt_malloc(sizeof(QStateMachine_t));
    QSM_ASSERT(self != RT_NULL);
    if (self == RT_NULL)
    {
        return RT_NULL;
    }

    self->state_mpool = rt_mp_create(name, QSM_DEFAYLT_STATE_NUM, sizeof(QStateObj_t));
    if (self->state_mpool == RT_NULL)
    {
        rt_free(self);
        return RT_NULL;
    }

    if (name)
    {
        rt_size_t obj_name_len = rt_strlen(name);
        if (obj_name_len > QSM_MACHINE_NAME_LEN - 1)
        {
            QSM_ASSERT(obj_name_len <= QSM_MACHINE_NAME_LEN - 1);
        }
        rt_memcpy(self->name, name, obj_name_len);
        self->name[obj_name_len] = '\0';
    }
    else
    {
        self->name[0] = '\0';
    }

    self->parent_ = RT_NULL;
    self->exect_state = QExectState_Idle;
    self->curr_state = RT_NULL;
    self->next_state = RT_NULL;

    self->SetCurrState = QStateMachineSetCurrState;
    self->GetCurrState = QStateMachineGetCurrState;
    self->SetNextState = QStateMachineSetNextState;
    self->GetNextState = QStateMachineGetNextState;
    self->SetExectState = QStateMachineSetExectState;
    self->GetExectState = QStateMachineGetExectState;

    return self;
}

QExectStateRst_t QstateMachineLoadState(QStateMachine_t *self, uint8_t state_idx, QstateObjRunOnceFptr method, void *user_data)
{
    QSM_ASSERT(self != RT_NULL);
    QSM_ASSERT(method != RT_NULL);

    QExectStateRst_t rst = QExectStateRst_Success;

    QStateObj_t *new_state = QStateObjCreate(self, state_idx, method, user_data);
    if (new_state == RT_NULL)
    {
        return QExectStateRst_Fail;
    }

    self->SetNextState(self, new_state);

    return rst;
}

void QstateMachineRunOnce(QStateMachine_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    if (self == RT_NULL)
    {
        return;
    }

    if (self->GetNextState(self) != RT_NULL)
    {
        QStateObj_t *curr_state = RT_NULL;
        if (self->GetNextState(self) != self->GetCurrState(self))
        {
            curr_state = self->GetCurrState(self);
            if (curr_state != RT_NULL)
            {
                self->SetExectState(self, QExectState_Over);
                curr_state->RunOnce(curr_state, self, curr_state->user_data);
                QstateObjDestory(self->GetCurrState(self));
            }

            self->SetExectState(self, QExectState_Init);
            self->SetCurrState(self, self->GetNextState(self));
            self->init_tick = rt_tick_get_millisecond();
        }
        else
        {
            self->SetExectState(self, QExectState_Run);
        }

        curr_state = self->GetCurrState(self);
        if (curr_state != RT_NULL && curr_state->RunOnce != RT_NULL)
        {
            self->run_tick = rt_tick_get_millisecond() - self->init_tick;
            curr_state->RunOnce(curr_state, self, curr_state->user_data);
        }
    }
}

void QstateMachineDestroy(QStateMachine_t *self)
{
    QSM_ASSERT(self != RT_NULL);
    if (self != RT_NULL)
    {
        QStateObj_t *curr_state = self->GetCurrState(self);
        if (curr_state != RT_NULL)
        {
            self->SetExectState(self, QExectState_Over);
            curr_state->RunOnce(curr_state, self, curr_state->user_data);
            QstateObjDestory(self->GetCurrState(self));
        }
        rt_mp_delete(self->state_mpool);
        rt_memset(self->name, 0, QSM_MACHINE_NAME_LEN);
        rt_free(self);
    }
}

