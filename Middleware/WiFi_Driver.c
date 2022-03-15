/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
ESP WiFi Driver file

****************************************************/
#include "WiFi_Driver.h"

static WiFi_FSM_States Barcode_State = WiFi_Invalid_State;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Run_WiFi_Driver(void)
{

	switch(Barcode_State)
	{
	
			case WiFi_FSM1:

			break;
			
			case WiFi_FSM2:

			break;
			
			case WiFi_FSM3:

			break;
			
			case WiFi_FSM4:

			break;
			
			case WiFi_Invalid_State:
			//default:
			break;			
	
	}
	
}

/** EOF ****/

