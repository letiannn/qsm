#include "qsm_netlink.h"
#include "esp_log.h"

#define TAG "sm_netlink"

#define QSM_NETLINK_IDLE "0_Idle"
#define QSM_NETLINK_INIT "1_netinit"
#define QSM_NETLINK_CONN "2_netlink"
#define QSM_NETLINK_WORK "3_network"
#define QSM_NETLINK_RELINK "4_netrelink"

static void TNetWorkIdle(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkInit(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkLink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkWork(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
static void TNetWorkRelink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);

QsmManageTable_t g_qsm_network_table[] =
{
    {0, QSM_NETLINK_IDLE,     TNetWorkIdle,   QSM_NETLINK_INIT,     QSM_NETLINK_RELINK  },
    {1, QSM_NETLINK_INIT,     TNetWorkInit,   QSM_NETLINK_CONN,     QSM_NETLINK_IDLE    },
    {2, QSM_NETLINK_CONN,     TNetWorkLink,   QSM_NETLINK_WORK,     QSM_NETLINK_INIT    },
    {3, QSM_NETLINK_WORK,     TNetWorkWork,   QSM_NETLINK_RELINK,   QSM_NETLINK_CONN    },
    {4, QSM_NETLINK_RELINK,   TNetWorkRelink, QSM_NETLINK_IDLE,     QSM_NETLINK_WORK    },
};

uint8_t g_qsm_network_num = sizeof(g_qsm_network_table) / sizeof(g_qsm_network_table[0]); 

static void TNetWorkIdle(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *(uint32_t *)user_data);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        ESP_LOGI(TAG, "%s run", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        ESP_LOGI(TAG, "%s destroy", self->state_name);
    }
}

static void TNetWorkInit(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *(uint32_t *)user_data);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        ESP_LOGI(TAG, "%s run", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        ESP_LOGI(TAG, "%s destroy", self->state_name);
    }
}

static void TNetWorkLink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *(uint32_t *)user_data);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        ESP_LOGI(TAG, "%s run", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        ESP_LOGI(TAG, "%s destroy", self->state_name);
    }
}

static void TNetWorkWork(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *(uint32_t *)user_data);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        ESP_LOGI(TAG, "%s run", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        ESP_LOGI(TAG, "%s destroy", self->state_name);
    }
}

static void TNetWorkRelink(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *(uint32_t *)user_data);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        ESP_LOGI(TAG, "%s run", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        ESP_LOGI(TAG, "%s destroy", self->state_name);
    }
}



