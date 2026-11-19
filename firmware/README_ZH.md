# ESP32-C6-GEEK 固件

[English](README.md)

此固件用于恢复当前出货的 ESP32-C6-GEEK 的出厂演示。

| 文件 | 用途 |
| --- | --- |
| [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | 恢复出厂演示。 |

将 BIN 从 `0x0` 烧录。整片擦除 Flash 会清除设备中的固件、设置和用户数据，请先备份。烧录完成后复位开发板，检查出厂演示是否正常启动。

使用 esptool 5 或更新版本，将 `PORT` 替换为开发板的串口。

```sh
python -m esptool --chip esp32c6 --port PORT erase-flash
python -m esptool --chip esp32c6 --port PORT write-flash 0x0 ESP32-C6-GEEK_FactoryOnly_20260323.bin
```

烧录步骤请参阅[产品文档](https://docs.waveshare.net/ESP32-C6-GEEK/)。
