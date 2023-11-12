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

float counter = 0;                      //Used to keep track of the runtime
float rawValue;                         //These 3 are used to determine the battery percentage and voltage
float voltageLevel;
float batteryFrac;

//Constantly updating the battery indicator level every loop iteration is jarring, and it would be best to update it every few seconds.
float updateFreqTimer;                  //Used to keep track of how many seconds it's been since the battery indicator was updated
float updateFreq = 10;                  //How often the battery indicator should update in seconds
int battIndicTemp;                      //Since the screen is cleared at the beginning of every loop, it's necessary to keep track of what level the battery indicator was at every update interval to redraw the battery level

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
  updateFreqTimer = 0;
}

void loop() {
  long begin = millis();

  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
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
  updateFreqTimer += delta;                               //Counter keeps track of the uptime for the code, and delta keeps track of how long each loop iteration generally takes
  counter = (float(millis())/1000) - delta;               //Amended timekeeping solution. Basically allows for counter to increment without taking into account the delta time req to run the code.
}

void showOLEDMsg(float sec) {                            //Very simple. Prints the runtime in a specific format to a specific position
  int secDecimal = int((sec - floor(sec))*10);

  display.setTextSize(1);
  display.setCursor(4, 46);
  display.printf("Runtime:");
  display.setCursor(4, 54);
//Allows for different time formats based on the length of the runtime
  if(sec < 60) {                                                                                    //sec
    display.printf("%.1f sec", sec);
  } else if(sec < 3600) {                                                                           //min:sec
    display.printf("%i min %i.%i sec", int(sec)/60, int(sec)%60, secDecimal);
  } else {                                                                                          //hr:min:sec
    display.printf("%i h %i m %i.%i s", int(sec)/3600, (int(sec)/60)%60, int(sec)%60, secDecimal);
  }
}

void showBattery(float voltage, float percent) {          //Logic to determine battery voltage and percentage. Notably, utilizes the constrain func to show a 0-100% correlated to 3.5V-4.2V. Prints the text too.
  rawValue = analogRead(A13);
  voltageLevel = (rawValue/MAX_ANALOG_VAL)*2*1.1*3.3;
  batteryFrac = constrain((voltageLevel-3.5),0,0.7) / (MAX_BATTERY_VOLTAGE-3.5);

  display.setTextSize(1);
  display.setCursor(4, 3);
  display.printf("Voltage: %.3fV/4.2V\n\n\n   Battery:%.2f%%", voltage, percent);
}

void batteryIndicator(float battpercent) {                //Draws shapes to show an empty battery, along with a bar that changes in width depending on the battery percentage
  display.drawRoundRect(8, 13, 108, 10, 2, 1);
  display.fillRoundRect(8, 13, 108, 10, 2, 1);
  display.fillRoundRect(116, 16, 3, 4, 1, 1);
  display.fillRect(116, 16, 2, 4, 1);
  display.fillRect(11, 16, 102, 4, 0);

//This shows a battery filling up animation until then. Battery indicator level is not drawn until the first update interval passes.
  if(counter < updateFreq) {                          
    display.fillRect(12+int(counter*10), 17, 100-int(counter*20), 2, 1);
    if(counter > updateFreq/2) {
      display.fillRect(12, 17, int((counter-updateFreq/2)*10), 2, 1);
      display.drawLine(111, 17, 110-int((counter-updateFreq/2)*10), 17, 1);
      display.drawLine(111, 18, 110-int((counter-updateFreq/2)*10), 18, 1);
    }
  }

//Because the screen is cleared at the beginning of every loop, the battery indicator must be redrawn every loop, not just at every update. This will prevent the updates from being too jarring to look at
  if(updateFreqTimer >= updateFreq) {                     //If the timer surpasses how frequently the battery indicator should update,
    updateFreqTimer = 0;                                  //reset the timer,
    display.fillRect(12, 17, int(battpercent), 2, 1);     //draw the updated battery indicator,
    battIndicTemp = int(battpercent);                     //and keep track of what this updated battery indicator looks like.
  } else {
    display.fillRect(12, 17, battIndicTemp, 2, 1);
  }
}
