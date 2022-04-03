#include "GPIO.h"


void Init_GPIO(void)
{
	SIM->SCGC5 |=SIM_SCGC5_PORTC_MASK;
	
	//Make GPIO
	PORTC->PCR[R_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[R_LED] |= PORT_PCR_MUX(1);
	PORTC->PCR[G_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[G_LED] |= PORT_PCR_MUX(1);
	PORTC->PCR[B_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[B_LED] |= PORT_PCR_MUX(1);
	PORTC->PCR[Y_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[Y_LED] |= PORT_PCR_MUX(1);
	
	PORTC->PCR[SW1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW1] |= PORT_PCR_MUX(1);
	PORTC->PCR[SW2] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW2] |= PORT_PCR_MUX(1);
	PORTC->PCR[SW3] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW3] |= PORT_PCR_MUX(1);
	
	//LED as output
	PTC->PDDR |= MASK(R_LED)|MASK(G_LED)|MASK(B_LED)|MASK(Y_LED);
	//btn inputs
	PTC->PDDR &= ~(MASK(SW1)|MASK(SW2)|MASK(SW3));
	
	led(R_LED, on);
	led(Y_LED, on);
	led(G_LED, on);
	led(B_LED, on);	
	Delay(100);
	led(R_LED, off);
	led(Y_LED, off);
	led(G_LED, off);
	led(B_LED, off);
}

void led(int X_LED, int state)
{

		switch (state){
			case 1:
				PTC->PCOR = MASK(X_LED);
			  break;
			case 2:
				PTC->PSOR = MASK(X_LED);
				break;
		}
}

void btn_debounce(void)
{
	static enum{ST_NO_SW,ST_SW1,ST_SW2,ST_SW3} state;
	switch (state){
		case ST_NO_SW:
			if(btn_press(SW1))
			{
				state = ST_SW1;
			}
			else if(btn_press(SW2))
			{
				state = ST_SW2;
			}
			else if(btn_press(SW3))
			{
				state = ST_SW3;
			}
			else
			{
				state = ST_NO_SW;
			}
		break;
		case ST_SW1:
			if(btn_press(SW1))
			{
				state = ST_SW1;
				//led(R_LED, on); //for testing
				Delay(50);
				//led(R_LED, off); //for testing
			}
			else
			{
				state = ST_NO_SW;
			}
		break;
		case ST_SW2:
			if(btn_press(SW2))
			{
				state = ST_SW2;
				//led(Y_LED, on); //for testing
				Delay(50);
				//led(Y_LED, off); //for testing
			}
			else
			{
				state = ST_NO_SW;
			}
		break;
		case ST_SW3:
			if(btn_press(SW3))
			{
				state = ST_SW3;
				//led(B_LED, on); //for testing
				Delay(50);
				//led(B_LED, off); //for testing
			}
			else
			{
				state = ST_NO_SW;
			}			
		break;
	}
}

//void Delay (uint32_t dly) {
//  volatile uint32_t t;

//	for (t=dly*10000; t>0; t--)
//		;
//}






