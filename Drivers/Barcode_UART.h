/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
WiFi UART header file

****************************************************/
#ifndef _BARCODE_UART_H
#define _BARCODE_UART_H

#include "common.h"

#include <stdint.h>
#include <MKL25Z4.H>
#include "queue.h"

#define USE_UART_INTERRUPTS 	(1) // 0 for polled UART communications, 1 for interrupt-driven
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6) //9446
#define SYS_CLOCK							(48e6) //18662

void Init_UART0(uint32_t baud_rate);
void UART0_Transmit_Poll(uint8_t data);
uint8_t UART0_Receive_Poll(void);

//void Send_String_Poll(uint8_t * str);
//void Send_String(uint8_t * str);

uint32_t UART0_Rx_Chars_Available(void);
uint8_t	UART0_Get_Rx_Char(void);

extern void Set_BC_Available_Flag(int BC_Flag);
extern int Get_BC_Available_Flag(void);
extern Q_T BC_TxQ, BC_RxQ;

#endif //_BARCODE_UART_H


/** EOF ****/
