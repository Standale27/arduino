Battery voltage checker and timer for Adafruit Huzzah32 v1. Pinout has SDA on 23 and SCL on 22. Voltage checker is meant for LiPo batteries, anything that operates between 3.7 and 4.2 volts will work.

Timer should be accurate since it utilizes millis() rather than delay().

Specifically made for an AdaFruit SSD1306 0.93" monochrome OLED display. Connects to display via I2C protocol.

Version for the SH1107 (PIM374 from Pimoroni, 1.12" 128x128 monochrome OLED) has been made, includes a battery bar that should deplete along with the battery percentage.

Battery percentage now goes from 0-100% and is correlated to a range of 3.5-4.2V. So, if the voltage reads 3.6V, the percentage will read 14.29%. Auto-shutoff was shown to occur around 3.55V after leaving the arduino on overnight on battery power.

https://i.imgur.com/As4Aw2h.png
