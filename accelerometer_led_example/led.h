/*
LEDs 1 to 4 are connected to PJ.0, PJ.1, PJ.2 and PJ.3.
LEDs 5 to 8 are connected to P3.4, P3.5, P3.6 and P3.7.

Pins of Port 3 are also available as physical headers. So, working can also be verified by measuring voltage.

The LEDonboardInit function has 3 modes:
1) Using LEDs of only PJ.
2) using LEDs of only P3.
3) Using LEDs of both PJ and P3.

*** Usage ***
LED_on(number);
LED_off(number);
*/

void LEDonboardInit(int x)
{
  if(x==1)
  {
    PJDIR |= BIT0 + BIT1 + BIT2 + BIT3; //LEDs 1 to 4 = PJ0to3. Set as output pins.
    PJOUT &= ~(BIT0 + BIT1 + BIT2 + BIT3); //switch all off
  }
  else if(x==2)
  {
    P3DIR |= BIT4 + BIT5 + BIT6 + BIT7; //LEDs 5 to 8 = PJ3.4to3.7 Set as output pins.
    P3OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7); //switch all off
  }
  else
  {
    PJDIR |= BIT0 + BIT1 + BIT2 + BIT3;
    P3DIR |= BIT4 + BIT5 + BIT6 + BIT7;
    PJOUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    P3OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
  }
}

void LED_off(int x)
{
  switch(x)
  {
  case 1: PJOUT &= ~BIT0;
          break;
  case 2: PJOUT &= ~BIT1;
          break;
  case 3: PJOUT &= ~BIT2;
          break;
  case 4: PJOUT &= ~BIT3;
          break;
  case 5: P3OUT &= ~BIT4;
          break;
  case 6: P3OUT &= ~BIT5;
          break;
  case 7: P3OUT &= ~BIT6;
          break;
  case 8: P3OUT &= ~BIT7;
          break;
  default: break;
  }
}

void LED_off_all()
{
  PJOUT &= ~BIT0; PJOUT &= ~BIT1; PJOUT &= ~BIT2; PJOUT &= ~BIT3;
  P3OUT &= ~BIT4; P3OUT &= ~BIT5; P3OUT &= ~BIT6; P3OUT &= ~BIT7;
}

void LED_on(int x)
{
  switch(x)
  {
  case 1: PJOUT |= BIT0;
          break;
  case 2: PJOUT |= BIT1;
          break;
  case 3: PJOUT |= BIT2;
          break;
  case 4: PJOUT |= BIT3;
          break;
  case 5: P3OUT |= BIT4;
          break;
  case 6: P3OUT |= BIT5;
          break;
  case 7: P3OUT |= BIT6;
          break;
  case 8: P3OUT |= BIT7;
          break;
  default: break;
  }
}