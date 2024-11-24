#include <Arduino.h>
#include <avr/io.h>
#include "displayhandler.h"

#define LED_PINS PIN_PA2,PIN_PA1,PIN_PC3,PIN_PC2,PIN_PC1,PIN_PC0,PIN_PB1,PIN_PB4,PIN_PB5,PIN_PA7,PIN_PA6,PIN_PA5
#define HOUR_GND_PIN PIN_PA4
#define MIN_GND_PIN PIN_PA3
#define Blink_duration 10
#define Diplay_duration 4000

int initdisplay()
{
    // Set Pins as output
    pin_size_t pinsLed [] = {LED_PINS};
    for (size_t i = 0; i < 12; i++)
    {
        pinMode(pinsLed[i],OUTPUT);
    
    }
    pinMode(MIN_GND_PIN,OUTPUT);
    pinMode(HOUR_GND_PIN,OUTPUT);
}



/*
This function handles the LEDs to display a supplied time

Input: 
    int hour: The hour in format [0-24]
    int minute: the minute in format [0-60]
    
Output: 1 if successfull
*/

int displayTime(int hour, int minute)
{
  
  pin_size_t pinsLed [] = {LED_PINS};
  int iLedHour; int iLedMin;
    
  // calculate which LED to turn on

  if(hour > 12)
  {
    iLedHour = hour - 12;
  }
  else 
  {
    iLedHour = hour;
  }
   

  iLedMin = (int) round((double) minute / 5);
  // Enable LED
  for (size_t i = 0; i < (int) (Diplay_duration / Blink_duration); i++)
  {
    digitalWrite(HOUR_GND_PIN,HIGH);
    digitalWrite(MIN_GND_PIN,LOW);
    digitalWrite(pinsLed[iLedHour],HIGH);
    digitalWrite(pinsLed[iLedMin],LOW);
    

    delay(Blink_duration);

    digitalWrite(HOUR_GND_PIN,LOW);
    digitalWrite(MIN_GND_PIN,HIGH);
    digitalWrite(pinsLed[iLedHour],LOW);
    digitalWrite(pinsLed[iLedMin],HIGH);
    
    delay(Blink_duration);

  }
  digitalWrite(pinsLed[iLedHour],LOW);
  digitalWrite(pinsLed[iLedMin],LOW);
  digitalWrite(HOUR_GND_PIN,LOW);
  digitalWrite(MIN_GND_PIN,LOW);
  
  return 1;
}

int debugBlink ()
{
  digitalWrite(PIN_PA1,HIGH);
  digitalWrite(HOUR_GND_PIN,HIGH);
  delay(500);
  digitalWrite(PIN_PA1,LOW);
  digitalWrite(HOUR_GND_PIN,LOW);
}