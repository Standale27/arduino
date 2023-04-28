#include <BluetoothSerial.h>
#include <stdio.h>

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ledpin 13   //Onboard LED
byte BTdata;
BluetoothSerial SerialBT;

const float MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2;
float counter = 0;

float overallPTable[1000];    //Global tables which will help to track battery pecentage and voltage
float overallVTable[1000];
float timeTable[1000];

int pPointer = 0;   //Pointers for the global tablees
int vPointer = 0;
int tPointer = 0;

float rawVal = 0;
float voltLev = 0;
float battFrac = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

  SerialBT.begin();
  Serial.println("Bluetooth enabled! Ready to pair...");
}

void loop() {
  long begin = millis();

  if(Serial.available()) {      // Read from laptop serial monitor and output to phone serial monitor
    SerialBT.write(Serial.read());
  }

  if(SerialBT.available()) {    // Read from phone serial monitor and output to laptop serial monitor
    BTdata = SerialBT.read();
    if(BTdata == '1' || BTdata == '0' || BTdata == 'V') {}
    else{
      Serial.write(BTdata);
    }
  }

  if(BTdata == '1') {             // If input from phone is 1 or 0, turn onboard LED on or off respectively
    digitalWrite(ledpin, HIGH);
  }
  if(BTdata == '0') {
    digitalWrite(ledpin, LOW);
  }
  if(BTdata == 'V') {             //If input is v, write battery information to the bluetooth monitor
    batteryInfo();
  }


  long end = millis();
  float tempdelta = (end-begin);
  float delta = tempdelta/1000;

  counter = (float(millis())/1000) - delta;

}

void batteryInfo() {
  float percentSum = 0;   //Used for calculating moving averages
  float voltageSum = 0;

  for(int i = 0; i < 100; i++) {   //Take 100 samples of A13 and calculate battery voltage and perecentage, keep track of sum
    batCalc();
    percentSum += (battFrac * 100);
    voltageSum += voltLev;
  }

  float battPercent = percentSum/100;   //Complete the average calculation
  float voltage = voltageSum/100;

  overallPTable[pPointer] = percentSum;   //Add in the averaged percentage, voltage, and associated time to global arrays to keep track of battery usage over time
  overallVTable[vPointer] = voltageSum;
  timeTable[tPointer] = counter;

  pPointer = (pPointer+1)%sizeof(overallPTable);   //increment the pointer keeping track of where we are in the global arrays
  vPointer = (vPointer+1)%sizeof(overallVTable);    //These will roll over back to index 0 once 1000 samples have been taken
  tPointer = (tPointer+1)%sizeof(timeTable);

  SerialBT.printf("Battery Percentage is: %.2f%%\nVoltage is: %.2f/4.20\nRuntime: %.1f seconds\nData Point %i/1000\n", battPercent, voltage, counter, tPointer);
}

void batCalc() {    //Function calculates voltage level and battery percentage at a single point in time
  rawVal = analogRead(A13);   //A13 is an internal pin and is correlated to battery voltagee
  voltLev = (rawVal/MAX_ANALOG_VAL)*2*1.1*3.3;    //Correction factor, accounts for voltage divider
  battFrac = voltLev/MAX_BATTERY_VOLTAGE;
}
