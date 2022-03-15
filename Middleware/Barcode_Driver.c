/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Barcode file

****************************************************/
#include "Barcode_Driver.h"
static BC_FSM_States Barcode_State = BC_Invalid_State;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Run_Barcode_Driver(void)
{

	switch(Barcode_State)
	{
	
			case BC_FSM1:

			break;
			
			case BC_FSM2:

			break;
			
			case BC_FSM3:

			break;
			
			case BC_FSM4:

			break;
			
			case BC_Invalid_State:
			//default:
			break;			
	
	}
	
}


/** EOF ****/
