#include "TM1637Display.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7,6);

#define CLK 4
#define DIO 3
#define DONE 2
#define BUTTON 5
#define SPEAKER 8

int sec = 3;
int min = 0;

TM1637Display display = TM1637Display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:

  //serial for bluetooth
  mySerial.begin(9600);
  Serial.begin(9600);

  //declare leds
  pinMode(DONE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //display shenanigans
  display.clear();
  delay(1000);
  display.setBrightness(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available()) {
    String i = mySerial.readString();
    Serial.println("DATA RECEIVED:");
    if(i[0] == 'S') {
      Serial.println("ENGAGE");
      tone(SPEAKER, 4231, 50);
      display.showNumberDecEx(0, 0b11100000, true);

      min = (((String)i[1]).toInt())*10 + ((String)i[2]).toInt();
      sec = (((String)i[3]).toInt())*10 + ((String)i[4]).toInt();
      display.showNumberDecEx((min * 100) + sec, 0b11100000, true);

      Serial.println("Waiting for confirmation");
      if(i[5] == 'C') timer();
      else {
        int c = 0;
        while(c < 5000){
          int state = digitalRead(BUTTON);
          if(state == LOW) {
            Serial.println("STARTING");
            timer();
            break;
          }
          delay(1);
          c++;
        }
      }
      Serial.println("CANCELLED");
      tone(SPEAKER, 2564, 50);
      display.showNumberDecEx(0, 0b11100000, true);
    }
    if(i[0] == 'R') {
      Serial.println("OFF");
      digitalWrite(DONE, 0);
    }
  }
}

void timer() {
  digitalWrite(DONE, 0);
  tone(SPEAKER, 4231, 50);
  for (int i = min * 60 + sec; i >= 0; i--) {
    display.showNumberDecEx((min * 100) + sec, 0b11100000, true);
    if(sec == 0 && min != 0) {
      min--;
      sec = 60;
    }
    sec--;
    delay(1000);
  }
  for (int i = 0; i < 3; i++) {
    tone(SPEAKER, 2564, 30);
    delay(100);
  }
  digitalWrite(DONE, 10);
}