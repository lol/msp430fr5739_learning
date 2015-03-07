/*
	AccelInit(); //Setup the accelerometer pins.
	SetupAccel();	//Setup the ADC and Accel
	CalibrateADC(); //Find zero points of all 3 axis

*/

#define ADC_START_ADD 0xD400
#define ADC_END_ADD 0xF000

// Pin Definitions
#define ACC_PWR_PIN       BIT7
#define ACC_PWR_PORT_DIR  P2DIR
#define ACC_PWR_PORT_OUT  P2OUT
#define ACC_PORT_DIR      P3DIR
#define ACC_PORT_OUT      P3OUT
#define ACC_PORT_SEL0     P3SEL0
#define ACC_PORT_SEL1     P3SEL1
#define ACC_X_PIN         BIT0
#define ACC_Y_PIN         BIT1
#define ACC_Z_PIN         BIT2

// Accelerometer Input Channel Definitions
#define ACC_X_CHANNEL     ADC10INCH_12
#define ACC_Y_CHANNEL     ADC10INCH_13
#define ACC_Z_CHANNEL     ADC10INCH_14

unsigned int ADC_counter = 0;
unsigned int ADCResult_X, ADCResult_Y, ADCResult_Z;
unsigned int CalValue_X, CalValue_Y, CalValue_Z;

void AccelInit()
{
  	// P3.0,P3.1 and P3.2 are accelerometer inputs
  	P3OUT &= ~(BIT0 + BIT1 + BIT2);   
  	P3DIR &= ~(BIT0 + BIT1 + BIT2); 
  	P3REN |= BIT0 + BIT1 + BIT2;
}

void CalibrateADC(void) //Sets the "zero" point of the accelerometer
{
	unsigned char CalibCounter =0;
	unsigned int Value = 0;
	
    __disable_interrupt();	//Turn off any interupts just incase
    
    ADC10CTL0 &= ~ADC10ENC; //Toggle ENC bit. Need this to change the ADC10INCH_x value.
    ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_12;	//Sample the X-axis  
	while(CalibCounter <50)
	{
		P3OUT ^= BIT4;
		CalibCounter++;
		ADC10CTL0 |= ADC10ENC | ADC10SC;	//Start sample.
		while (ADC10CTL1 & BUSY);	//Wait for sample to be done.
		Value += ADC10MEM0;
	}
	CalValue_X = Value/50;	//Average all samples to find the best zero.
	
	ADC10CTL0 &= ~ADC10ENC;	//Toggle ENC bit. Need this to change the ADC10INCH_x value.
	CalibCounter = 0;	//Reset the counters
	Value = 0;
	ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_13;	//Sample the Y-axis 
	while(CalibCounter <50)
	{
		P3OUT ^= BIT4;
		CalibCounter++;
		ADC10CTL0 |= ADC10ENC | ADC10SC;	//Start sample.
		while (ADC10CTL1 & BUSY);	//Wait for sample to be done.
		Value += ADC10MEM0;
	}
	CalValue_Y = Value/50;	//Average all samples to find the best zero.
	
	ADC10CTL0 &= ~ADC10ENC;	//Toggle ENC bit. Need this to change the ADC10INCH_x value.
	CalibCounter = 0;	//Reset the counters
	Value = 0;
	ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_14;	//Sampel the Z-axis 
	while(CalibCounter <50)
	{
		P3OUT ^= BIT4;
		CalibCounter++;
		ADC10CTL0 |= ADC10ENC | ADC10SC;	//Start sample.
		while (ADC10CTL1 & BUSY);  	//Wait for sample to be done.
		Value += ADC10MEM0;
	}
	CalValue_Z = Value/50;	//Average all samples to find the best zero.
	
	ADC10CTL0 &= ~ADC10ENC;	//Toggle ENC bit. Need this to change the ADC10INCH_x value.
	ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_12;	//We need to start at the X-axis first due to how the interupt routine works.
	
	__enable_interrupt();	//enable interupts
}

void SetupAccel(void)
{  
	//Setup  accelerometer
	// ~20KHz sampling
	//Configure GPIO
	ACC_PORT_SEL0 |= ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN;    //Enable A/D channel inputs
	ACC_PORT_SEL1 |= ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN;
  	ACC_PORT_DIR &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN);
  	ACC_PWR_PORT_DIR |= ACC_PWR_PIN;              //Enable ACC_POWER
  	ACC_PWR_PORT_OUT |= ACC_PWR_PIN;

  	// Allow the accelerometer to settle before sampling any data 
  	__delay_cycles(200000);   
  
  	//Setting up the ADC stuff 
  	ADC10CTL0 &= ~ADC10ENC;                        // Ensure ENC is clear
  	ADC10CTL0 = ADC10ON + ADC10SHT_5;  
  	ADC10CTL1 = ADC10SHS_0 + ADC10SHP + ADC10CONSEQ_0 + ADC10SSEL_0;  
  	ADC10CTL2 = ADC10RES;    
  	ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_12;
  	ADC10IV = 0x00;		//Clear all ADC12 channel int flags  
  	ADC10IE |= ADC10IE0;	//Enable ADC10 interrupts
  	
  	__enable_interrupt();
  	
}


//ADC10 interupt routine
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{   
	if (ADC_counter == 0) //X-axis
	{
		ADC10CTL0 &= ~ADC10ENC;	
		ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_13;	//Next channel is the Y-axis
		ADCResult_X = ADC10MEM0;
		ADC_counter++;
		ADC10CTL0 |= ADC10ENC | ADC10SC;
	}
	else if (ADC_counter == 1)	//Y-axis
	{
		ADC10CTL0 &= ~ADC10ENC;
		ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_14;	//Next channel is the Z-axis
		ADCResult_Y = ADC10MEM0;
		ADC_counter++;
		ADC10CTL0 |= ADC10ENC | ADC10SC;
	}
	else	//Z-axis
	{
		ADC10CTL0 &= ~ADC10ENC;
		ADC10MCTL0 = ADC10SREF_0 + ADC10INCH_12;	//Next channel is the X-axis
		ADCResult_Z = ADC10MEM0;
		ADC_counter = 0;
		ADC10CTL0 |= ADC10ENC | ADC10SC;
	}
}