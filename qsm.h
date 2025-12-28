/*** 
 * @Date: 2025-09-26 03:00:44
 * @LastEditTime: 2025-10-09 09:15:34
 * @FilePath: /brd_project/main/qsm.h
 * @Description: 
 */
#ifndef _QSM_H_
#define _QSM_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif 

// #define QSM_NDEBUG

static void assert_fail(const char *expr, const char *file, int line) {
    fprintf(stderr, "debug qsm assert failed: [%s], file [%s], line [%d]\n", expr, file, line);
    abort();
}

#ifdef QSM_NDEBUG
    #define QSM_ASSERT(expr) \
    do { \
    } while (0)
#else
    #define QSM_ASSERT(expr) \
        ((expr) ? (void)0 : assert_fail(#expr, __FILE__, __LINE__))
#endif


typedef enum    QExectStateRst  QExectStateRst_t;
typedef enum    QExectState     QExectState_t;
typedef struct  QStateObj       QStateObj_t;
typedef struct  QStateMachine   QStateMachine_t;

typedef void                (*QstateObjRunOnceFptr)(QStateObj_t *self, QStateMachine_t *qsm, void *user_data);
typedef QExectStateRst_t    (*QStateMachineSetCurrStateFptr)(QStateMachine_t *self, QStateObj_t *state);
typedef QStateObj_t*        (*QStateMachineGetCurrStateFptr)(QStateMachine_t *self);
typedef QExectStateRst_t    (*QStateMachineSetNextStateFptr)(QStateMachine_t *self, QStateObj_t *state);
typedef QStateObj_t*        (*QStateMachineGetNextStateFptr)(QStateMachine_t *self);
typedef QExectStateRst_t    (*QStateMachineSetExectStateFptr)(QStateMachine_t *self, QExectState_t exect_state);
typedef QExectState_t       (*QStateMachineGetExectStateFptr)(QStateMachine_t *self);

enum QExectStateRst
{
    QExectStateRst_Success = 0,     //成功
    QExectStateRst_Fail,            //失败
};

enum QExectState
{
    QExectState_Idle,               //空闲状态
    QExectState_Init,               //初始
    QExectState_Run,                //运行
    QExectState_Over,               //结束
};

struct QStateObj
{
    void                            *parent_;           //父类
    const char                      *state_name;        //状态机名称
    QstateObjRunOnceFptr            RunOnce;            //状态机运行回调
    void                            *user_data;         //用户数据
};

struct QStateMachine
{
    void                                *parent_;               //父类
    QExectState_t                       exect_state;            //执行器的状态
    QStateObj_t                         *curr_state;            //当前状态
    QStateObj_t                         *next_state;            //上一个状态
    
    QStateMachineSetCurrStateFptr       SetCurrState;           //设置当前状态机
    QStateMachineGetCurrStateFptr       GetCurrState;           //获取当前状态机
    QStateMachineSetNextStateFptr       SetNextState;           //设置下一个状态机
    QStateMachineGetNextStateFptr       GetNextState;           //获取下一个状态机
    QStateMachineSetExectStateFptr      SetExectState;          //设置执行中的状态机
    QStateMachineGetExectStateFptr      GetExectState;          //获取执行中的状态机
};

/*** 
 * @description: 状态机执行器创建
 * @param {*}
 * @return {*}
 */
QStateMachine_t *QstateMachineCreate(void);

/*** 
 * @description: 状态机加载
 * @param {*}
 * @return {*}
 */
QExectStateRst_t QstateMachineLoadState(QStateMachine_t *self, const char *name, QstateObjRunOnceFptr method, void *user_data);

/*** 
 * @description: 状态机运行
 * @param {*}
 * @return {*}
 */
void QstateMachineRunOnce(QStateMachine_t *self);

/*** 
 * @description: 状态机销毁
 * @param {*}
 * @return {*}
 */
void QstateMachineDestroy(QStateMachine_t *self);

#ifdef __cplusplus
}
#endif

#endif
