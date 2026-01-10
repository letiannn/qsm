/**
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2026-01-09 13:14:19
 * @FilePath: \rt-thread\components\ccmp\qsm\qsm.h
 * @Description: 
 */
#ifndef _QSM_H_
#define _QSM_H_

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QSM_DEBUG_ENABLE
#define QSM_MACHINE_NAME_LEN  (RT_NAME_MAX)
#define QSM_DEFAYLT_STATE_NUM (2)

#define QSM_ASSERT(expr) RT_ASSERT(expr)

typedef enum QExectStateRst QExectStateRst_t;
typedef enum QExectState QExectState_t;
typedef struct QStateObj QStateObj_t;
typedef struct QStateMachine QStateMachine_t;

typedef void (*QstateObjRunOnceFptr)(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
typedef QExectStateRst_t (*QStateMachineSetCurrStateFptr)(QStateMachine_t *self, QStateObj_t *state);
typedef QStateObj_t *(*QStateMachineGetCurrStateFptr)(QStateMachine_t *self);
typedef QExectStateRst_t (*QStateMachineSetNextStateFptr)(QStateMachine_t *self, QStateObj_t *state);
typedef QStateObj_t *(*QStateMachineGetNextStateFptr)(QStateMachine_t *self);
typedef QExectStateRst_t (*QStateMachineSetExectStateFptr)(QStateMachine_t *self, QExectState_t exect_state);
typedef QExectState_t (*QStateMachineGetExectStateFptr)(QStateMachine_t *self);

enum QExectStateRst
{
    QExectStateRst_Success = 0,                         //成功
    QExectStateRst_Fail,                                //失败
};

enum QExectState
{
    QExectState_Idle,                                   //空闲状态
    QExectState_Init,                                   //初始
    QExectState_Run,                                    //运行
    QExectState_Over,                                   //结束
};

struct QStateObj
{
    void *parent_;                                      //父类
    uint8_t state_idx;                                  //状态机索引
    QstateObjRunOnceFptr RunOnce;                       //状态机运行回调
    void *user_data;                                    //用户数据
};

struct QStateMachine
{
    void *parent_;                                       //父类
    char name[QSM_MACHINE_NAME_LEN];                     //状态机名称
    rt_mp_t state_mpool;                                 //状态机对象池
    rt_tick_t init_tick;                                 //初始化时间 
    rt_tick_t run_tick;                                  //运行时间
    QExectState_t exect_state;                           //执行器的状态
    QStateObj_t *curr_state;                             //当前状态
    QStateObj_t *next_state;                             //上一个状态
    QStateMachineSetCurrStateFptr SetCurrState;          //设置当前状态机
    QStateMachineGetCurrStateFptr GetCurrState;          //获取当前状态机
    QStateMachineSetNextStateFptr SetNextState;          //设置下一个状态机
    QStateMachineGetNextStateFptr GetNextState;          //获取下一个状态机
    QStateMachineSetExectStateFptr SetExectState;        //设置执行中的状态机
    QStateMachineGetExectStateFptr GetExectState;        //获取执行中的状态机
};

QStateMachine_t *QstateMachineCreate(char *name);

QExectStateRst_t QstateMachineLoadState(QStateMachine_t *self, uint8_t state_idx, QstateObjRunOnceFptr method, void *user_data);

void QstateMachineRunOnce(QStateMachine_t *self);

void QstateMachineDestroy(QStateMachine_t *self);

#ifdef __cplusplus
}
#endif

#endif
