/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
WiFi UART header file

****************************************************/
#ifndef _WIFI_UART_H
#define _WIFI_UART_H

#include "common.h"
#include "queue.h"
#include <stdint.h>
//#include "queue.h"

#define USE_UART_INTERRUPTS 	(1) // 0 for polled UART communications, 1 for interrupt-driven
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)

//extern void UART0_IRQHandler(void);
void Init_UART2(uint32_t baud_rate);
void UART2_Transmit_Poll(uint8_t data);
uint8_t UART2_Receive_Poll(void);

void Send_String_Poll(uint8_t * str);
void Send_String(uint8_t * str);

uint32_t Rx_Chars_Available(void);
uint8_t	Get_Rx_Char(void);

extern Q_T WiFi_UART_TxQ, WiFi_UART_RxQ;


#endif //_WIFI_UART_H


/** EOF ****/

