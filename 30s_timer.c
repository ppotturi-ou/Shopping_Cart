// INCLUDE
#include <stdint.h>
#include <MKL25Z4.h>
#include "GPIO.h"
int sleep_flag = 0;

void sleep_timer()
{
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;  // Make sure clock is enabled
    LPTMR0->CSR = 0;                     // Reset LPTMR settings
    LPTMR0->CMR = 30000;             // Set compare value (in ms)
		

    // Use 1kHz LPO with no prescaler
    LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    // Start the timer and wait for it to reach the compare value
    LPTMR0->CSR = LPTMR_CSR_TEN_MASK;
    while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK))  //as long as the counter doesnt match the compare keep counting
		{
			if(btn_press(SW1)|(btn_press(SW2)|(btn_press(SW3))		//if any buttons are pressed
			{
				LPTMR0->CSR = !LPTMR_CSR_TEN_MASK;		//disable timer
				LPTMR0->CSR = LPTMR_CSR_TEN_MASK;			//enable timer (reset timer)
			}	
			
			if(LPTMR0->CSR = LPTMR_CSR_TCF_MASK)//if TCF flag is set, 30 seconds is reached
			{
				sleep_flag = 1;			//raise sleep flag to indicate sleep mode should occur
			}				
		}        
}

void wake_up()
{
	if(btn_press(SW1)|(btn_press(SW2)|(btn_press(SW3))
	{
		sleep_flag = 0;
	}
}