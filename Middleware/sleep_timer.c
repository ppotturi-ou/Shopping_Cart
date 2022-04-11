/****************************************************
Project: Shopping Cart with Built in billing system
Members: anahid@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Sleep Timer Driver file

****************************************************/
//#include "MKL25Z4.h"
#include "sleep_timer.h"

#define RED_LED (18)    //B18
//DigitalIn timer_stop(D3);
//DigitalOut LED_G(PTB19);//declaring the onboard RGB led as pedLED Red (PTB19)


static int sleep_flag = 0;


void Start_timer(void)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;  //Enable clock to PIT module
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;    //Enable module, freeze timers in debug mode
       
    /*
    The PIT uses the Bus clock which has a frequency that is half of the System Clock?s frequency. 
    The system clock has a frequency of 20971520 Hz, 
    hence the frequency of the PIT bus clock is 20971520 * 0.5 = 10485760 Hz
    
    ///////////Calculating the Load Value///////////////////////////////////
    The Load Value is the value that the PIT counts down from.
    Assuming you want an interrupt every T seconds, the LoadValue V can be calculated using this formula:
    V = T * 10485760?1
    */
    
    //Based on above calculations, Initialize PIT0 to count down from starting_value
    PIT->CHANNEL[0].LDVAL =0x12BFFFFF;   //every 15s 
		//PIT->CHANNEL[0].LDVAL =0x257FFFFE;   //every 30s
    
    
    PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;  //No chaining of timers
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  //Enable channel timer
    
    //Let the PIT channel generate interrupt requests so enable interrupts from the PIT
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
    
       
    NVIC_SetPriority(PIT_IRQn, 3); 
    //Clear any pending IRQ from PIT
    NVIC_ClearPendingIRQ(PIT_IRQn); 
    //Enable the PIT interrupt in the NVIC
    NVIC_EnableIRQ(PIT_IRQn);   
}


void PIT_IRQHandler(void)
{
    //Check channel 0 if triggered interrupt
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 
    {     
        //PTB->PTOR |=MASK(RED_LED);  //toggle RED
        sleep_flag = 1;
        //Clear interrupt request flag for channel
        PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
			
			
    }
}

int get_sleep_status(void)
{
	return sleep_flag;
}

void Stop_timer(void)
{
	PIT->MCR |= PIT_MCR_MDIS_MASK;  //Disable module
	sleep_flag = 0;
	//SIM->SCGC6 &= ~SIM_SCGC6_PIT_MASK; //Disable clock to PIT module
}

