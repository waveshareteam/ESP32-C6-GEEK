/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "string.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lcd_st7789.h"
#include "lvgl_Driver.h"
#include "esp_lvgl_port.h"

void app_main(void)
{
    char str[50];
    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* Show LVGL objects */
    /* Task lock */
    lvgl_port_lock(0);
    /* Option 1: Create a simple ui
     * ---------------------
     */
    // app_main_display();
    /* Option 2: Or try out a demo.
     * -------------------------------------------------------------------------------------------
     */
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_snprintf(str, sizeof(str), "Hello LVGL \nVersion: V %d.%d.%d ", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);
    lv_label_set_text(label, str);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* Task unlock */
    lvgl_port_unlock();
}
