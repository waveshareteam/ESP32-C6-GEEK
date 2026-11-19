# ESP32-C6-GEEK 固件

[English](README.md)

该出厂镜像已在 V1 和 V2 上实测通过。可查看板背面是否贴有 V2 标签。

| 硬件版本 | 文件 | 用途 |
| --- | --- | --- |
| V1 | [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | V1 出厂演示，测试 LCD 和 Micro SD 卡。 |
| V2 | [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | V2 出厂演示，测试 LCD 和 Micro SD 卡。 |

在本目录打开终端，使用 esptool 5 或更新版本，将 `PORT` 替换为开发板串口。整片擦除会清除原有固件、设置和用户数据，请先备份。两种板型使用相同的烧录命令。

```sh
python -m esptool --chip esp32c6 --port PORT erase-flash
python -m esptool --chip esp32c6 --port PORT write-flash 0x0 ESP32-C6-GEEK_FactoryOnly_20260323.bin
```

烧录完成后复位开发板，确认出厂演示正常启动。

烧录步骤请参阅[产品文档](https://docs.waveshare.net/ESP32-C6-GEEK/)。
