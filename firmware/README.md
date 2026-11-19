# ESP32-C6-GEEK Firmware

[简体中文](README_ZH.md)

The factory image was tested on both V1 and V2. Check the back of the board for a V2 label.

| Board | File | Purpose |
| --- | --- | --- |
| V1 | [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | V1 factory demo for testing the LCD and microSD card. |
| V2 | [ESP32-C6-GEEK_FactoryOnly_20260323.bin](ESP32-C6-GEEK_FactoryOnly_20260323.bin) | V2 factory demo for testing the LCD and microSD card. |

Open a terminal in this directory, use esptool 5 or later, and replace `PORT` with the board’s serial port. Back up any data you need before erasing the entire flash. Use the following command for either board revision.

```sh
python -m esptool --chip esp32c6 --port PORT erase-flash
python -m esptool --chip esp32c6 --port PORT write-flash 0x0 ESP32-C6-GEEK_FactoryOnly_20260323.bin
```

After flashing, reset the board and confirm that the factory demo starts.

For flashing instructions, see the [product documentation](https://docs.waveshare.com/ESP32-C6-GEEK).
