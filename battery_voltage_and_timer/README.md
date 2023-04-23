Battery voltage checker and timer for Adafruit Huzzah32 v1. Pinout has SDA on 23 and SCL on 22. Voltage checker is meant for LiPo batteries, anything that operates between 3.7 and 4.2 volts will work.

Timer should be accurate since it utilizes millis() rather than delay().

Specifically made for an AdaFruit SSD1306 0.93" monochrome OLED display. Connects to display via I2C protocol.

Future work would be to add a version meant to use an SH1107 128x128 monochrome OLED display, and to add graphics (like a bar which depletes as the battery percentage drops)
