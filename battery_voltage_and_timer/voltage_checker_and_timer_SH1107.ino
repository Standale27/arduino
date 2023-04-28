#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define OLED_RESET -1

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

const float MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2;

float counter = 0;
float rawValue;
float voltageLevel;
float batteryFrac;

void setup() {
  Serial.begin(115200);
  delay(250);
  display.begin(0x3C, true);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextColor(1);
}

void loop() {
  long begin = millis();
  display.clearDisplay();

  showOLEDMsg(counter);
  showBattery(voltageLevel, (batteryFrac * 100));
  batteryIndicator(batteryFrac*100);
  display.display();
  long end = millis();
  float tempdelta = (end-begin);
  float delta = tempdelta/1000;
  counter = (float(millis())/1000) - delta;
}

void showOLEDMsg(float time) {
  display.setTextSize(1);
  display.setCursor(0, 110);
  display.printf("Runtime:\n%.1f sec", time);
}

void showBattery(float voltage, float percent) {
  rawValue = analogRead(A13);
  voltageLevel = (rawValue/MAX_ANALOG_VAL)*2*1.1*3.3;
  batteryFrac = voltageLevel / MAX_BATTERY_VOLTAGE;

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("Voltage:  %.3fV/4.2V\n\n\n\n   Battery:%.2f%%", voltage, percent);
}

void batteryIndicator(float battpercent) {
  display.drawRoundRect(8, 10, 108, 19, 3, 1);
  display.fillRoundRect(8, 10, 108, 19, 3, 1);
  display.fillRoundRect(116, 15, 4, 8, 2, 1);
  display.fillRect(116, 15, 3, 8, 1);

  display.fillRect(11, 13, 102, 13, 0);
  if(battpercent < 100) {
    display.fillRect(12, 14, int(battpercent), 11, 1);
  }
  if(battpercent >= 100) {
    display.fillRect(12, 14, 100, 11, 1);
  }
}
