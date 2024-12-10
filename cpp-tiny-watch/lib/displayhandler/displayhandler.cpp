#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "displayhandler.h"

//#define LED_PINS PIN_PA2,PIN_PA1,PIN_PC3,PIN_PC2,PIN_PC1,PIN_PC0,PIN_PB1,PIN_PB4,PIN_PB5,PIN_PA7,PIN_PA6,PIN_PA5
#define HOUR_GND_PIN PIN_PA4 
#define MIN_GND_PIN PIN_PA3
#define LED_PINS {{1,2},{1,1},{3,3},{3,2},{3,1},{3,0},{2,1},{2,4},{2,5},{1,7},{1,6},{1,5}}

#define BLINK_DURATION 1
//#define Diplay_duration 900
#define Diplay_duration 200

void TogglePinOutLevel(int Port,int PinNumber)
{ 
  int Pinmap[] = {PIN0_bm,PIN1_bm,PIN2_bm,PIN3_bm,PIN4_bm,PIN5_bm,PIN6_bm,PIN7_bm};

  switch (Port)
  {
  case 1:
    PORTA.OUTTGL = Pinmap[PinNumber];
    break;
  case 2:
    PORTB.OUTTGL = Pinmap[PinNumber];
    break;
  case 3:
    PORTC.OUTTGL = Pinmap[PinNumber];
    break;
  default:
    break;
  }
}

void SetPinDirOut(int Port,int PinNumber)
{ 
  int Pinmap[] = {PIN0_bm,PIN1_bm,PIN2_bm,PIN3_bm,PIN4_bm,PIN5_bm,PIN6_bm,PIN7_bm};
  switch (Port)
  {
  case 1:
    PORTA.DIRSET = Pinmap[PinNumber];
    break;
  case 2:
    PORTB.DIRSET = Pinmap[PinNumber];
    break;
  case 3:
    PORTC.DIRSET = Pinmap[PinNumber];
    break;
  default:
    break;
  }
}

int initdisplay()
{
    // Set Pins as output
    int pinsLed [][2] = LED_PINS;
    for (int i = 0; i < 12; i++)
    {
      SetPinDirOut(pinsLed[i][0],pinsLed[i][1]);
    }

    SetPinDirOut(1,4); // Hour Pin
    SetPinDirOut(1,3); // Minute Pin
}

void blinkHour(int ihour)
{
  int pinsLed [][2] = LED_PINS;
  TogglePinOutLevel(pinsLed[ihour][0],pinsLed[ihour][1]);
  TogglePinOutLevel(1,4);
  _delay_ms(BLINK_DURATION);
  TogglePinOutLevel(pinsLed[ihour][0],pinsLed[ihour][1]);
  TogglePinOutLevel(1,4);
}

void blinkMinute(int iMinute)
{
  int pinsLed [][2] = LED_PINS;
  TogglePinOutLevel(pinsLed[iMinute][0],pinsLed[iMinute][1]);
  TogglePinOutLevel(1,3);
  _delay_ms(BLINK_DURATION);
  TogglePinOutLevel(pinsLed[iMinute][0],pinsLed[iMinute][1]);
  TogglePinOutLevel(1,3);
}
/*
This function handles the LEDs to display a supplied time

Input: 
    int hour: The hour in format [0-24]
    int minute: the minute in format [0-60]
    
Output: 1 if successfull
*/

void displayTime(int hour, int minute)
{
  int pinsLed [][2] = LED_PINS;
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
  for (int i = 0; i < (int) (Diplay_duration / (BLINK_DURATION * 2 )); i++)
  {
    blinkHour(iLedHour);
    blinkMinute(iLedMin); 
  }
}
