#ifndef SD_CARD_BSP_H
#define SD_CARD_BSP_H

void SD_card_Init(void);
int s_example_write_file(const char *path, char *data);
int s_example_read_file(const char *path,char *pxbuf,uint32_t *outLen);

#endif