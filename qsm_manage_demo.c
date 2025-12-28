/*
 * @Date: 2025-09-26 09:08:16
 * @LastEditTime: 2025-10-09 09:20:18
 * @FilePath: /brd_project/main/qsm_manage_demo.c
 * @Description: 
 */
#include "qsm.h"
#include "qsm_manage.h"
#include "qsm_netlink.h"
#include "stdint.h"
#include "stdio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "demo"

uint32_t idx = 0;

void qsm_task(void *arg)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "qsm init");
    QsmManage_t *netlink_qsm = QsmManageCreate(g_qsm_network_table, g_qsm_network_num, &idx);

    while(idx < 30000)
    {
        idx+=1;

        if(idx == 10)
        {
            QsmManageStart(netlink_qsm);
        }
        if(idx % 20 < 10 && idx > 10)
        {
            QsmManageLoadSucc(netlink_qsm);
        }
        else if(idx % 20 > 10 && idx > 10)
        {
            QsmManageLoadFail(netlink_qsm);
        }

        QsmManageRunOnce(netlink_qsm);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        
    }
    QsmManageDestroy(netlink_qsm);

    while(1)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
