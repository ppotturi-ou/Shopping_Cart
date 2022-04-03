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

#define BARCODE_RX_SIZE 14
typedef enum  
{
	BC_Init = 0,
	BC_ReadCode = 1,
  BC_FSM3 = 2,
	BC_FSM4 = 3,
	BC_Invalid_State = 4,
}BC_FSM_States;

void Run_Barcode_Driver(void);
void Set_BC_Available_Flag(int BC_Flag);
int Get_BC_Available_Flag(void);
void Get_New_BC_Data(char* BC_Dest);

extern int BC_New_Data_Ready;

#endif //_BARCODE_DRIVER_H



/** EOF ****/
