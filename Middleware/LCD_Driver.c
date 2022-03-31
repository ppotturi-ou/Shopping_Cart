/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
LCD Driver file

****************************************************/
#include "LCD_Driver.h"
static LCD_FSM_States LCD_State = LCD_Invalid_State;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Run_LCD_Driver(void)
{

	switch(LCD_State)
	{
	
			case LCD_FSM1:

			break;
			
			case LCD_FSM2:

			break;
			
			case LCD_FSM3:

			break;
			
			case LCD_FSM4:

			break;
			
			case LCD_Invalid_State:
			//default:
			break;			
	
	}
	
}



/** EOF ****/
