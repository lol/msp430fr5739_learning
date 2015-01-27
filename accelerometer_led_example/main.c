/*
This program turns on the on-board LEDs in response to the accelerometer reading.

LEDs 7 to 2 are used.

Talking of X-axis, if the accelerometer is at a position close to its caliberated axis, then all LEDs will be off.
The LEDs are turned on according to the tilt. More the tilt, the farther LED will be turned on.

CalValue_X, CalValue_Y and CalValue_Z store the initial calibrated values by CalibrateADC() function.
ADCResult_X, ADCResult_Y and ADCResult_Z store the current ADC values.
*/

#include "msp430fr5739.h"
#include "led.h"
#include "accel.h"

void main(void) 
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    AccelInit(); //Setup the accelerometer pins.
    SetupAccel();	//Setup the ADC and Accel. ADC can only convert one axis at a time.
    CalibrateADC(); //Find zero points of all 3 axis.

    ADC10CTL0 |= ADC10ENC | ADC10SC; //Start the first sample. If this is not done the ADC10 interupt will not trigger.

    LEDonboardInit(0); //for using on-board LEDs.

    while(1)
	{  
          if (ADCResult_X < CalValue_X - 60)        
          {
          LED_off_all();
          LED_on(7);
          }
          else if((ADCResult_X > (CalValue_X - 60)) && (ADCResult_X < (CalValue_X - 40)))
          {
            LED_off_all();
            LED_on(6);
          }
          else if((ADCResult_X > (CalValue_X - 40)) && (ADCResult_X < (CalValue_X - 20)))
          {
            LED_off_all();
            LED_on(5);
          }
          else if((ADCResult_X > (CalValue_X - 20)) && (ADCResult_X < (CalValue_X + 20)))
          {
            LED_off_all();
          }
          else if((ADCResult_X < (CalValue_X + 40)) && (ADCResult_X > (CalValue_X + 20)))
          {
            LED_off_all();
            LED_on(4);
          }
          else if((ADCResult_X < (CalValue_X + 60)) && (ADCResult_X > (CalValue_X + 40)))
          {
            LED_off_all();
            LED_on(3);
          }
          else if(ADCResult_X > CalValue_X + 60)
          {
            LED_off_all();
            LED_on(2);
          }
        }
}