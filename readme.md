# qsm 迷你状态机

## 项目概述

**qsm** 是一个轻量级的状态机框架，提供了一个简单而灵活的状态机实现。该框架支持状态的动态加载、执行状态管理、以及状态间的切换，适用于需要状态管理的嵌入式系统和应用程序。

## 核心特性

### 1. 状态机生命周期管理
- **初始化**：状态创建时的初始化处理
- **运行**：状态的主运行逻辑
- **结束**：状态销毁前的清理工作

### 2. 灵活的状态切换
- 支持动态加载新状态
- 自动处理状态切换过程
- 提供状态执行状态管理

### 3. 用户数据支持
- 每个状态都可以携带用户数据
- 支持回调函数传递用户数据

## 系统架构

### 核心数据结构

| 结构体 | 作用 |
|--------|------|
| [QStateObj_t](./qsm.h#L56-L62) | 状态对象，包含状态名称、运行函数和用户数据 |
| [QStateMachine_t](./qsm.h#L64-L76) | 状态机，管理当前状态、下一状态和执行状态 |

### 执行状态

- [QExectState_Idle](./qsm.h#L45-L45): 空闲状态
- [QExectState_Init](./qsm.h#L46-L46): 初始化状态
- [QExectState_Run](./qsm.h#L47-L47): 运行状态
- [QExectState_Over](./qsm.h#L48-L48): 结束状态

### 执行结果

- [QExectStateRst_Success](./qsm.h#L35-L35): 操作成功
- [QExectStateRst_Fail](./qsm.h#L36-L36): 操作失败

## 主要功能函数

### 状态机管理

- [QstateMachineCreate()](./qsm.h#L82-L82): 创建状态机实例
- [QstateMachineDestroy()](./qsm.h#L98-L98): 销毁状态机实例
- [QstateMachineLoadState()](./qsm.h#L86-L86): 加载新状态
- [QstateMachineRunOnce()](./qsm.h#L92-L92): 执行一次状态机循环

### 状态对象管理

- [QStateObj_t](./qsm.h#L56-L62): 状态对象结构体，包含 [RunOnce](./qsm.c#L57-L57) 回调函数

## 高级功能：状态机管理器

### QSM Manage 模块

除了基础状态机，项目还提供了一个高级的状态机管理器（qsm_manage），用于管理复杂的状态转换流程。

#### 核心数据结构

- [QsmManageTable_t](./qsm_manage.h#L22-L28): 状态管理表项，定义状态间的跳转关系
- [QsmManage_t](./qsm_manage.h#L30-L36): 状态管理器，管理状态机表和当前状态

#### 状态跳转机制

- [succ_loadname](./qsm_manage.h#L26-L26): 成功时跳转到的状态名称
- [fail_loadname](./qsm_manage.h#L27-L27): 失败时跳转到的状态名称

#### 管理器功能函数

- [QsmManageCreate()](./qsm_manage.h#L38-L38): 创建状态管理器
- [QsmManageStart()](./qsm_manage.h#L40-L40): 启动状态管理器
- [QsmManageLoadSucc()](./qsm_manage.h#L42-L42): 加载成功状态
- [QsmManageLoadFail()](./qsm_manage.h#L44-L44): 加载失败状态
- [QsmManageRunOnce()](./qsm_manage.h#L46-L46): 执行一次状态管理器循环
- [QsmManageDestroy()](./qsm_manage.h#L48-L48): 销毁状态管理器

## 使用示例

### 基础状态机使用

```c
// 定义状态执行函数
void MyStateRunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        // 初始化逻辑
        printf("State %s initialized\n", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Run)
    {
        // 运行逻辑
        printf("State %s running\n", self->state_name);
    }
    else if(qsm->GetExectState(qsm) == QExectState_Over)
    {
        // 结束逻辑
        printf("State %s destroyed\n", self->state_name);
    }
}

// 使用状态机
int main()
{
    // 创建状态机
    QStateMachine_t *qsm = QstateMachineCreate();
    
    // 加载状态
    QstateMachineLoadState(qsm, "MyState", MyStateRunOnce, &some_data);
    
    // 运行状态机循环
    for(int i = 0; i < 100; i++)
    {
        QstateMachineRunOnce(qsm);
        usleep(10000); // 延时10ms
    }
    
    // 销毁状态机
    QstateMachineDestroy(qsm);
    return 0;
}
```

### 状态管理器使用

```c
// 定义状态管理表
QsmManageTable_t state_table[] = {
    {0, "State1", State1RunOnce, "State2", "State3"},  // 状态1：成功跳转到状态2，失败跳转到状态3
    {1, "State2", State2RunOnce, "State3", "State1"},  // 状态2：成功跳转到状态3，失败跳转到状态1
    {2, "State3", State3RunOnce, "State1", "State2"},  // 状态3：成功跳转到状态1，失败跳转到状态2
};
uint8_t table_size = sizeof(state_table) / sizeof(state_table[0]);

// 使用状态管理器
int main()
{
    QsmManage_t *manager = QsmManageCreate(state_table, table_size, &user_data);
    
    // 启动状态管理器
    QsmManageStart(manager);
    
    // 状态机主循环
    for(int i = 0; i < 1000; i++)
    {
        QsmManageRunOnce(manager);
        
        // 根据条件决定跳转到成功或失败状态
        if(some_condition)
        {
            QsmManageLoadSucc(manager);
        }
        else
        {
            QsmManageLoadFail(manager);
        }
        
        usleep(10000);
    }
    
    QsmManageDestroy(manager);
    return 0;
}
```

## 实际应用示例：网络连接状态机

项目中提供了一个网络连接状态机的实际应用示例：

- [QSM_NETLINK_IDLE](./qsm_netlink.c#L9-L9): 空闲状态
- [QSM_NETLINK_INIT](./qsm_netlink.c#L10-L10): 网络初始化状态
- [QSM_NETLINK_CONN](./qsm_netlink.c#L11-L11): 网络连接状态
- [QSM_NETLINK_WORK](./qsm_netlink.c#L12-L12): 网络工作状态
- [QSM_NETLINK_RELINK](./qsm_netlink.c#L13-L13): 网络重连状态

## 安全特性

### 断言机制

- 提供 [QSM_ASSERT](./qsm.h#L24-L27) 宏用于参数检查
- 可通过定义 [QSM_NDEBUG](./qsm.h#L16-L16) 禁用断言以提高性能

### 内存管理

- 自动内存分配和释放
- 状态切换时自动销毁旧状态对象
- 防止内存泄漏

## 设计模式

该状态机实现了以下设计模式：

1. **状态模式**：每个状态封装了其行为
2. **策略模式**：不同的状态对象可以有不同的执行策略
3. **命令模式**：状态执行函数作为可执行的命令

## 适用场景

- 网络连接状态管理
- 设备工作状态管理
- UI状态管理
- 流程控制
- 有限状态机应用

## 注意事项

1. 确保在使用前正确初始化状态机
2. 状态执行函数应根据执行状态（[QExectState](./qsm.h#L39-L49)）执行相应的逻辑
3. 合理使用用户数据参数传递状态间需要共享的数据
4. 在状态切换时注意资源的释放和重新分配