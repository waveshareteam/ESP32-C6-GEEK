#include <stdio.h>
#include "user_app.h"

#include "sd_card_bsp.h"

#define sdcard_write_Test

void user_app_init(void);
void example_sdcard_task(void *arg);


void user_top_init(void)
{
  SD_card_Init();
  user_app_init();      //example
}
void user_app_init(void)
{
  #ifdef sdcard_write_Test
  xTaskCreate(example_sdcard_task, "example_sdcard_task", 3000, NULL, 2, NULL);
  #endif
}

void example_sdcard_task(void *arg)
{
  uint32_t value = 1;
  char test[45] = {""};
  char rtest[45] = {""};
  for(;;)
  {
    snprintf(test,45,"sdcard_writeTest : %ld\n",value);
    s_example_write_file("/sdcard/writeTest.txt",test);
    vTaskDelay(pdMS_TO_TICKS(500));
    s_example_read_file("/sdcard/writeTest.txt",rtest,NULL);
    printf("rtest:%s\n",rtest);
    vTaskDelay(pdMS_TO_TICKS(500));
    value++;
  }
}



