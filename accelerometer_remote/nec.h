/*
#include <msp430fr5739.h>
#include <stdint.h>
#include "led.h"
*/


#define LOGIC_0                                 0
#define LOGIC_0_ON_TIME                         4450     //562.5us ON
#define LOGIC_0_OFF_TIME                        4450     //562.5us OFF

#define LOGIC_1                                 1
#define LOGIC_1_ON_TIME                         4450     //562.5us ON  
#define LOGIC_1_OFF_TIME                        13350    //1687.5us OFF

#define START_WAVE                              2
#define START_WAVE_ON_TIME                      71200    //leading 9ms pulse
#define START_WAVE_OFF_TIME                     35600    //4.5ms gap after the 9ms pulse

#define END_BIT                                 3
#define END_BIT_ON_TIME                         4450
#define END_BIT_OFF_TIME                        37412



void transmit_command(uint8_t, uint8_t);
void transmit_pulse(uint8_t);

void transmit_pulse(uint8_t pulse_type)
{
         

  switch(pulse_type)
   {
    case LOGIC_0:
            TA0CTL |= MC_1;                     //timer on
            __delay_cycles(LOGIC_0_ON_TIME);
            TA0CTL = TASSEL_1 + MC_0;           //timer off
            TA0CCTL1 = OUTMOD_0;                //avoid constant high
            __delay_cycles(LOGIC_0_ON_TIME);
            break;
    case LOGIC_1:
            TA0CTL |= MC_1;                     //timer on
            __delay_cycles(LOGIC_1_ON_TIME);
            TA0CTL = TASSEL_1 + MC_0;           //timer off
            TA0CCTL1 = OUTMOD_0;                ///avoid constant high
            __delay_cycles(LOGIC_1_OFF_TIME);			
            break;
    case END_BIT:
            TA0CTL |= MC_1;                     //timer on
            __delay_cycles(END_BIT_ON_TIME);
            TA0CTL = TASSEL_1 + MC_0;           //timer off
            TA0CCTL1 = OUTMOD_0;                //avoid constant high
            __delay_cycles(END_BIT_OFF_TIME);	
            break;
    case START_WAVE:
            TA0CTL |= MC_1;                     //timer on
            __delay_cycles(START_WAVE_ON_TIME);
            TA0CTL = TASSEL_1 + MC_0;           //timer off
            TA0CCTL1 = OUTMOD_0;                //avoid constant high
            __delay_cycles(START_WAVE_OFF_TIME);
            break;
   }
  

}


void transmit_command(uint8_t address, uint8_t command)
{
  uint8_t i, j, value;
  uint8_t frame[4] = {address, ~address, command, ~command};

  transmit_pulse(START_WAVE);
	
  for( i = 0; i < 4; i++ )
  {
    value = frame[i];
    for( j = 0; j < 8; j++ )
    {
      if( value & (1<<j) )
      {
        transmit_pulse(LOGIC_1);
      }
      else 
      {
        transmit_pulse(LOGIC_0);
      }
    }
  }
  transmit_pulse(END_BIT);
  TA0CCTL1 = OUTMOD_0;
  __delay_cycles(500000); 
}

/*
void main()
{
  WDTCTL = WDTPW + WDTHOLD;
  CSCTL0_H = 0xA5;
  LEDonboardInit(0);
  
  
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
  
  __enable_interrupt();
  
  while(1)
  {
    transmit_command(0x04, 0x00);
  }
}
*/


#pragma vector=TIMER0_A0_VECTOR                 //Timer A0 interrupt
__interrupt void Timer0_A0 ()
{
  TA0CCTL1 = OUTMOD_7;
}