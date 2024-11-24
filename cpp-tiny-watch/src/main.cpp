#include <Arduino.h>
#include <avr/io.h>

#include <displayhandler.h>

typedef struct time
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
};

time t;

uint8_t str2dec(const char *p) {
  return( (*p == ' ') ? (*(++p) - '0') : ((*p++ - '0') * 10 + (*p - '0')) );
}

void initTime(void)
{
    char *ptr = __TIME__;                         // format "23:59:01"
    t.hour   = str2dec(ptr); ptr += 3;            // hour
    t.minute = str2dec(ptr); ptr += 3;            // minute
    t.second = str2dec(ptr);                      // second
};

void incrementTime()
{
  t.second++;
  if(t.second >= 60)
  {
    // minute wrap
    t.second = 0;
    t.minute++;
  }
  if(t.minute >= 60)
  {
    // hour wrap
    t.minute = 0;
    t.hour ++;
  }
  if(t.hour >= 24)
  {
    t.hour = 0;
    t.minute = 0;
  }
}

void setup() {
  initTime();
  initdisplay();
  displayTime((int) t.hour,(int) t.minute);
}

void loop() {

}



