#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_pm.h"
#include "iot_button.h"
#include "esp_sleep.h"
#include "esp_idf_version.h"
#include "button_gpio.h"
#include "esp_log.h"
#include "esp_check.h"
#include "button_driver.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "lvgl_Driver.h"

button_handle_t boot_btn;

extern lv_obj_t *g_text_label;

static const char *TAG = "button_example";

static const char *button_event_str[] = {
    "BUTTON_PRESS_DOWN",
    "BUTTON_PRESS_UP",
    "BUTTON_PRESS_REPEAT",
    "BUTTON_PRESS_REPEAT_DONE",
    "BUTTON_SINGLE_CLICK",
    "BUTTON_DOUBLE_CLICK",
    "BUTTON_MULTIPLE_CLICK",
    "BUTTON_LONG_PRESS_START",
    "BUTTON_LONG_PRESS_HOLD",
    "BUTTON_LONG_PRESS_UP",
    "BUTTON_PRESS_END",
    "BUTTON_EVENT_MAX",
    "BUTTON_NONE_PRESS",
};


static void _update_screen_text(const char *text, lv_color_t color)
{
    if (g_text_label == NULL) {
        ESP_LOGE(TAG, "LVGL text label is NULL, display failed!");
        return;
    }

    lvgl_port_lock(0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
    lv_label_set_text(g_text_label, text);
    lv_obj_set_style_text_color(g_text_label, color, 0);
    lv_obj_set_style_text_font(g_text_label, &lv_font_montserrat_20, 0);
    lv_obj_align(g_text_label, LV_ALIGN_CENTER, 0, 0);
    lvgl_port_unlock();
}

static void button_event_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "boot Button event: %s", button_event_str[iot_button_get_event((button_handle_t)arg)]); 
    
    button_event_t event = iot_button_get_event((button_handle_t)arg);
    switch (event) {
        case BUTTON_SINGLE_CLICK:
            _update_screen_text("Click", lv_color_hex(0xFFFF00));
            break;

        case BUTTON_LONG_PRESS_START:
            _update_screen_text("Long Press", lv_color_hex(0xFF0000));
            break;

        case BUTTON_DOUBLE_CLICK:
            _update_screen_text("Double Click", lv_color_hex(0x00FF00));
            break;

        default:
            ESP_LOGW(TAG, "Unsupported button event: %d", event);
            break;
    }
}
void button_init(void)
{
    button_config_t btn_cfg = {0};
    button_gpio_config_t gpio_cfg = {
        .gpio_num = BOOT_BUTTON_NUM,
        .active_level = 0,
        .enable_power_save = true,
    };

    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &boot_btn);
    assert(ret == ESP_OK);

    // ret = iot_button_register_cb(boot_btn, BUTTON_PRESS_DOWN, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_PRESS_UP, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_PRESS_REPEAT, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_PRESS_REPEAT_DONE, NULL, button_event_cb, NULL);
    ret |= iot_button_register_cb(boot_btn, BUTTON_SINGLE_CLICK, NULL, button_event_cb, NULL);
    ret |= iot_button_register_cb(boot_btn, BUTTON_DOUBLE_CLICK, NULL, button_event_cb, NULL);
    ret |= iot_button_register_cb(boot_btn, BUTTON_LONG_PRESS_START, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_LONG_PRESS_HOLD, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_LONG_PRESS_UP, NULL, button_event_cb, NULL);
    // ret |= iot_button_register_cb(boot_btn, BUTTON_PRESS_END, NULL, button_event_cb, NULL);
}
