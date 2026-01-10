#include "qsm_mgr.h"

#define LOG_TAG "qsm_mgr"
#define LOG_LVL LOG_LVL_INFO
#include <ulog.h>

QsmMgr_t *QsmMgrCreate(char *qsm_name, QsmMgrTbl_t *qsm_table, uint8_t qsm_table_size, void *user_data)
{
    if (qsm_table == RT_NULL || qsm_table_size == 0)
    {
        return RT_NULL;
    }

    QsmMgr_t *qsm_mgr = rt_malloc(sizeof(QsmMgr_t));
    if (qsm_mgr == RT_NULL)
    {
        QSM_ASSERT(qsm_mgr != RT_NULL);
        return RT_NULL;
    }

    qsm_mgr->qsm = QstateMachineCreate(qsm_name);
    if (qsm_mgr->qsm == RT_NULL)
    {
        QSM_ASSERT(qsm_mgr->qsm != RT_NULL);
        return RT_NULL;
    }

    qsm_mgr->qsm->parent_ = qsm_mgr;
    qsm_mgr->sm_state_idx = 0;
    qsm_mgr->table = qsm_table;
    qsm_mgr->table_size = qsm_table_size;
    qsm_mgr->user_data = user_data;

    return qsm_mgr;
}

QExectStateRst_t QsmMgrStart(QsmMgr_t *qsm_mgr)
{
    QSM_ASSERT(qsm_mgr != RT_NULL);
    QSM_ASSERT(qsm_mgr->qsm != RT_NULL);
    return QstateMachineLoadState(qsm_mgr->qsm, qsm_mgr->table[0].state_idx, qsm_mgr->table[0].method, qsm_mgr->user_data);
}

QExectStateRst_t QsmMgrLoadSucc(QsmMgr_t *qsm_mgr)
{
    QSM_ASSERT(qsm_mgr != RT_NULL);
    QSM_ASSERT(qsm_mgr->qsm != RT_NULL);

    QExectStateRst_t rst = QExectStateRst_Fail;

    QsmMgrTbl_t *curr_tbl = &qsm_mgr->table[qsm_mgr->sm_state_idx];
    uint8_t next_state_idx = curr_tbl->succ_state_idx;

    if (next_state_idx < qsm_mgr->table_size)
    {
        rst = QstateMachineLoadState(qsm_mgr->qsm, qsm_mgr->table[next_state_idx].state_idx, qsm_mgr->table[next_state_idx].method, qsm_mgr->user_data);
        if (rst == QExectStateRst_Success)
        {
            qsm_mgr->sm_state_idx = next_state_idx;
        }
    }

    return rst;
}

QExectStateRst_t QsmMgrLoadFail(QsmMgr_t *qsm_mgr)
{
    QSM_ASSERT(qsm_mgr != RT_NULL);
    QSM_ASSERT(qsm_mgr->qsm != RT_NULL);

    QExectStateRst_t rst = QExectStateRst_Fail;

    QsmMgrTbl_t *curr_tbl = &qsm_mgr->table[qsm_mgr->sm_state_idx];
    uint8_t next_state_idx = curr_tbl->fail_state_idx;

    if (next_state_idx < qsm_mgr->table_size)
    {
        rst = QstateMachineLoadState(qsm_mgr->qsm, qsm_mgr->table[next_state_idx].state_idx, qsm_mgr->table[next_state_idx].method, qsm_mgr->user_data);
        if (rst == QExectStateRst_Success)
        {
            qsm_mgr->sm_state_idx = next_state_idx;
        }
    }

    return rst;
}

void QsmMgrRunOnce(QsmMgr_t *qsm_mgr)
{
    if (qsm_mgr != RT_NULL)
    {
        QstateMachineRunOnce(qsm_mgr->qsm);
    }
}

void QsmMgrDestroy(QsmMgr_t *qsm_mgr)
{
    if (qsm_mgr != RT_NULL)
    {
        QstateMachineDestroy(qsm_mgr->qsm);
        rt_free(qsm_mgr);
    }
}
