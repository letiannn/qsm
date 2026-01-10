/*** 
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2026-01-09 22:53:10
 * @FilePath: \rt-thread\lt_workspace\rtt_app_at32\rt-thread\components\ccmp\qsm\qsm_mgr.h
 * @Description: 
 */
#ifndef _QSM_MANAGE_
#define _QSM_MANAGE_

#include "qsm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct QsmMgrTbl QsmMgrTbl_t;
typedef struct QsmMgr QsmMgr_t;
struct QsmMgrTbl
{
    uint8_t state_idx;              //索引
    QstateObjRunOnceFptr method;    //状态机方法
    uint8_t succ_state_idx;         //成功转跳索引
    uint8_t fail_state_idx;         //失败转跳索引
};

struct QsmMgr
{
    QStateMachine_t *qsm;           //状态机
    uint8_t sm_state_idx;           //当前状态机索引
    QsmMgrTbl_t *table;             //状态机表
    uint8_t table_size;             //状态机表大小
    void *user_data;                //用户数据
};

QsmMgr_t *QsmMgrCreate(char *qsm_name, QsmMgrTbl_t *qsm_table, uint8_t qsm_table_size, void *user_data);

QExectStateRst_t QsmMgrStart(QsmMgr_t *qsm_manage);

QExectStateRst_t QsmMgrLoadSucc(QsmMgr_t *qsm_manage);

QExectStateRst_t QsmMgrLoadFail(QsmMgr_t *qsm_manage);

void QsmMgrRunOnce(QsmMgr_t *qsm_manage);

void QsmMgrDestroy(QsmMgr_t *qsm_manage);

#ifdef __cplusplus
}
#endif

#endif
