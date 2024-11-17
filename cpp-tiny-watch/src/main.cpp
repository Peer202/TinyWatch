#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PC2, OUTPUT);
}

void loop() {
  digitalWrite(PIN_PC2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(PIN_PC2, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
