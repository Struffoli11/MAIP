#include <Wire.h> //
#include <Arduino.h>

#define TP1_positive_terminal 3
#define TP2_positve_terminal 11
#define TP1_amplified A0
#define TP2_amplified A1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); 
  analogWrite(TP1_positive_terminal, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  float val = analogRead(TP1_amplified);
  Serial.println(val);
  delay(1000);
}
