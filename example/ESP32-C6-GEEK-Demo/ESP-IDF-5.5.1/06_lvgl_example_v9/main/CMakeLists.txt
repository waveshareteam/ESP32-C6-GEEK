set(SOURCES
    "./main.c"
    "./LCD_Driver/lcd_st7789.c"
    "./LVGL_Driver/LVGL_Driver.c"
)

set(INCLUDE_DIRS 
    "./LCD_Driver"  
    "./LVGL_Driver" 
    "."
)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
)
