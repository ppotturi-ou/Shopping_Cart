/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
WiFi UART file

****************************************************/
#include "WiFi_UART.h"
#include <stdio.h>

/**** Global Variable ****/
Q_T WiFi_UART_TxQ, WiFi_UART_RxQ;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
//struct __FILE
//{
 // int handle;
//};

FILE __stdout;  //Use with printf
FILE __stdin;		//use with fget/sscanf, or scanf


//Retarget the fputc method to use the UART2
int fputc(int ch, FILE *f){
	while(!(UART2->S1 & UART_S1_TDRE_MASK) && !(UART2->S1 & UART_S1_TC_MASK));
	UART2->D = ch;
	return ch;
}

//Retarget the fgetc method to use the UART2
int fgetc(FILE *f){
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	return UART2->D;
}

// Code listing 8.8, p. 231
void Init_UART2(uint32_t baud_rate) {
	uint16_t sbr;
	uint8_t temp;
	
	// Enable clock gating for UART2 and Port E
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK; 										
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	
	
	// Make sure transmitter and receiver are disabled before init
	UART2->C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK; 		
	
	// Set UART clock to 48 MHz clock 
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART2 Rx and Tx
	PORTE->PCR[23] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(4); // Rx
	PORTE->PCR[22] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(4); // Tx
	
	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((BUS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE)); 			
	UART2->BDH &= ~UART_BDH_SBR_MASK;
	UART2->BDH |= UART_BDH_SBR(sbr>>8);
	UART2->BDL = UART_BDL_SBR(sbr);
	//UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);				

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART2->BDH |= UART_BDH_RXEDGIE(0) | UART_BDH_SBNS(0) | UART_BDH_LBKDIE(0);
	
	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART2->C1 = UART_C1_LOOPS(0) | UART_C1_M(0) | UART_C1_PE(0); 
	// Don't invert transmit data, don't enable interrupts for errors
	UART2->C3 = UART_C3_TXINV(0) | UART_C3_ORIE(0)| UART_C3_NEIE(0) 
			| UART_C3_FEIE(0) | UART_C3_PEIE(0);

	// Clear error flags
//	UART2->S1 = UART_S1_OR(1) | UART_S1_NF(1) | UART_S1_FE(1) | UART_S1_PF(1);

//	// Try it a different way
//	UART2->S1 |= UART2_S1_OR_MASK | UART2_S1_NF_MASK | 
//									UART2_S1_FE_MASK | UART2_S1_PF_MASK;
	
	// Send LSB first, do not invert received data
	//UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0); 
	UART2->S2 = UART_S2_RXINV(0);
	
#if USE_UART_INTERRUPTS
	// Enable interrupts. Listing 8.11 on p. 234
	Q_Init(&WiFi_UART_TxQ);
	Q_Init(&WiFi_UART_RxQ);

	NVIC_SetPriority(UART2_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART2_IRQn); 
	NVIC_EnableIRQ(UART2_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART2->C2 |= UART_C2_RIE(1);
#endif

	// Enable UART receiver and transmitter
	UART2->C2 |= UART_C2_RE(1) | UART_C2_TE(1);	
	
	// Clear the UART RDRF flag
	temp = UART2->D;
	//UART2->S1 &= ~UART_S1_RDRF_MASK;
	Send_String((uint8_t *)"\n");
}

/* END - UART2 Device Driver 
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/

	Modified by Alex Dean 9/13/2016
*/

// Code listing 8.9, p. 233
void UART2_Transmit_Poll(uint8_t data) {
		while (!(UART2->S1 & UART_S1_TDRE_MASK))
			;
		UART2->D = data;
}	

uint8_t UART2_Receive_Poll(void) {
		while (!(UART2->S1 & UART_S1_RDRF_MASK))
			;
		return UART2->D;
}	

// UART2 IRQ Handler. Listing 8.12 on p. 235
void UART2_IRQHandler(void) {
	uint8_t ch;
	
	/*if (UART2->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | 
		UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// clear the error flags
			UART2->S1 |= UART2_S1_OR_MASK | UART2_S1_NF_MASK | 
									UART2_S1_FE_MASK | UART2_S1_PF_MASK;	
			// read the data register to clear RDRF
			ch = UART2->D;
	}*/
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		// received a character
		ch = UART2->D;
		if (!Q_Full(&WiFi_UART_RxQ)) {
			Q_Enqueue(&WiFi_UART_RxQ, ch);
		} else {
			// error - queue full.
			// discard character
		}
	}
	if ( (UART2->C2 & UART_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART2->S1 & UART_S1_TDRE_MASK) ) { // tx buffer empty
		// can send another character
		if (!Q_Empty(&WiFi_UART_TxQ)) {
			UART2->D = Q_Dequeue(&WiFi_UART_TxQ);
		} else {
			// queue is empty so disable transmitter interrupt
			UART2->C2 &= ~UART_C2_TIE_MASK;
		}
	}
}

void Send_String_Poll(uint8_t * str) {
	// enqueue string
	while (*str != '\0') { // Send characters up to null terminator
		UART2_Transmit_Poll(*str++);
	}
}

void Send_String(uint8_t * str) {
	// enqueue string
	while (*str != '\0') { // copy characters up to null terminator
		while (Q_Full(&WiFi_UART_TxQ))
			; // wait for space to open up
		Q_Enqueue(&WiFi_UART_TxQ, *str);
		str++;
	}
	// start transmitter if it isn't already running
	if (!(UART2->C2 & UART_C2_TIE_MASK)) {
		UART2->D = Q_Dequeue(&WiFi_UART_TxQ); 
		UART2->C2 |= UART_C2_TIE(1);
	}
}


uint32_t Rx_Chars_Available(void) {
	return Q_Size(&WiFi_UART_RxQ);
}

uint8_t	Get_Rx_Char(void) {
	return Q_Dequeue(&WiFi_UART_RxQ);
}



/** EOF ****/
