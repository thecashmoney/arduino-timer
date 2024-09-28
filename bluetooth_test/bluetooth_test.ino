#include "TM1637Display.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7,6);

#define CLK 4
#define DIO 3
#define GREEN 2
#define DONE 5
int sec;
int min;

TM1637Display display = TM1637Display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:

  //serial for bluetooth
  mySerial.begin(9600);
  Serial.begin(9600);

  //declare leds
  pinMode(DONE, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available()) {
    int i = mySerial.read();
    Serial.println("DATA RECEIVED:");
    if(i == '1') {
      digitalWrite(DONE, 10);
      Serial.println("On");
    }
    if(i == '0') {
      digitalWrite(DONE, 0);
      Serial.println("off");
    }
  }
}
