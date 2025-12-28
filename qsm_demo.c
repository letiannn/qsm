/*
 * @Date: 2025-09-26 09:08:16
 * @LastEditTime: 2025-09-26 09:09:23
 * @FilePath: /brd_project/main/qsm_demo.c
 * @Description: 
 */
#include "qsm.h"
#include "stdint.h"
#include "stdio.h"
#include "esp_log.h"
#include "freertos/task.h"

#define TAG "demo"

void Test1RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *idx = (uint32_t *)user_data;
    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *idx);
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

void Test2RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *idx = (uint32_t *)user_data;
    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *idx);
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

void Test3RunOnce(QStateObj_t *self, QStateMachine_t *qsm, void *user_data)
{
    QSM_ASSERT(self != NULL);
    QSM_ASSERT(qsm != NULL);

    uint32_t *idx = (uint32_t *)user_data;
    if(qsm->GetExectState(qsm) == QExectState_Init)
    {
        ESP_LOGI(TAG, "%s create %ld", self->state_name, *idx);
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

uint32_t idx = 0;

void qsm_task(void *arg)
{
    QStateMachine_t *qsm = QstateMachineCreate();
    
    while(idx < 30000)
    {
        QstateMachineRunOnce(qsm);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        idx+=1;
        if(idx % 10 == 0)
        {
            QstateMachineLoadState(qsm, "test_sm2", Test2RunOnce, &idx);
        }
        else if(idx % 10 == 3)
        {
            QstateMachineLoadState(qsm, "test_sm3", Test3RunOnce, &idx);
        }
        else if(idx % 10 == 6)
        {
            QstateMachineLoadState(qsm, "test_sm1", Test1RunOnce, &idx);
        }
    }
    QstateMachineDestroy(qsm);

    while(1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
