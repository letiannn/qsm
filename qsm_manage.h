/*** 
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2025-10-09 09:06:07
 * @FilePath: /brd_project/main/qsm_manage.h
 * @Description: 
 */
#ifndef _QSM_MANAGE_
#define _QSM_MANAGE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qsm.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct QsmManageTable QsmManageTable_t;
typedef struct QsmManage QsmManage_t;

struct QsmManageTable
{
    uint8_t     idx;                //索引
    const char  *name;              //状态机名称    
    QstateObjRunOnceFptr method;    //状态机方法
    const char  *succ_loadname;     //成功运行跳转状态机名称
    const char  *fail_loadname;     //失败运行跳转状态机名称
};

struct QsmManage
{
    QStateMachine_t *qsm;       //状态机
    uint8_t sm_idx;             //当前状态机索引  
    QsmManageTable_t *table;    //状态机表
    uint8_t table_size;         //状态机表大小
    void *user_data;            //用户数据
};

QsmManage_t *QsmManageCreate(QsmManageTable_t *qsm_table, uint8_t qsm_table_size, void *user_data);

QExectStateRst_t QsmManageStart(QsmManage_t *qsm_manage);

QExectStateRst_t QsmManageLoadSucc(QsmManage_t *qsm_manage);

QExectStateRst_t QsmManageLoadFail(QsmManage_t *qsm_manage);

void QsmManageRunOnce(QsmManage_t *qsm_manage);

void QsmManageDestroy(QsmManage_t *qsm_manage);

#ifdef __cplusplus
}
#endif

#endif
