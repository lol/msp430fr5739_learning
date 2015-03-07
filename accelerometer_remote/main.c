#include <msp430fr5739.h>
#include <stdint.h>
#include "led.h"
#include "nec.h"
#include "accel.h"


void main()
{
  WDTCTL = WDTPW + WDTHOLD;             //Stop Watchdog Timer
  CSCTL0_H = 0xA5;
    
  
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;                //set max. DCO setting.. 8Mhz
  CSCTL2 = SELA_3 + SELS_3 + SELM_3;            //set ACLK = MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;            //set all dividers
  
  P1DIR |= BIT0;
  P1SEL0 |= BIT0;
  
  TA0CCR0 = 207;                                //timer count value 207 for 38 kHz carrier.
  TA0CCR1 = 69;                                 //25% duty cycle for the 38 kHz carrier.
  //TA0CCTL1 = OUTMOD_7;
  TA0CCTL0 = 0x10;
  TA0CTL = TASSEL_1 + MC_1;
  
  
  LEDonboardInit(0);
  
  AccelInit();                          //Setup the accelerometer pins.
  SetupAccel();                         //Setup the ADC and Accel. ADC can only convert one axis at a time.
  CalibrateADC();                       //Find zero points of all 3 axis.

  ADC10CTL0 |= ADC10ENC | ADC10SC; //Start the first sample. If this is not done the ADC10 interupt will not trigger.
  
  __enable_interrupt();
  
  
   while(1)
	{
          TA0CTL |= MC_1;
          ADC10IE &= ~ADC10IE0;
          LED_off_all();
          if (ADCResult_X < CalValue_X - 60 && ADCResult_Y > CalValue_Y - 60 && ADCResult_Y < CalValue_Y + 60)        
          {
          LED_off_all();
          LED_on(8);
          transmit_command(0x04, 0x01);
          }
          else if(ADCResult_X > CalValue_X + 60 && ADCResult_Y > CalValue_Y - 60 && ADCResult_Y < CalValue_Y + 60)
          {LED_off_all();
          LED_on(7);
          transmit_command(0x04, 0x00); 
          }
           else if(ADCResult_Y < CalValue_Y - 60 && ADCResult_X > CalValue_X - 60 && ADCResult_X < CalValue_X + 60)
          {
          LED_off_all();
          LED_on(2);
          transmit_command(0x04, 0x03); 
          }
          else if (ADCResult_Y > CalValue_Y + 60 && ADCResult_X > CalValue_X - 60 && ADCResult_X < CalValue_X + 60)
          {
          LED_off_all();
          LED_on(1);
          transmit_command(0x04, 0x02); 
          }
          TA0CTL = TASSEL_1 + MC_0;           //timer off
          TA0CCTL1 = OUTMOD_0;                //avoid constant high
          //__delay_cycles(500000); 
          ADC10IE |= ADC10IE0;
          //ADC10IE &= ~ADC10IE0;
        }
  
  /*
  while(1)
  {
    transmit_command(0x04, 0x00);
  }
   */
}

/*
#pragma vector=TIMER0_A0_VECTOR                 //Timer A0 interrupt
__interrupt void Timer0_A0 ()
{
  TA0CCTL1 = OUTMOD_7;
}
*/