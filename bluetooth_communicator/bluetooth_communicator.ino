#include <BluetoothSerial.h>

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ledpin 13
byte BTdata;
BluetoothSerial SerialBT;

const char *pin = "13579";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

  SerialBT.begin();
  Serial.println("Bluetooth enabled! Ready to pair...");

  SerialBT.setPin(pin);
  Serial.println("Using PIN: ");
  Serial.print(pin);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {      // Read from laptop serial monitor and output to phone serial monitor
    SerialBT.write(Serial.read());
  }

  if(SerialBT.available()) {    // Read from phone serial monitor and output to laptop serial monitor
    BTdata = SerialBT.read();
    if(BTdata == '1' || BTdata == '0') {}
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
}
