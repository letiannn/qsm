/*
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2025-10-09 09:14:05
 * @FilePath: /brd_project/main/qsm.c
 * @Description: 
 */
#include "qsm.h"

static QExectStateRst_t QStateMachineSetCurrState(QStateMachine_t *self, QStateObj_t *state)
{
    QSM_ASSERT(self != NULL);
    self->curr_state = state;
    return QExectStateRst_Success;
}

static QStateObj_t* QStateMachineGetCurrState(QStateMachine_t *self)
{
    QSM_ASSERT(self != NULL);
    return self->curr_state;
}

static QExectStateRst_t QStateMachineSetNextState(QStateMachine_t *self, QStateObj_t *state)
{
    QSM_ASSERT(self != NULL);
    self->next_state = state;
    return QExectStateRst_Success;
}

static QStateObj_t* QStateMachineGetNextState(QStateMachine_t *self)
{
    QSM_ASSERT(self != NULL);
    return self->next_state;
}

static QExectStateRst_t QStateMachineSetExectState(QStateMachine_t *self, QExectState_t exect_state)
{
    QSM_ASSERT(self != NULL);
    self->exect_state = exect_state;
    return QExectStateRst_Success;
}

static QExectState_t QStateMachineGetExectState(QStateMachine_t *self)
{
    QSM_ASSERT(self != NULL);
    return self->exect_state;
}

static QStateObj_t *QStateObjCreate(const char *name, QstateObjRunOnceFptr method, void *user_data)
{
    QSM_ASSERT(name != NULL);
    QSM_ASSERT(method != NULL);

    QStateObj_t *self = (QStateObj_t *)malloc(sizeof(QStateObj_t));
    if(self == NULL)
    {
        return NULL;
    }
    
    self->state_name = name;
    self->RunOnce = method;
    self->user_data = user_data;
    
    return self;
}

static void QstateObjDestory(QStateObj_t *self)
{
    QSM_ASSERT(self != NULL);
    if(self != NULL)
    {
        free(self);
    }
}

QStateMachine_t *QstateMachineCreate(void)
{
    QStateMachine_t *self = (QStateMachine_t *)malloc(sizeof(QStateMachine_t));
    QSM_ASSERT(self != NULL);
    if(self == NULL)
    {
        return NULL;
    }

    self->exect_state = QExectState_Idle;
    self->curr_state = NULL;
    self->next_state = NULL;

    self->SetCurrState      = QStateMachineSetCurrState;   
    self->GetCurrState      = QStateMachineGetCurrState;  
    self->SetNextState      = QStateMachineSetNextState;   
    self->GetNextState      = QStateMachineGetNextState;  
    self->SetExectState     = QStateMachineSetExectState;  
    self->GetExectState     = QStateMachineGetExectState; 

    return self;
}

QExectStateRst_t QstateMachineLoadState(QStateMachine_t *self, const char *name, QstateObjRunOnceFptr method, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(name != NULL);
    QSM_ASSERT(method != NULL);

    QExectStateRst_t rst = QExectStateRst_Success;

    QStateObj_t *new_state = QStateObjCreate(name, method, user_data);
    if(new_state == NULL)
    {
        return QExectStateRst_Fail;
    }
    
    self->SetNextState(self, new_state);

    return rst;
}

void QstateMachineRunOnce(QStateMachine_t *self)
{
    QSM_ASSERT(self != NULL);
    if(self == NULL)
    {
        return;
    }

    if(self->GetNextState(self) != NULL)
    {
        QStateObj_t *curr_state = NULL;
        if(self->GetNextState(self) != self->GetCurrState(self))
        {
            curr_state = self->GetCurrState(self);
            if(curr_state != NULL)
            {
                self->SetExectState(self, QExectState_Over);
                curr_state->RunOnce(curr_state, self, curr_state->user_data);
                QstateObjDestory(self->GetCurrState(self));
            }
            
            self->SetExectState(self, QExectState_Init);
            self->SetCurrState(self, self->GetNextState(self));
        }
        else 
        {
            self->SetExectState(self, QExectState_Run);
        }
        
        curr_state = self->GetCurrState(self);
        if(curr_state != NULL && curr_state->RunOnce != NULL)
        {
            curr_state->RunOnce(curr_state, self, curr_state->user_data);
        }
    }
}

void QstateMachineDestroy(QStateMachine_t *self)
{
    QSM_ASSERT(self != NULL);
    if(self != NULL)
    {
        QStateObj_t *curr_state = self->GetCurrState(self);
        if(curr_state != NULL)
        {
            self->SetExectState(self, QExectState_Over);
            curr_state->RunOnce(curr_state, self, curr_state->user_data);
            QstateObjDestory(self->GetCurrState(self));
        }
        free(self);
    }
}
