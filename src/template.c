// ELECTRONIC SANDTIMER CODE TEMPLATE
// Last update: 04 Sep 2019

#include <xc.h>

#pragma config FOSC = INTOSC  // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config MCLRE = ON     // Pin acts as MCLR
#pragma config WDTE = OFF     // Disable watchdog timer


// use __delay_ms() or __delay_us()
#define _XTAL_FREQ 250000

void startTimer(void);
void reconfigure(void);


void main()
{
  unsigned char debounceCount;

  // INITIALIZATION ---------------------------------------------------------------
  // OPTION_REG = 0b11011111;  // Timer0 clock is Fosc/4
  IRCF = 0x10; // IRCF=1 (250 kHz); default is 6 (8 MHz)
  TRISA = 0b11111100; // GP0 & GP1 as outputs; leave GP2 as input
  nWPUEN = 0; // Enable weak pull-ups (active low enable)
  WPUA2 = 1; // Enable weak pull for RA2

  // Check for configuration signal -----------------------------------------------
  debounceCount = 50; // Debounce configuration for 0.2 seconds
  while (RA2 == 0) // If input pads are held down
  {
    __delay_ms(4);
    debounceCount--;
    if (debounceCount == 0) // debounce complete?
      reconfigure();
  }
  nWPUEN = 1; // Disable weak pull-ups (active low enable)

  // Run timer --------------------------------------------------------------------
  startTimer();

  // Nighty-night! ----------------------------------------------------------------
  PWM1CON = 0; // Disable PWM1
  SLEEP();     // Conserve power until the next use
}



void startTimer()
{
  unsigned char blinkCount;
  PWM1CON = 0xC0; // Enable PWM1

  // Start bright, then dim down
  PWM1DCH = 0xFF; // set duty cycle for PWM1
  while (PWM1DCH > 0)
  {
    __delay_ms(10);
    PWM1DCH--;
  }

  // All done! Flash the other LED for a bit
  blinkCount = 44; // blink for 20 seconds
  while (blinkCount != 0)
  {
    RA1 = 1; // Activate LED
    __delay_ms(300);
    RA1 = 0; // Deactivate LED
    __delay_ms(150);
    blinkCount--;
  }
}


void reconfigure()
{
  // Activate both LEDs
  RA0 = 1;
  RA1 = 1;

  // Perform any desired reconfiguration options here
  while (1);
}

