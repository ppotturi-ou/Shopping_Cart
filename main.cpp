#include "MKL25Z4.h"

#define MASK(x) (1UL << x)
#define RED_LED (18)    //B18

//void init_led(); testing
void init_pit();
int sleep_flag = 0;
int main()
{
 //   init_led();
    init_pit();

    while(1){}
}

void init_pit()
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;  //Enable clock to PIT module
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;    //Enable module, freeze timers in debug mode
       
    /*
    The PIT uses the Bus clock which has a frequency that is half of the System Clock’s frequency. 
    The system clock has a frequency of 20971520 Hz, 
    hence the frequency of the PIT bus clock is 20971520 * 0.5 = 10485760 Hz
    
    ///////////Calculating the Load Value///////////////////////////////////
    The Load Value is the value that the PIT counts down from.
    Assuming you want an interrupt every T seconds, the LoadValue V can be calculated using this formula:
    V = T * 10485760–1
    */
    
    //Based on above calculations, Initialize PIT0 to count down from starting_value
    PIT->CHANNEL[0].LDVAL =0x12BFFFFF;   //every 30s 
    
    
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

void PIT_IRQHandler()
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

void init_led()
{
        //clock gating
        SIM->SCGC5 |=SIM_SCGC5_PORTB_MASK ; //same as above

        //configure pins as GPIO
        PORTB->PCR[RED_LED ] &= ~PORT_PCR_MUX_MASK; //Clear mux
        PORTB->PCR[RED_LED ] |= PORT_PCR_MUX(1);    //setup to be GPIO

        //set up led to be output
        PTB->PDDR |= MASK(RED_LED);
        PTB->PSOR |=MASK(RED_LED);
}