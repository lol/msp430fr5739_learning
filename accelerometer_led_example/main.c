#include "msp430fr5739.h"
#include "led.h"
#include "accel.h"

void main(void) 
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    AccelInit(); //Setup the accelerometer pins.
    SetupAccel();	//Setup the ADC and Accel
    CalibrateADC(); //Find zero points of all 3 axis
    ADC10CTL0 |= ADC10ENC | ADC10SC; //Start the first sample. If this is not done the ADC10 interupt will not trigger.
    LEDonboardInit(0);

    while(1)
	{  
          if (ADCResult_X < CalValue_X - 30)        
          {
          LED_off_all();
          LED_on(7);
          }
          else if((ADCResult_X > (CalValue_X - 30)) && (ADCResult_X < (CalValue_X-20)))
          {
            LED_off_all();
            LED_on(6);
          }
          else if((ADCResult_X > (CalValue_X - 20)) && (ADCResult_X < (CalValue_X-10)))
          {
            LED_off_all();
            LED_on(5);
          }
          else if((ADCResult_X > (CalValue_X - 10)) && (ADCResult_X < (CalValue_X+10)))
          {
            LED_off_all();
          }
          else if((ADCResult_X < (CalValue_X + 20)) && (ADCResult_X > (CalValue_X+10)))
          {
            LED_off_all();
            LED_on(4);
          }
          else if((ADCResult_X < (CalValue_X + 30)) && (ADCResult_X > (CalValue_X+20)))
          {
            LED_off_all();
            LED_on(3);
          }
          else if(ADCResult_X > CalValue_X+30)
          {
            LED_off_all();
            LED_on(2);
          }
        }
}