#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Protomatter.h>
#include <Adafruit_GFX.h>

#define HEIGHT 64
#define WIDTH 64
#define MAX_FPS 45
uint8_t addrPins[] = {17,18,19,20,21};
uint8_t rgbPins[] = {7,8,9,10,11,12};
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;

Adafruit_Protomatter matrix(WIDTH, 5, 1, rgbPins, 5, addrPins, clockPin, latchPin, oePin, true);
Adafruit_LIS3DH accel = Adafruit_LIS3DH();

float counter;

struct ball {
  int x;
  int y;
  int dx;
  int dy;
  int rad;

  int r;
  int g;
  int b;
};

struct ball b;

void setup() {
  Serial.begin(115200);
  ProtomatterStatus status = matrix.begin();
  matrix.setRotation(3);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.color565(255, 25, 255));
  matrix.printf("Testing...\n");
  matrix.setTextColor(matrix.color565(25, 255, 255));
  matrix.printf("Sahil\nTandale");
  matrix.show();
  delay(1000);


  b.x = 32;
  b.y = 12;
  b.dx = 1;
  b.dy = 1;
  b.rad = 2;
  b.r = 255;
  b.g = 255;
  b.b = 255;
}

void loop() {
  long begin = millis();
  matrix.fillScreen(matrix.color565(0, 0, 0));
  matrix.drawCircle(b.x, b.y, b.rad, matrix.color565(b.r, b.g, b.b));
  positionChange();
  matrix.show();
  long end = millis();
  float tDelta = end-begin;
  float delta = tDelta/1000;
  counter = (float(millis())/1000) - delta;
}

void positionChange() {
  if(b.x >= WIDTH || b.x <= 0) {
    b.dx = b.dx *-1;
    b.dx = constrain((b.dx * random(1,5)),1,5);
  }
  if(b.y >= HEIGHT || b.y <= 0) {
    b.dy = b.dy *-1;
    b.dy = constrain((b.dy * random(1,5)),1,5);
  }
  b.x = b.x + b.dx;
  b.y = b.y + b.dy;
/*
  if((b.x == 0 && b.y == 0) || (b.x == 64 && b.y == 0) || (b.x == 0 && b.y == 64) || (b.x == 64 && b.y == 64)) {
    b.x = 32;
    b.y = 24;
  } */
  Serial.println(b.x);
  Serial.println(b.y);
}

void colorChange() {

}
