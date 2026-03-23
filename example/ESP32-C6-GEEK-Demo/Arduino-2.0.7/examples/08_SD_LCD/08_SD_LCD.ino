#include <SPI.h>
#include <SD.h>
#include <JPEGDecoder.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include "OneButton.h"

#define PIN_INPUT 9
#define SD_CS   23   // 片选引脚
#define SD_MOSI 18  // MOSI引脚
#define SD_MISO 20  // MISO引脚  
#define SD_SCK  19  // SCK引脚

OneButton button(PIN_INPUT, true);

char click = 0;
int totalImages = 0;
String imageFiles[50]; // 存储图片文件名

SPIClass sdSPI(FSPI);

void setup() {
  Serial.begin(115200);

  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  // 初始化SD卡
  if (!SD.begin(SD_CS,sdSPI)) {
    Serial.println("SD卡初始化失败!");
    //return;
  }
  else{
      Serial.println("SD卡初始化成功");
  }

  // 扫描SD卡中的图片文件
  scanImageFiles();
  
  Config_Init();
  LCD_Init();
  LCD_Clear(BLACK);
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  
  // 显示第一张图片（可以是内置的或SD卡的）
  if (totalImages > 0) {
    showSDImage(0); // 显示SD卡第一张图片
  } else {
    Paint_DrawImage(gImage_pic1, 0, 0, 135, 240); // 显示内置图片
  }

  button.attachLongPressStart(LongPressStart, &button);
  button.attachClick(Click, &button);
  button.setLongPressIntervalMs(1000);
}

void loop() {
  button.tick();
  delay(10);
}

void LongPressStart(void *oneButton) {
  analogWrite(DEV_BL_PIN, 0);
}

void Click(void *oneButton) {
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  click++;
  
  if (totalImages > 0) {
    // 使用SD卡图片
    if (click >= totalImages) click = 0;
    showSDImage(click);
  } else {
    // 使用内置图片
    if (click >= 4) click = 1;
    switch(click) {
      case 1:  
        Paint_DrawImage(gImage_pic1, 0, 0, 135, 240);
        break;
      case 2:  
        Paint_DrawImage(gImage_pic2, 0, 0, 135, 240);
        break;
      case 3: 
        Paint_DrawImage(gImage_pic3, 0, 0, 135, 240);
        break;
    }
  }
}

// 扫描SD卡中的图片文件
void scanImageFiles() {
  File root = SD.open("/");
  totalImages = 0;
  
  while (File file = root.openNextFile()) {
    String fileName = file.name();
    if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg") || fileName.endsWith(".JPG")) {
      imageFiles[totalImages] = fileName;
      totalImages++;
      Serial.println("找到图片: " + fileName);
      if (totalImages >= 50) break; // 最多存储50个文件名
    }
    file.close();
  }
  root.close();
  Serial.println("总共找到 " + String(totalImages) + " 张图片");
}

// 显示SD卡中的JPEG图片
void showSDImage(int index) {
  if (index < 0 || index >= totalImages) return;
  
  String filePath = "/" + imageFiles[index];
  Serial.println("显示图片: " + filePath);
  
  File jpegFile = SD.open(filePath, FILE_READ);
  if (!jpegFile) {
    Serial.println("无法打开文件: " + filePath);
    return;
  }
  
  // 解码并显示JPEG
  boolean decoded = JpegDec.decodeSdFile(filePath.c_str());
  
  if (decoded) {
    // 图片信息
    Serial.print("图片尺寸: ");
    Serial.print(JpegDec.width);
    Serial.print(" x ");
    Serial.print(JpegDec.height);
    Serial.print(", MCU尺寸: ");
    Serial.print(JpegDec.MCUWidth);
    Serial.print(" x ");
    Serial.println(JpegDec.MCUHeight);
    
    // 调整图片尺寸以适应屏幕
    renderJPEG();
  } else {
    Serial.println("JPEG解码失败");
  }
  
  jpegFile.close();
}

void renderJPEG() {
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t jpeg_width = JpegDec.width;
  uint32_t jpeg_height = JpegDec.height;
  
  Serial.print("270度旋转: ");
  Serial.print(jpeg_width);
  Serial.print(" x ");
  Serial.println(jpeg_height);
  
  uint16_t rotated_width = jpeg_height;   // 135
  uint16_t rotated_height = jpeg_width;   // 240
  
  uint16_t x_pos = (LCD_WIDTH - rotated_width) / 2;
  uint16_t y_pos = (LCD_HEIGHT - rotated_height) / 2;
  
  Paint_Clear(WHITE);
  
  while (JpegDec.read()) {
    pImg = JpegDec.pImage;
    
    uint16_t mcu_x = JpegDec.MCUx * mcu_w;
    uint16_t mcu_y = JpegDec.MCUy * mcu_h;
    
    for (int y = 0; y < mcu_h; y++) {
      for (int x = 0; x < mcu_w; x++) {
        uint16_t orig_x = mcu_x + x;
        uint16_t orig_y = mcu_y + y;
        
        if (orig_x >= jpeg_width || orig_y >= jpeg_height) continue;
        
        // 旋转公式：
        uint16_t screen_x = x_pos + (jpeg_height - orig_y - 1);
        uint16_t screen_y = y_pos + orig_x;
        
        if (screen_x < LCD_WIDTH && screen_y < LCD_HEIGHT) {
          uint16_t color = pImg[x + y * mcu_w];
          Paint_SetPixel(screen_x, screen_y, color);
        }
      }
    }
  }
  
  JpegDec.abort();
}