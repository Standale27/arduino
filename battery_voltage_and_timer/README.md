Battery voltage checker and timer for Adafruit Huzzah32 v1. Pinout has SDA on 23 and SCL on 22. Voltage checker is meant for LiPo batteries, anything that operates between 3.7 and 4.2 volts will work.

Timer should be accurate since it utilizes millis() rather than delay().

Specifically made for an AdaFruit SSD1306 0.93" monochrome OLED display. Connects to display via I2C protocol.

Version for the SH1107 (PIM374 from Pimoroni, 1.12" 128x128 monochrome OLED) has been made, includes a battery bar that should deplete along with the battery percentage.

Battery percentage now goes from 0-100% and is correlated to a range of 3.5-4.2V. So, if the voltage reads 3.6V, the percentage will read 14.29%. Auto-shutoff was shown to occur around 3.55V after leaving the arduino on overnight on battery power.

https://i.imgur.com/As4Aw2h.png

Update 11/12/23

Back in April, I added an Adafruit FeatherWing OLED display (SH1107, 1.3", 128x64 monochrome OLED) to the Adafruit Huzzah32 ESP32 Feather. It comes with three buttons as well as an additional reset button.

I mounted the Feather + FeatherWing to a mini-breadboard along with a 420 mAh LiPo battery.

Later on, I soldered in an on/off switch to easily open and close the connection from the battery to the ESP32.

I 3D printed a housing for the assembly in Fusion 360, and added a lid/cover to slide over the top. There are holes and gaps to allow access to the buttons on the FeatherWing and the connection points on the breadboard respectively.

>Top view in housing:                   [https://i.imgur.com/p6WUQPt.png](https://i.imgur.com/p6WUQPt.png)
>
>Top view in housing with open lid:     [https://i.imgur.com/iCJkVXJ.png](https://i.imgur.com/iCJkVXJ.png)
>
>Side view in housing:                  [https://i.imgur.com/uTqC4Ky.png](https://i.imgur.com/uTqC4Ky.png)
>
>Side view without housing:             [https://i.imgur.com/hhfqqBR.png](https://i.imgur.com/hhfqqBR.png)

Lastly, I updated the voltage checker and timer code:

This involved things like:
>Simple design changes (border around the screen)
>
>Runtime display shows the runtime in seconds at first, then min:sec, then hr:min:sec
>
>If battery voltage is over 4.2V, the battery percentage is shown as 100%. If voltage is at/below 3.5V, the percentage is shown as 0%
>
>The battery indicator only updates once every 10 seconds rather than every iteration of the loop function, to prevent the visually jarring amount of updates
  >Because of this, the indicator was showing as empty for the first 10 seconds of runtime. So, I added an animation to the indicator to pass the time
>
>The buttons do work, but only display the name of the button (A, B, C) when each one is depressed.

One more change I'd like to add is the ability to toggle on and off the information shown using the buttons, and maybe eventually display other information if components are attached:
>Button A could show/hide the battery voltage, battery indicator, and battery percentage (all 3 at once or sequentially)
>
>Button B could show/hide the runtime (or change the format for how the runtime is shown)
>
>Button C could show/hide the border (or allow for attached component data [like light sensor data, temperature/humidity data, soil humidity data, etc.] to be shown/hidden)

I would also like to update the 3D printed housing, though this depends on me fixing the extrusion issues with my 3D printer:
>Components could be printed in different colors of PLA
>
>The button mechanism I was aiming to have is intrinsically flawed; the design of the lid must be reworked
>
>The design could better encase the entire assembly while simultaneously allowing for easier access to the connection points on the breadboard

I'm not sure what else I could add in terms of functionality and design, as the main goal of this project is to have a self-contained, portable Arduino machine that can display data and be used to prototype other projects.
