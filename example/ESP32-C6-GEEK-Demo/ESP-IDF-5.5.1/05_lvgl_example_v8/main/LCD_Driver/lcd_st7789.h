#ifndef LCD_ST7789_H
#define LCD_ST7789_H

#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

/* LCD size */
#define EXAMPLE_LCD_H_RES (135)
#define EXAMPLE_LCD_V_RES (240)

#define EXAMPLE_LCD_X_GAP (52)
#define EXAMPLE_LCD_Y_GAP (40)

/* LCD settings */
#define EXAMPLE_LCD_SPI_NUM (SPI2_HOST)
#define EXAMPLE_LCD_PIXEL_CLK_HZ (40 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS (8)
#define EXAMPLE_LCD_PARAM_BITS (8)
#define EXAMPLE_LCD_COLOR_SPACE (ESP_LCD_COLOR_SPACE_RGB)
#define EXAMPLE_LCD_BITS_PER_PIXEL (16)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (1)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (50)
#define EXAMPLE_LCD_BL_ON_LEVEL (1)

/* LCD pins */
#define EXAMPLE_LCD_GPIO_SCLK (GPIO_NUM_1)
#define EXAMPLE_LCD_GPIO_MOSI (GPIO_NUM_2)
#define EXAMPLE_LCD_GPIO_RST (GPIO_NUM_4)
#define EXAMPLE_LCD_GPIO_DC (GPIO_NUM_3)
#define EXAMPLE_LCD_GPIO_CS (GPIO_NUM_5)
#define EXAMPLE_LCD_GPIO_BL (GPIO_NUM_6)

extern esp_lcd_panel_handle_t lcd_panel;
extern esp_lcd_panel_io_handle_t lcd_io;

esp_err_t app_lcd_init(void);

#endif