#include <Wire.h>
#include <SSD1306Wire.h>

#define OLED_ADDR 0x3d
#define OLED_SDA 23
#define OLED_SCL 22

const float MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2;

SSD1306Wire display(OLED_ADDR, SDA, SCL);
float counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
}

void loop() {
  // put your main code here, to run repeatedly:
  long begin = millis();
  display.clear();

  showOLEDMsg("Runtime (sec): ", String(counter,1), "");

  float rawValue = analogRead(A13);
  float voltageLevel = (rawValue/MAX_ANALOG_VAL)*2*1.1*3.3;
  float batteryFrac = voltageLevel / MAX_BATTERY_VOLTAGE;
  showBattery("Raw/1000: ", String((rawValue/1000),2), "Voltage: ", String(voltageLevel,2), "Battery %: ", String((batteryFrac * 100),1));

  display.display();

  Serial.println(rawValue);
  Serial.println(voltageLevel,5);
  Serial.println((batteryFrac/100),5);

  long end = millis();
  float tempdelta = (end-begin);
  float delta = tempdelta/1000;

  counter = (float(millis())/1000) - delta;
}

void showOLEDMsg(String line1, String line2, String line3) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 43, line1);
  display.drawString(0, 53, line2);
  display.drawString(0, 63, line3);
}

void showBattery(String l1, String l2, String l3, String l4, String l5, String l6) {
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, l1);
  display.drawString(60, 0, l2);
  display.drawString(0, 10, l3);
  display.drawString(60, 10, l4);
  display.drawString(0, 20, l5);
  display.drawString(60, 20, l6);
}
