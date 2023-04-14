#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

int count = 0;
float dt = 0.1;

struct Clock {
  int h1;
  int h2;
  int m1;
  int m2;
  int s1;
  int s2;
  bool am;
};

int changeDigitUp = 2;
int changeDigitDown = 3;
int increase = 4;
int decrease = 5;

int ledpin = 13;

struct Clock clock_i;
long now = 0, last_time = 0;

void setup() {
  Serial.begin(9600);
  pinMode(changeDigitUp, INPUT);
  pinMode(changeDigitDown, INPUT);
  pinMode(increase, INPUT);
  pinMode(decrease, INPUT);
  pinMode(ledpin, OUTPUT);
  clock_i.h1 = 1, clock_i.h2 = 2, clock_i.m1 = 0, clock_i.m2 = 0, clock_i.s1 = 0, clock_i.s2 = 0;
  clock_i.am = 1;

  now = millis();
// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  long startTime = millis();

  logic();
  display.clearDisplay();
  draw();
  display.display();

  long endTime = millis();
  long duration = endTime - startTime;
  dt = (float)duration / 1000.0;
}

void logic(){
  clock();
}

void draw(){
  printTime();  
}

void clock(){
  for(int i = 0; i < 5; i++) {
    while((now-last_time) < 200) {
      now = millis();
    }
    last_time = now;
    changeTime();
  }

  clock_i.s2++;
  if(clock_i.s2 == 10){
    clock_i.s1++;
    clock_i.s2 = 0;
  }
  if(clock_i.s1 == 6){
    clock_i.m2++;
    clock_i.s1 = 0;
  }
  if(clock_i.m2 == 10){
    clock_i.m1++;
    clock_i.m2 = 0;
  }
  if(clock_i.m1 == 6){
    clock_i.h2++;
    clock_i.s2 = clock_i.s2 + 2; // Correction factor, addresses the fact that the clock lags behind real time by 2 seconds every hour. In 24 hours, I was behind by 48 seconds.
    clock_i.m1 = 0;
  }
  if(clock_i.h2 == 10){
    clock_i.h1++;
    clock_i.h2 = 0;        
  }

  if(clock_i.s2 == 0){      // This mess allows for the changeover from AM to PM and vice versa
    if(clock_i.s1 == 0){
      if(clock_i.m2 == 0){
        if(clock_i.m1 == 0){
          if(clock_i.h2 == 2){
            if(clock_i.am == 1){
              clock_i.am = 0;
            }
            else{
              clock_i.am = 1;      
            }
          }
        }
      }
    }
  }
  if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
    if(clock_i.h2 == 3){
      clock_i.h1--;
      clock_i.h2 = 1;
    }
  }
}

void printTime(){
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(2, display.height()/4);

  display.print(clock_i.h1);
  display.print(clock_i.h2);
  if(clock_i.s2 % 2 == 1){
      display.print(F(":"));
  }
  else{
      display.print(F(" "));
  }
  display.print(clock_i.m1);
  display.print(clock_i.m2);

  display.setTextSize(2);
  display.setCursor(6, display.height()-16);
  if(clock_i.am == 1){
    display.print(F("AM"));
  }
  if(clock_i.am == 0){
    display.print(F("PM"));
  }
  
  display.setCursor(display.width()-32, display.height()-16);
  display.print(clock_i.s1);
  display.print(clock_i.s2);
}

void changeTime(){
  if(digitalRead(changeDigitUp) == HIGH || changeDigitDown == HIGH) {
    digitalWrite(ledpin, HIGH);
    count++;
    while(count > 0){
      if(count == 1){
        display.drawRect(display.width()-21, display.height()-18, 12, 18, 1);
        display.drawRect(display.width()-33, display.height()-18, 12, 18, 0);
        display.display();

        if(digitalRead(increase) == HIGH) {
          clock_i.s2++;

          if(clock_i.s2 == 10){
            clock_i.s1++;
            clock_i.s2 = 0;

            if(clock_i.s1 == 6){
              clock_i.m2++;
              clock_i.s1 = 0;

              if(clock_i.m2 == 10){
                clock_i.m1++;
                clock_i.m2 = 0;

                  if(clock_i.m1 == 6){
                    clock_i.h2++;
                    clock_i.m1 = 0;

                    if(clock_i.h2 == 10){
                      clock_i.h1++;
                      clock_i.h2 = 0;

                      display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                      display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                      display.setTextSize(4);
                      display.setCursor(2, display.height()/4);
                      display.print(clock_i.h1);
                      display.print(clock_i.h2);
                    }
                  display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                  display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                  display.setTextSize(4);
                  display.setCursor(2, display.height()/4);
                  display.print(" ");
                  display.print(clock_i.h2);
                  display.print(":");
                  display.print(clock_i.m1);   
                }
                display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
                display.setTextSize(4);
                display.setCursor(2, display.height()/4);
                display.print("   ");
                display.print(clock_i.m1);
                display.print(clock_i.m2);
              }
              display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print("    ");
              display.print(clock_i.m2);
            }
            display.fillRect(display.width()-33, display.height()-18, 12, 18, 0);
            display.setCursor(display.width()-32, display.height()-16);
            display.setTextSize(2);
            display.print(clock_i.s1);
            display.print(clock_i.s2);
          }
          display.fillRect(display.width()-21, display.height()-18, 12, 18, 0);
          display.setCursor(display.width()-32, display.height()-16);
          display.setTextSize(2);
          display.print(" ");
          display.print(clock_i.s2);

          if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
            if(clock_i.h2 == 3){
              clock_i.h1--;
              clock_i.h2 = 1;

              display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(clock_i.h1);
              display.print(clock_i.h2);
            }
          }
        }
        if(digitalRead(decrease) == HIGH) {
          clock_i.s2--;

          if(clock_i.s2 == -1){
            clock_i.s1--;
            clock_i.s2 = 9;

            if(clock_i.s1 == -1){
              clock_i.m2--;
              clock_i.s1 = 5;

              if(clock_i.m2 == -1){
                clock_i.m1--;
                clock_i.m2 = 9;

                  if(clock_i.m1 == -1){
                    clock_i.h2--;
                    clock_i.m1 = 5;

                    if(clock_i.h2 == -1){
                      clock_i.h1--;
                      clock_i.h2 = 9;

                      display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                      display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                      display.setTextSize(4);
                      display.setCursor(2, display.height()/4);
                      display.print(clock_i.h1);
                      display.print(clock_i.h2);
                    }
                  display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                  display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                  display.setTextSize(4);
                  display.setCursor(2, display.height()/4);
                  display.print(" ");
                  display.print(clock_i.h2);
                  display.print(":");
                  display.print(clock_i.m1);   
                }
                display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
                display.setTextSize(4);
                display.setCursor(2, display.height()/4);
                display.print("   ");
                display.print(clock_i.m1);
                display.print(clock_i.m2);
              }
              display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
              display.setTextSize(4); // Draw 2X-scale text
              display.setCursor(2, display.height()/4);
              display.print("    ");
              display.print(clock_i.m2);
            }
            display.fillRect(display.width()-33, display.height()-18, 12, 18, 0);
            display.setCursor(display.width()-32, display.height()-16);
            display.setTextSize(2);
            display.print(clock_i.s1);
            display.print(clock_i.s2);
          }
          display.fillRect(display.width()-21, display.height()-18, 12, 18, 0);
          display.setCursor(display.width()-32, display.height()-16);
          display.setTextSize(2);
          display.print(" ");
          display.print(clock_i.s2);

          if(clock_i.h1 == 0){      // Change from 12:59 to 1:00
            if(clock_i.h2 == 0){
              clock_i.h1++;
              clock_i.h2 = 2;

              display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(clock_i.h1);
              display.print(clock_i.h2);
            }
          }
        }
      }
      if(count == 2){
        display.drawRect(display.width()-21, display.height()-18, 12, 18, 0);
        display.drawRect(display.width()-33, display.height()-18, 12, 18, 1);
        display.drawRect(display.width()-32, display.height()-50, 24, 32, 0);
        display.display();

        if(digitalRead(increase) == HIGH) {
          clock_i.s1++;

          if(clock_i.s1 == 6){
            clock_i.m2++;
            clock_i.s1 = 0;

            if(clock_i.m2 == 10){
              clock_i.m1++;
              clock_i.m2 = 0;

                if(clock_i.m1 == 6){
                  clock_i.h2++;
                  clock_i.m1 = 0;

                  if(clock_i.h2 == 10){
                    clock_i.h1++;
                    clock_i.h2 = 0;

                    display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                    display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                    display.setTextSize(4);
                    display.setCursor(2, display.height()/4);
                    display.print(clock_i.h1);
                    display.print(clock_i.h2);
                  }
                display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                display.setTextSize(4);
                display.setCursor(2, display.height()/4);
                display.print(" ");
                display.print(clock_i.h2);
                display.print(":");
                display.print(clock_i.m1);   
              }
              display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print("   ");
              display.print(clock_i.m1);
              display.print(clock_i.m2);
            }
            display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
            display.setTextSize(4); // Draw 2X-scale text
            display.setCursor(2, display.height()/4);
            display.print("    ");
            display.print(clock_i.m2);
          }
          display.fillRect(display.width()-33, display.height()-18, 12, 18, 0);
          display.setCursor(display.width()-32, display.height()-16);
          display.setTextSize(2);
          display.print(clock_i.s1);
        }

        if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 3){
            clock_i.h1--;
            clock_i.h2 = 1;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
        if(digitalRead(decrease) == HIGH) {
          clock_i.s1--;

          if(clock_i.s1 == -1){
            clock_i.m2--;
            clock_i.s1 = 5;

            if(clock_i.m2 == -1){
              clock_i.m1--;
              clock_i.m2 = 9;

                if(clock_i.m1 == -1){
                  clock_i.h2--;
                  clock_i.m1 = 5;

                  if(clock_i.h2 == -1){
                    clock_i.h1--;
                    clock_i.h2 = 9;

                    display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                    display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                    display.setTextSize(4);
                    display.setCursor(2, display.height()/4);
                    display.print(clock_i.h1);
                    display.print(clock_i.h2);
                  }
                display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
                display.setTextSize(4);
                display.setCursor(2, display.height()/4);
                display.print(" ");
                display.print(clock_i.h2);
                display.print(":");
                display.print(clock_i.m1);   
              }
              display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print("   ");
              display.print(clock_i.m1);
              display.print(clock_i.m2);
            }
            display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
            display.setTextSize(4); // Draw 2X-scale text
            display.setCursor(2, display.height()/4);
            display.print("    ");
            display.print(clock_i.m2);
          }
          display.fillRect(display.width()-33, display.height()-18, 12, 18, 0);
          display.setCursor(display.width()-32, display.height()-16);
          display.setTextSize(2);
          display.print(clock_i.s1);
        }

        if(clock_i.h1 == 0){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 0){
            clock_i.h1++;
            clock_i.h2 = 2;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
      }
      if(count == 3){
        display.drawRect(display.width()-33, display.height()-18, 12, 18, 0);
        display.drawRect(display.width()-32, display.height()-50, 24, 32, 1);
        display.drawRect(display.width()-56, display.height()-50, 24, 32, 0);
        display.display();

        if(digitalRead(increase) == HIGH) {
          clock_i.m2++;

          if(clock_i.m2 == 10){
            clock_i.m1++;
            clock_i.m2 = 0;

              if(clock_i.m1 == 6){
                clock_i.h2++;
                clock_i.m1 = 0;

                if(clock_i.h2 == 10){
                  clock_i.h1++;
                  clock_i.h2 = 0;

                  display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                  display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                  display.setTextSize(4);
                  display.setCursor(2, display.height()/4);
                  display.print(clock_i.h1);
                  display.print(clock_i.h2);
                }
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(" ");
              display.print(clock_i.h2);
              display.print(":");
              display.print(clock_i.m1);   
            }
            display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print("   ");
            display.print(clock_i.m1);
            display.print(clock_i.m2);
          }
          display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
          display.setTextSize(4); // Draw 2X-scale text
          display.setCursor(2, display.height()/4);
          display.print("    ");
          display.print(clock_i.m2);
        }

        if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 3){
            clock_i.h1--;
            clock_i.h2 = 1;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
        if(digitalRead(decrease) == HIGH) {
          clock_i.m2--;

          if(clock_i.m2 == -1){
            clock_i.m1--;
            clock_i.m2 = 9;

              if(clock_i.m1 == -1){
                clock_i.h2--;
                clock_i.m1 = 5;

                if(clock_i.h2 == -1){
                  clock_i.h1--;
                  clock_i.h2 = 9;

                  display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
                  display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
                  display.setTextSize(4);
                  display.setCursor(2, display.height()/4);
                  display.print(clock_i.h1);
                  display.print(clock_i.h2);
                }
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(" ");
              display.print(clock_i.h2);
              display.print(":");
              display.print(clock_i.m1);   
            }
            display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print("   ");
            display.print(clock_i.m1);
            display.print(clock_i.m2);
          }
          display.fillRect(display.width()-32, display.height()-50, 24, 32, 0);
          display.setTextSize(4); // Draw 2X-scale text
          display.setCursor(2, display.height()/4);
          display.print("    ");
          display.print(clock_i.m2);
        }
        if(clock_i.h1 == 0){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 0){
            clock_i.h1++;
            clock_i.h2 = 2;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
      }
      if(count == 4){
        display.drawRect(display.width()-32, display.height()-50, 24, 32, 0);
        display.drawRect(display.width()-56, display.height()-50, 24, 32, 1);
        display.drawRect(display.width()-128, display.height()-50, 48, 32, 0);
        display.display();

        if(digitalRead(increase) == HIGH) {
          clock_i.m1++;
          if(clock_i.m1 == 6){
            clock_i.h2++;
            clock_i.m1 = 0;

            if(clock_i.h2 == 10){
              clock_i.h1++;
              clock_i.h2 = 0;

              display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(clock_i.h1);
              display.print(clock_i.h2);
            }
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(" ");
            display.print(clock_i.h2);
            display.print(":");
            display.print(clock_i.m1);   
          }
          display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
          display.setTextSize(4);
          display.setCursor(2, display.height()/4);
          display.print("   ");
          display.print(clock_i.m1);
        }
        if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 3){
            clock_i.h1--;
            clock_i.h2 = 1;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
        if(digitalRead(decrease) == HIGH) {
          clock_i.m1--;
          if(clock_i.m1 == -1){
            clock_i.h2--;
            clock_i.m1 = 5;

            if(clock_i.h2 == -1){
              clock_i.h1--;
              clock_i.h2 = 9;

              display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
              display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
              display.setTextSize(4);
              display.setCursor(2, display.height()/4);
              display.print(clock_i.h1);
              display.print(clock_i.h2);
            }
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(" ");
            display.print(clock_i.h2);
            display.print(":");
            display.print(clock_i.m1);   
          }
          display.fillRect(display.width()-56, display.height()-50, 24, 32, 0);
          display.setTextSize(4);
          display.setCursor(2, display.height()/4);
          display.print("   ");
          display.print(clock_i.m1);
        }
        if(clock_i.h1 == 0){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 0){
            clock_i.h1++;
            clock_i.h2 = 2;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
      }
      if(count == 5){
        display.drawRect(display.width()-56, display.height()-50, 24, 32, 0);
        display.drawRect(display.width()-128, display.height()-50, 48, 32, 1);
        display.drawRect(display.width()-124, display.height()-18, 26, 18, 0);
        display.display();

        if(digitalRead(increase) == HIGH) {
          clock_i.h2++;

          if(clock_i.h2 == 10){
            clock_i.h1++;
            clock_i.h2 = 0;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
          display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
          display.setTextSize(4);
          display.setCursor(2, display.height()/4);
          display.print(" ");
          display.print(clock_i.h2);
          display.print(":"); 
        }
        if(clock_i.h1 == 1){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 3){
            clock_i.h1--;
            clock_i.h2 = 1;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
        if(digitalRead(decrease) == HIGH) {
          clock_i.h2--;

          if(clock_i.h2 == -1){
            clock_i.h1--;
            clock_i.h2 = 9;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
          display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
          display.setTextSize(4);
          display.setCursor(2, display.height()/4);
          display.print(" ");
          display.print(clock_i.h2);
          display.print(":");
        }
        if(clock_i.h1 == 0){      // Change from 12:59 to 1:00
          if(clock_i.h2 == 0){
            clock_i.h1++;
            clock_i.h2 = 2;

            display.fillRect(display.width()-128, display.height()-50, 24, 32, 0);
            display.fillRect(display.width()-104, display.height()-50, 24, 32, 0);
            display.setTextSize(4);
            display.setCursor(2, display.height()/4);
            display.print(clock_i.h1);
            display.print(clock_i.h2);
          }
        }
      }
      if(count == 6){
        display.drawRect(display.width()-128, display.height()-50, 48, 32, 0);
        display.drawRect(display.width()-124, display.height()-18, 26, 18, 1);
        display.fillRect(display.width()-97, display.height()-16, 61, 16, 0);
        display.display();

        if(digitalRead(increase) == HIGH || digitalRead(decrease) == HIGH){
          if(clock_i.am == 1){
            clock_i.am = 0;
            display.fillRect(display.width()-124, display.height()-18, 26, 18, 0);
            display.setTextSize(2);
            display.setCursor(6, display.height()-16);
            display.print(F("PM"));
          }
          else{
            clock_i.am = 1;
            display.fillRect(display.width()-124, display.height()-18, 26, 18, 0);
            display.setTextSize(2);
            display.setCursor(6, display.height()-16);
            display.print(F("AM"));      
          }
        }
      }
      if(count >= 7){
        display.drawRect(display.width()-124, display.height()-18, 26, 18, 0);
        display.drawRect(display.width()-96, display.height()-15, 59, 14, 1);
        display.setCursor(35, display.height()-12);
        display.setTextSize(1);
        display.print(F("Set Time?"));
        display.display();
        if(digitalRead(increase) == HIGH){
          display.fillRect(display.width()-97, display.height()-16, 61, 16, 0);
          count = 0;
        }
        else if(digitalRead(changeDigitDown) == HIGH || digitalRead(decrease) == HIGH){
          display.fillRect(display.width()-97, display.height()-16, 61, 16, 0);
          count = 1;
        }
      }
      if(count < 0){
        count = 0;
      }
      delay(125);
      if(digitalRead(changeDigitUp) == HIGH) {
        count++;
      }
      if(digitalRead(changeDigitDown) == HIGH) {
        if(count > 1){
          count--;
        }
        else{
          count = 1;
        }
      }     
    }
  }
  else {
    digitalWrite(ledpin, LOW);
  }
}