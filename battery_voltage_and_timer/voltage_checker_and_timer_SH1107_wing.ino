#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET, 1000000, 100000);

#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14

const float MAX_ANALOG_VAL = 4095;      //Pin A13 is internally tied to the battery and is associated with the battery voltage.
const float MAX_BATTERY_VOLTAGE = 4.2;  //Realistically, the actual max analog value that will be read will be ~2430

float counter = 0;    //Used to keep track of the runtime
float rawValue;       //These 3 are used to determine the battery percentage and voltage
float voltageLevel;
float batteryFrac;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  delay(100);
  display.begin(0x3C, true);    //Initialization. The AdaFruit splash screen is hardcoded into begin, and using display() followed by a delay and a clear allows for it to show up
  display.setRotation(1);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextColor(1);
}

void loop() {
  long begin = millis();

  display.clearDisplay();
  showOLEDMsg(counter);                                   //Displays the runtime in seconds
  showBattery(voltageLevel, (batteryFrac * 100));         //Shows the battery voltage and percentage in text
  batteryIndicator(constrain((batteryFrac*100),0,100));   //Shows the filling/depleting battery graphically

  display.setTextSize(2);
  if(digitalRead(BUTTON_A) == LOW) {
    display.setCursor(64, 46);
    display.print("A");
  }
  if(digitalRead(BUTTON_B) == LOW) {
    display.setCursor(76, 46);
    display.print("B");
  }
  if(digitalRead(BUTTON_C) == LOW) {
    display.setCursor(88, 46);
    display.print("C");
  }

  display.display();

  long end = millis();
  float tempdelta = (end-begin);
  float delta = tempdelta/1000;
  counter = (float(millis())/1000) - delta;               //Amended timekeeping solution. Basically allows for counter to increment without taking into account the delta time req to run the code.
}                                                         //Seems accurate, but further testing would help to confirm this

void showOLEDMsg(float time) {                            //Very simple. Prints the runtime in a specific format to a specific position
  display.setTextSize(1);
  display.setCursor(0, 46);
  display.printf("Runtime:\n%.1f sec", time);
}

void showBattery(float voltage, float percent) {          //Logic to determine battery voltage and percentage. Notably, utilizes the constrain func to show a 0-100% correlated to 3.5V-4.2V. Prints the text too.
  rawValue = analogRead(A13);
  voltageLevel = (rawValue/MAX_ANALOG_VAL)*2*1.1*3.3;
  batteryFrac = constrain((voltageLevel-3.5),0,0.7) / (MAX_BATTERY_VOLTAGE-3.5);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("Voltage:  %.3fV/4.2V\n\n\n   Battery:%.2f%%", voltage, percent);
}

void batteryIndicator(float battpercent) {                //Draws shapes to show an empty battery, along with a bar that changes in width depending on the battery percentage
  display.drawRoundRect(8, 10, 108, 10, 2, 1);
  display.fillRoundRect(8, 10, 108, 10, 2, 1);
  display.fillRoundRect(116, 13, 3, 4, 1, 1);
  display.fillRect(116, 13, 2, 4, 1);

  display.fillRect(11, 13, 102, 4, 0);
  display.fillRect(12, 14, int(battpercent), 2, 1);
}