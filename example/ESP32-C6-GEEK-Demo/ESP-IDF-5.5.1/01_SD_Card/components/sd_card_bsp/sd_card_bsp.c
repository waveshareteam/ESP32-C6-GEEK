#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "sd_card_bsp.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define PIN_NUM_MISO  (gpio_num_t)20
#define PIN_NUM_MOSI  (gpio_num_t)18
#define PIN_NUM_CLK   (gpio_num_t)19
#define SDlist "/sdcard" //Directory, similar to a standard
#define PIN_NUM_CS    (gpio_num_t)23
#define SD_SPI SPI2_HOST


sdmmc_card_t *card = NULL; //handle


void SD_card_Init(void)
{
  esp_vfs_fat_sdmmc_mount_config_t mount_config = 
  {
    .format_if_mount_failed = false,    //If the hook fails, create a partition table and format the SD card
    .max_files = 5,                    //Maximum number of open files
    .allocation_unit_size = 512  //Similar to sector size
  };
  spi_bus_config_t bus_cfg = 
  {
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = PIN_NUM_MISO,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4000,   //Maximum transfer size   
  };
  ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize(SD_SPI, &bus_cfg, SDSPI_DEFAULT_DMA));
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = PIN_NUM_CS;
  slot_config.host_id = SD_SPI;
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SD_SPI;
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdspi_mount(SDlist, &host, &slot_config, &mount_config, &card)); 
  if(card != NULL)
  {
    sdmmc_card_print_info(stdout, card); //Print out the card information
    printf("practical_size:%.2fG\n",(float)(card->csd.capacity)/2048/1024);//g
  }
}
float sd_cadr_get_value(void)
{
  if(card != NULL)
  {
    return (float)(card->csd.capacity)/2048/1024; //G
  }
  else
  return 0;
}

/*write data
path:path
data:data
*/
int s_example_write_file(const char *path, char *data)
{
  esp_err_t err;
  if(card == NULL)
  {
    return ESP_ERR_NOT_FOUND;
  }
  err = sdmmc_get_status(card); //First check if there is an SD card
  if(err != ESP_OK)
  {
    return err;
  }
  FILE *f = fopen(path, "w"); //Get path address
  if(f == NULL)
  {
    printf("path:Write Wrong path\n");
    return ESP_ERR_NOT_FOUND;
  }
  fprintf(f, data); //write in
  fclose(f);
  return ESP_OK;
}
/*
read data
path:path
*/
int s_example_read_file(const char *path,char *pxbuf,uint32_t *outLen)
{
  esp_err_t err;
  if(card == NULL)
  {
    printf("path:card == NULL\n");
    return ESP_ERR_NOT_FOUND;
  }
  err = sdmmc_get_status(card); //First check if there is an SD card
  if(err != ESP_OK)
  {
    printf("path:card == NO\n");
    return err;
  }
  FILE *f = fopen(path, "rb");
  if (f == NULL)
  {
    printf("path:Read Wrong path\n");
    return ESP_ERR_NOT_FOUND;
  }
  fseek(f, 0, SEEK_END);     //Move the pointer to the back
  uint32_t unlen = ftell(f);
  //fgets(pxbuf, unlen, f); //Read text
  fseek(f, 0, SEEK_SET); //Move the pointer to the front
  uint32_t poutLen = fread((void *)pxbuf,1,unlen,f);
  printf("pxlen: %ld,outLen: %ld\n",unlen,poutLen);
  //*outLen = poutLen;
  fclose(f);
  return ESP_OK;
}
