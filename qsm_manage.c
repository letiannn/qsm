#include "qsm_manage.h"
#include "esp_log.h"

#define TAG "qsm_manage"

QsmManage_t *QsmManageCreate(QsmManageTable_t *qsm_table, uint8_t qsm_table_size, void *user_data)
{
    if(qsm_table == NULL)
    {
        return NULL;
    }

    QsmManage_t *qsm_manage = malloc(sizeof(QsmManage_t));
    if(qsm_manage == NULL)
    {
        return NULL;
    }

    qsm_manage->qsm = QstateMachineCreate();
    if(qsm_manage->qsm == NULL)
    {
        return NULL;
    }

    qsm_manage->sm_idx = 0;
    qsm_manage->table = qsm_table;
    qsm_manage->table_size = qsm_table_size;
    qsm_manage->user_data = user_data;

    return qsm_manage;
}

QExectStateRst_t QsmManageStart(QsmManage_t *qsm_manage)
{
    QSM_ASSERT(qsm_manage != NULL);
    QSM_ASSERT(qsm_manage->qsm != NULL);
    return QstateMachineLoadState(qsm_manage->qsm, qsm_manage->table[0].name, qsm_manage->table[0].method, qsm_manage->user_data);
}

QExectStateRst_t QsmManageLoadSucc(QsmManage_t *qsm_manage)
{
    QSM_ASSERT(qsm_manage != NULL);
    QSM_ASSERT(qsm_manage->qsm != NULL);

    QExectStateRst_t rst = QExectStateRst_Fail;
    for(int i = 0; i < qsm_manage->table_size; i++)
    {
        QsmManageTable_t *qsm_item = (QsmManageTable_t *)&qsm_manage->table[i];
        QSM_ASSERT(qsm_item != NULL);
        QSM_ASSERT(qsm_item->method != NULL);

        if(qsm_item->succ_loadname == NULL && qsm_manage->sm_idx == i)
        {
            ESP_LOGE(TAG, "load succ state not find");
            rst = QExectStateRst_Fail;  
            break;
        }
        else if(qsm_manage->table[qsm_manage->sm_idx].succ_loadname != NULL && qsm_item->name != NULL)
        {
            if(strcmp(qsm_manage->table[qsm_manage->sm_idx].succ_loadname, qsm_item->name) == 0)
            {
                QstateMachineLoadState(qsm_manage->qsm, qsm_item->name, qsm_item->method, qsm_manage->user_data);
                ESP_LOGI(TAG, "load succ state %s to %s", qsm_manage->table[qsm_manage->sm_idx].name, qsm_item->name);
                qsm_manage->sm_idx = qsm_item->idx;
                rst = QExectStateRst_Success;     
                break;           
            }
        }
    }
    return rst;
}

QExectStateRst_t QsmManageLoadFail(QsmManage_t *qsm_manage)
{
    QSM_ASSERT(qsm_manage != NULL);
    QSM_ASSERT(qsm_manage->qsm != NULL);

    QExectStateRst_t rst = QExectStateRst_Fail;
    for(int i = 0; i < qsm_manage->table_size; i++)
    {
        QsmManageTable_t *qsm_item = (QsmManageTable_t *)&qsm_manage->table[i];
        QSM_ASSERT(qsm_item != NULL);
        QSM_ASSERT(qsm_item->method != NULL);

        if(qsm_item->fail_loadname == NULL && qsm_manage->sm_idx == i)
        {
            ESP_LOGE(TAG, "load fail state not find");
            rst = QExectStateRst_Fail;  
            break;
        }
        else if(qsm_manage->table[qsm_manage->sm_idx].fail_loadname != NULL && qsm_item->name != NULL)
        {
            if(strcmp(qsm_manage->table[qsm_manage->sm_idx].fail_loadname, qsm_item->name) == 0)
            {
                QstateMachineLoadState(qsm_manage->qsm, qsm_item->name, qsm_item->method, qsm_manage->user_data);
                ESP_LOGI(TAG, "load fail state %s to %s", qsm_manage->table[qsm_manage->sm_idx].name, qsm_item->name);
                qsm_manage->sm_idx = qsm_item->idx;
                rst = QExectStateRst_Success;     
                break;           
            }
        }
    }
    return rst;
}

void QsmManageRunOnce(QsmManage_t *qsm_manage)
{
    if(qsm_manage != NULL)
    {
        QstateMachineRunOnce(qsm_manage->qsm);
    }
}

void QsmManageDestroy(QsmManage_t *qsm_manage)
{
    if(qsm_manage != NULL)
    {
        QstateMachineDestroy(qsm_manage->qsm);
        free(qsm_manage);
    }
}
