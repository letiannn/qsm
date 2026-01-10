# QSM (Quick State Machine)

QSM 是一个轻量级的有限状态机(FSM)组件，基于 RT-Thread 实时操作系统开发。它提供了简洁的状态管理机制，支持状态转换、生命周期管理和表驱动状态机模式。

## 特性

- **轻量级设计**：占用资源少，适合嵌入式系统
- **表驱动状态机**：通过状态表定义状态转换逻辑，代码清晰易维护
- **状态生命周期管理**：支持 Init/Run/Over 三种状态生命周期
- **内存池管理**：使用 RT-Thread 内存池提高分配效率
- **支持用户数据传递**：状态回调函数可接收用户自定义数据
- **灵活的状态切换**：支持成功和失败两种状态转换路径

## 目录结构

```
qsm/
├── qsm.h              # 状态机核心头文件
├── qsm.c              # 状态机核心实现
├── qsm_mgr.h          # 表驱动状态机管理器头文件
├── qsm_mgr.c          # 表驱动状态机管理器实现
├── SConscript         # SCons 构建脚本
└── example/           # 示例代码
    ├── qsm_demo.c     # 基础状态机示例
    └── qsm_netlink.c  # 网络链路状态机示例
```

## 核心概念

### 状态机 (QStateMachine)

状态机是 QSM 的核心对象，包含以下属性：

- **状态执行状态**：Idle、Init、Run、Over
- **当前状态**：正在执行的状态对象
- **下一个状态**：将要切换到的状态对象
- **内存池**：用于管理状态对象的内存分配

### 状态对象 (QStateObj)

状态对象代表一个具体的状态，包含：

- **状态索引**：唯一标识一个状态
- **运行回调**：状态执行时调用的函数
- **用户数据**：可传递给回调函数的自定义数据

### 状态表 (QsmMgrTbl)

状态表用于定义状态间的转换关系：

```c
struct QsmMgrTbl
{
    uint8_t state_idx;              // 状态索引
    QstateObjRunOnceFptr method;    // 状态机方法
    uint8_t succ_state_idx;         // 成功时跳转的索引
    uint8_t fail_state_idx;         // 失败时跳转的索引
};
```

## API 参考

### 基础状态机 API

```c
// 创建状态机
QStateMachine_t *QstateMachineCreate(char *name);

// 加载状态
QExectStateRst_t QstateMachineLoadState(QStateMachine_t *self, 
                                        uint8_t state_idx, 
                                        QstateObjRunOnceFptr method, 
                                        void *user_data);

// 运行一次状态机
void QstateMachineRunOnce(QStateMachine_t *self);

// 销毁状态机
void QstateMachineDestroy(QStateMachine_t *self);
```

### 状态机管理器 API

```c
// 创建状态机管理器
QsmMgr_t *QsmMgrCreate(char *qsm_name, 
                       QsmMgrTbl_t *qsm_table, 
                       uint8_t qsm_table_size, 
                       void *user_data);

// 启动状态机（加载第一个状态）
QExectStateRst_t QsmMgrStart(QsmMgr_t *qsm_manage);

// 加载成功状态
QExectStateRst_t QsmMgrLoadSucc(QsmMgr_t *qsm_manage);

// 加载失败状态
QExectStateRst_t QsmMgrLoadFail(QsmMgr_t *qsm_manage);

// 运行一次状态机
void QsmMgrRunOnce(QsmMgr_t *qsm_manage);

// 销毁状态机管理器
void QsmMgrDestroy(QsmMgr_t *qsm_manage);
```

## 使用示例

### 基础状态机示例

```c
#include "qsm.h"

// 定义状态运行函数
static void State1RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    if (qsm->GetExectState(qsm) == QExectState_Init)
    {
        // 状态初始化时执行
        LOG_I("State 1 init");
    }
    else if (qsm->GetExectState(qsm) == QExectState_Run)
    {
        // 状态运行时执行
        LOG_I("State 1 running");
        // 切换到下一个状态
        QstateMachineLoadState(qsm, 2, State2RunOnce, user_data);
    }
    else if (qsm->GetExectState(qsm) == QExectState_Over)
    {
        // 状态退出时执行
        LOG_I("State 1 destroy");
    }
}

// 在任务中使用
static void qsm_task(void *arg)
{
    QStateMachine_t *qsm = QstateMachineCreate("test_sm");
    QstateMachineLoadState(qsm, 1, State1RunOnce, NULL);
    
    while (1)
    {
        QstateMachineRunOnce(qsm);
        rt_thread_mdelay(10);
    }
    
    QstateMachineDestroy(qsm);
}
```

### 表驱动状态机示例

```c
#include "qsm_mgr.h"

// 定义状态表
static QsmMgrTbl_t state_table[] = {
    {0, StateIdleRunOnce,  1, 0},   // Idle -> Success: State1, Fail: Idle
    {1, State1RunOnce,     2, 0},   // State1 -> Success: State2, Fail: Idle
    {2, State2RunOnce,     3, 1},   // State2 -> Success: State3, Fail: State1
    {3, State3RunOnce,     3, 0},   // State3 -> Success: State3, Fail: Idle
};

// 使用管理器
static void qsm_mgr_task(void *arg)
{
    QsmMgr_t *qsm_mgr = QsmMgrCreate("test_mgr", state_table, 4, NULL);
    QsmMgrStart(qsm_mgr);
    
    while (1)
    {
        QsmMgrRunOnce(qsm_mgr);
        
        // 根据业务逻辑选择状态转换
        if (check_success())
        {
            QsmMgrLoadSucc(qsm_mgr);
        }
        else
        {
            QsmMgrLoadFail(qsm_mgr);
        }
        
        rt_thread_mdelay(10);
    }
    
    QsmMgrDestroy(qsm_mgr);
}
```

## 运行示例

在 RT-Thread FinSH 控制台中运行：

```
qsm_base_example
qsm_mgr_example
```

## 编译配置

在 `SConscript` 中配置组件编译：

```python
from building import *

cwd = GetCurrentDir()
src = Glob('*.c')
src += Glob('example/*.c')

if GetDepend(['PKG_USING_QSM']):
    src = []

group = DefineGroup('qsm', src, depend = [''])
Return('group')
```

## 依赖

- RT-Thread 实时操作系统
- ulog 组件（用于日志输出，可选）
