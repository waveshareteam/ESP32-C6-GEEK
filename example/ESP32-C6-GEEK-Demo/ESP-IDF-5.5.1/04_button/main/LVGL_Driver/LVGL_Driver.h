#ifndef LVGL_ADAPTER_H
#define LVGL_ADAPTER_H

#include "esp_err.h"
#include "lvgl.h"
#include "demos/lv_demos.h"

extern lv_display_t * lvgl_disp;

/**
 * @brief init LVGL
 * @return ESP_OK
 */
esp_err_t app_lvgl_init(void);


#endif // LVGL_ADAPTER_H