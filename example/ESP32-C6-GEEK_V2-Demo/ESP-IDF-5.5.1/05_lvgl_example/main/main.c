/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "ST7789.h"
#include "SD_SPI.h"
#include "Wireless.h"
#include "LVGL_Example.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static StaticSemaphore_t lvgl_mutex_buf;
static SemaphoreHandle_t lvgl_mutex = NULL;

static const char *TAG = "main";

/**
 * 初始化 LVGL 互斥锁
 * 应在 lv_init() 之前调用
 */
void lvgl_port_lock_init(void) {
    if(lvgl_mutex == NULL) {
        // 修复：传递静态缓冲区给函数
        lvgl_mutex = xSemaphoreCreateRecursiveMutexStatic(&lvgl_mutex_buf);
        if(lvgl_mutex == NULL) {
        }
    }
}

/**
 * 锁定 LVGL 操作
 * @param timeout_ms 超时时间（毫秒），0 表示无限等待
 * @return 成功返回 true，失败返回 false
 */
bool lvgl_port_lock(uint32_t timeout_ms) {
    if(lvgl_mutex == NULL) return false;

    TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    
    // 递归锁定
    if(xSemaphoreTakeRecursive(lvgl_mutex, timeout_ticks) == pdTRUE) {
        return true;
    } else {
        return false;
    }
}

/**
 * 解锁 LVGL 操作
 */
void lvgl_port_unlock(void) {
    if(lvgl_mutex != NULL) {
        // 递归解锁
        xSemaphoreGiveRecursive(lvgl_mutex);
    }
}

esp_err_t tf_card_rw_test(void)
{
    const char *file_path = "/sdcard/hello.txt";
    const char *test_str  = "Hello TF card!";

    // 写文件
    ESP_LOGI(TAG, "Opening file %s for write", file_path);
    FILE *f = fopen(file_path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");

        return ESP_FAIL;
    }
    fprintf(f, "%s\n", test_str);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // 读文件
    ESP_LOGI(TAG, "Opening file %s for read", file_path);
    f = fopen(file_path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");

        return ESP_FAIL;
    }

    char line[64] = {0};
    fgets(line, sizeof(line), f);
    fclose(f);

    ESP_LOGI(TAG, "Read from file: '%s'", line);

    return ESP_OK;
}

void example_sdcard_task(void *arg)
{
  uint32_t value = 1;
  char test[45] = {""};
  char rtest[45] = {""};
  for(;;)
  {
    lvgl_port_lock(0);
    // snprintf(test,45,"sdcard_writeTest : %ld\n",value);
    // s_example_write_file("/sdcard/writeTest.txt",test);

    // s_example_read_file("/sdcard/writeTest.txt");

    // value++;
    tf_card_rw_test();
    lvgl_port_unlock();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void app_main(void)
{

    Wireless_Init();
    Flash_Searching();
    lvgl_port_lock_init();
    SD_Init();                              // SD must be initialized behind the LCD
    LCD_Init();
    LVGL_Init();                            // returns the screen object

    xTaskCreate(example_sdcard_task, "example_sdcard_task", 3000, NULL, 2, NULL);
/********************* Demo *********************/
    lvgl_port_lock(0);
    Lvgl_Example1();

    // lv_demo_widgets();
    // lv_demo_keypad_encoder();
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_music();
    lvgl_port_unlock();

    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lvgl_port_lock(0);
        lv_timer_handler();
        lvgl_port_unlock();
    }
}


