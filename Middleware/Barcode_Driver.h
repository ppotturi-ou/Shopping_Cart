/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Barcode header file

****************************************************/
#ifndef _BARCODE_DRIVER_H
#define _BARCODE_DRIVER_H

#include "common.h"
#include "Barcode_UART.h"

typedef enum  
{
	BC_FSM1 = 0,
	BC_FSM2 = 1,
  BC_FSM3 = 2,
	BC_FSM4 = 3,
	BC_Invalid_State = 4,
}BC_FSM_States;

void Run_Barcode_Driver(void);

#endif //_BARCODE_DRIVER_H



/** EOF ****/
