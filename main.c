/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Lab#3 Application file

****************************************************/
#include "common.h"
#include "Barcode_Driver.h"
#include "LCD_Driver.h"
#include "WiFi_Driver.h"

static SC_FSM_States Shopping_Cart_App_State = Invalid_State;

void Run_Application(void);
SC_FSM_States Return_App_FSM_State(void);

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
int main(void)
{
	while(1)
	{
		Run_WiFi_Driver();
		Run_Barcode_Driver();
		Run_LCD_Driver();
		Run_Application();	
		
	}
}

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Run_Application(void)
{

	switch(Shopping_Cart_App_State)
	{
	
			case Init:

			break;
			
			case Self_Test:

			break;
			
			case Main_menu:

			break;
			
			case Shopping_menu:

			break;

			case Scanning:

			break;
			
			case Shopping_Complete:

			break;
			
			case Invalid_State:
			//default:
			break;			
	
	}
}
/**********************************************
Function: Return_App_FSM_State
Description: Return application FSM state
for other driver to know if needed
Parmeters: None
Return: SC_FSM_States

***********************************************/
SC_FSM_States Return_App_FSM_State(void)
{
	return Shopping_Cart_App_State;

}

/*** EOF  ****/
