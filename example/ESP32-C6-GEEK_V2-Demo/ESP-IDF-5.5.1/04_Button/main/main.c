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
#include "button_driver.h"
#include "lvgl.h"
#include "button_driver.h"

extern lv_display_t *lvgl_disp;

static const char *TAG = "APP_MAIN";
lv_obj_t *g_text_label = NULL;


void app_main(void)
{
    button_init();

    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* Show LVGL objects */
    /* Task lock */
    lvgl_port_lock(0);
    lv_display_set_rotation(lvgl_disp, LV_DISPLAY_ROTATION_90);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
    g_text_label = lv_label_create(lv_scr_act());
    lv_label_set_text(g_text_label, "Button Start");
    lv_obj_set_style_text_font(g_text_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(g_text_label, lv_color_hex(0x00FF00), 0);
    lv_obj_align(g_text_label, LV_ALIGN_CENTER, 0, 0);
    lvgl_port_unlock();

    /* Task unlock */
    lvgl_port_unlock();
}
