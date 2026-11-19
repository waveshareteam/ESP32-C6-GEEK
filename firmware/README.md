# ESP32-C6-GEEK Firmware

[简体中文](README_ZH.md)

Use this firmware to restore the factory demo on currently shipping ESP32-C6-GEEK boards.

| File | Purpose |
| --- | --- |
| [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | Restores the factory demo. |

Flash the BIN at `0x0`. Erasing the entire flash clears firmware, settings, and user data, so back up any data you need first. After flashing, reset the board and check that the factory demo starts.

Use esptool 5 or later and replace `PORT` with the board’s serial port.

```sh
python -m esptool --chip esp32c6 --port PORT erase-flash
python -m esptool --chip esp32c6 --port PORT write-flash 0x0 ESP32-C6-GEEK_FactoryOnly_20260323.bin
```

For flashing instructions, see the [product documentation](https://docs.waveshare.com/ESP32-C6-GEEK).
