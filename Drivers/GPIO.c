/****************************************************
Project: Shopping Cart with Built in billing system
Members: Jonatan Cogiel

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
GPIO driver file

****************************************************/

#include "GPIO.h"
#include "common.h"


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
	
	PORTC->PCR[SW_MENU] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW_MENU] |= PORT_PCR_MUX(1);
	PORTC->PCR[SW_UP] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW_UP] |= PORT_PCR_MUX(1);
	PORTC->PCR[SW_DOWN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[SW_DOWN] |= PORT_PCR_MUX(1);
	
	//LED as output
	PTC->PDDR |= MASK(R_LED)|MASK(G_LED)|MASK(B_LED)|MASK(Y_LED);
	//btn inputs
	PTC->PDDR &= ~(MASK(SW_MENU)|MASK(SW_UP)|MASK(SW_DOWN));
	
	// LED's Self-Test
	LED_ctrl(R_LED, LED_ON);
	LED_ctrl(Y_LED, LED_ON);
	LED_ctrl(G_LED, LED_ON);
	LED_ctrl(B_LED, LED_ON);	
	Delay(100);
	LED_ctrl(R_LED, LED_OFF);
	LED_ctrl(Y_LED, LED_OFF);
	LED_ctrl(G_LED, LED_OFF);
	LED_ctrl(B_LED, LED_OFF);
}

void LED_ctrl(int X_LED, int state)
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


int SW_status(int SW)
{
	switch (SW)
	{
		case SW_MENU:
			if(btn_press(SW_MENU))
			{
				Delay(50);
				if(btn_press(SW_MENU))
				{
					return SW_PRESSED;
				}
			}
			break;
			
		case SW_DOWN:
			if(btn_press(SW_DOWN))
			{
				Delay(50);
				if(btn_press(SW_DOWN))
				{
					return SW_PRESSED;
				}
			}
			break;
			
		case SW_UP:
			if(btn_press(SW_UP))
			{
				Delay(50);
				if(btn_press(SW_UP))
				{
					return SW_PRESSED;
				}
			}
			break;
	}
	
	return SW_RELEASED;
}

void Clear_All_LEDs(void)
{
	LED_ctrl(R_LED, LED_OFF);
	LED_ctrl(Y_LED, LED_OFF);
	LED_ctrl(G_LED, LED_OFF);
	LED_ctrl(B_LED, LED_OFF);

}




