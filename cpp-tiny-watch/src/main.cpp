//#include <Arduino.h>

#include <avr/io.h>           // for GPIO
#include <avr/sleep.h>        // for sleep functions
#include <avr/interrupt.h>    // for interrupts
#include <util/delay.h>       // for delay usage
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

// ===================================================================================
// UART Implementation (refer to TB3216)
// ===================================================================================

// UART definitions and macros
#define UART_BAUD       9600
#define UART_BAUD_RATE  4.0 * F_CPU / UART_BAUD + 0.5
#define UART_ready()    (USART0.STATUS & USART_DREIF_bm)
#define UART_flushed()  (USART0.STATUS & USART_TXCIF_bm)

// UART init
void UART_init(void) {
  PORTMUX.CTRLB = PORTMUX_USART0_bm;            // select alternative pins for USART0
  USART0.BAUD   = UART_BAUD_RATE;               // set BAUD
  USART0.CTRLB  = USART_TXEN_bm;                // enable TX
}

// UART transmit data byte
void UART_write(uint8_t data) {
  while(!UART_ready());                         // wait until ready for next data
  USART0.STATUS  = USART_TXCIF_bm;              // clear USART TX complete flag
  USART0.TXDATAL = data;                        // send data byte
}

// UART print string
void UART_print(const char *str) {
  while(*str) UART_write(*str++);               // write characters of string
}

// UART print 2-digit integer value via UART
void UART_printVal(uint8_t value) {
  UART_write((value / 10) + '0');
  UART_write((value % 10) + '0');
}

void TIME_send(void) {
  VPORTA.DIR |= PIN1_bm;                        // set TX pin as output  

  // Send time stamp
  UART_printVal(t.hour);   UART_write(':');
  UART_printVal(t.minute); UART_write(':');
  UART_printVal(t.second);

  while(!UART_flushed());                       // wait for UART TX to complete
  VPORTA.DIR &= ~PIN1_bm;                       // set TX pin as input to save power
}

// ===========================

void initButton()
{
  PORTB.DIRCLR = PIN0_bm;
  PORTB.PIN0CTRL = PORT_ISC_RISING_gc;
};

ISR(PORTB_PORT_vect){
  PORTB.INTFLAGS = PIN0_bm;                  // clear interrupt flag
  displayTime((int) t.hour,(int) t.minute);
}


void initPIT(){
  while (RTC.STATUS > 0)
  {
  ; /* Wait for all register to be synchronized */
  }
  //_PROTECTED_WRITE(CLKCTRL_OSC32KCTRLA, CLKCTRL_ENABLE_bm);  // enable OSCULAP 32K internal as Clock Source
  //RTC.CLKSEL     = RTC_CLKSEL_INT1K_gc;                     // select internal 1k Quartz
  RTC.DBGCTRL = RTC_DBGRUN_bm;
  RTC.PITINTCTRL = RTC_PI_bm;                                 // enable periodic interrupt
  RTC.PITCTRLA   = RTC_PERIOD_CYC32768_gc                  // set period to 1 second
                 | RTC_PITEN_bm;                              // enable PIT

}

ISR(RTC_PIT_vect){
  RTC.PITINTFLAGS = RTC_PI_bm;                  // clear interrupt flag
  incrementTime();
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
   //t.hour = 0;
   //t.minute = 0;
   //t.second = 0;
};

void incrementTime(void)
{
  //t.second++;
  t.second = t.second + 1;
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
  //TIME_send();
}


int main(void)
{
  uint8_t LED_state = 0;
  //SETUP ====================
  _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 3);

  initTime();
  initdisplay();
  initButton();
  initPIT();
  UART_init();

  // Test LED Setup
  SetPinDirOut(1,4);
  TogglePinOutLevel(1,4);

  
  // Prepare sleep mode
  SLPCTRL.CTRLA  = SLPCTRL_SMODE_PDOWN_gc       // set sleep mode to power down
                 | SLPCTRL_SEN_bm;              // enable sleep mode

  // enable Global Interrupt Mask
  sei(); 


 //Loop ====================
  while(1) {
    LED_state = !LED_state;
    TogglePinOutLevel(1,4);
    //TIME_send();
    UART_printVal(t.second);
    sleep_cpu();                                // sleep until next second
    //debugBlink();
  }
}
