#include <Arduino.h>

#include <avr/io.h>           // for GPIO
#include <avr/sleep.h>        // for sleep functions
#include <avr/interrupt.h>    // for interrupts

#include <displayhandler.h>
#include <clockhandler.h>

typedef struct time
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
};

time t;

void incrementTime(void);
void initPIT(void);
void initTime(void);

void LED0_init(void)
{
  /* Make High (OFF) */
  PORTB.OUT |= PIN5_bm;
  /* Make output */
  PORTB.DIR |= PIN5_bm;
  }
  inline void LED0_toggle(void)
  {
  PORTB.OUTTGL |= PIN5_bm;
}


void initPIT(){
  while (RTC.STATUS > 0)
  {
  ; /* Wait for all register to be synchronized */
  }
  //_PROTECTED_WRITE(CLKCTRL_OSC32KCTRLA, CLKCTRL_ENABLE_bm);  // enable OSCULAP 32K internal as Clock Source
  RTC.CLKSEL     = RTC_CLKSEL_INT1K_gc;                     // select external 32K crystal
  RTC.DBGCTRL = RTC_DBGRUN_bm;
  RTC.PITINTCTRL = RTC_PI_bm;                                 // enable periodic interrupt
  RTC.PITCTRLA   = RTC_PERIOD_CYC1024_gc                    // set period to 1 second
                 | RTC_PITEN_bm;                              // enable PIT

}

ISR(RTC_PIT_vect){
  RTC.PITINTFLAGS = RTC_PI_bm;                  // clear interrupt flag
  LED0_toggle();
  //incrementTime();
  //displayTime((int) t.hour,(int) t.minute);
}


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

void incrementTime(void)
{
  t.second++;
  if(t.second >= 60)
  {
    // minute wrap
    t.second = 0;
    t.minute++;
  }
  //t.minute = t.minute + 5;
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

int main(void)
{
  //SETUP
  //_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 3);
  initTime();
  initdisplay();
  //LED0_init();
  initPIT();
  // enable Global Interrupt Mask
  sei(); 
  //debugBlink();

  displayTime((int) t.hour,(int) t.minute);
  
  
  // Loop
  while(1) {                                    // loop until forever                         
    //sleep_cpu();                                // sleep until next second
    //debugBlink();
  }
}

/*
void setup() {
  
}

//RTC_PIT_vect
void loop() {
  //debugBlink();
}
*/


