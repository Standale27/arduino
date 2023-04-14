Clock for Arduino Uno, but should be usable for the Micro and maybe the Nano as well. Biggest issue is how unoptimized the code is: Currently uses >25000 bytes of program storage space.

Specifically made for an AdaFruit SSD1306 0.93" monochrome OLED display. Connects to display via I2C protocol.

Uses digital pins 2 through 5 for buttons to adjust the clock time.
