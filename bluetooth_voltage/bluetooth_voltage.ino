#include <BluetoothSerial.h>

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ledpin 13
byte BTdata;
BluetoothSerial SerialBT;

const float MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2;

float counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

  SerialBT.begin();
  Serial.println("Bluetooth enabled! Ready to pair...");
}

void loop() {
  // put your main code here, to run repeatedly:
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
  if(BTdata == 'V') {
    batteryInfo();
  }


  long end = millis();
  float tempdelta = (end-begin);
  float delta = tempdelta/1000;

  counter += delta;
}

void batteryInfo() {
  float rawVal = analogRead(A13);
  float voltLev = (rawVal/MAX_ANALOG_VAL)*2*1.1*3.3;
  float battFrac = voltLev/MAX_BATTERY_VOLTAGE;
  float percentSum = 0;
  float voltageSum = 0;

  float percentTable[500];
  float voltageTable[500];
  for(int i = 0; i < 499; i++) {
    percentTable[i] = (battFrac * 100);
    voltageTable[i] = voltLev;
  }
  for(int i = 0; i < 499; i++) {
    percentSum += percentTable[i];
    voltageSum += voltageTable[i];
  }
  float battPercent = percentSum/500;
  float voltage = voltageSum/500;

  SerialBT.printf("Battery Percentage is: %.2f%%\nVoltage is: %.2f/4.20\n", battPercent, voltage);
}