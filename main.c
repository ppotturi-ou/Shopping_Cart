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
#include "Shopping_list.h"

static SC_FSM_States Shopping_Cart_App_State = Invalid_State;

void Run_Application(void);
SC_FSM_States Return_App_FSM_State(void);
void LCD_Main_menu(void);

static struct shopping_list *ShoppingList;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
int main(void)
{
	Shopping_Cart_App_State = Scanning;
	while(1)
	{
		//Run_WiFi_Driver();
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
	char itemname[10], price[7];
  float price_dec=0.0;
  int itemno=0;
  double test=0;
	switch(Shopping_Cart_App_State)
	{
	
			case Init:
				if(OK == Get_WiFi_Init_Status())
				{
					Shopping_Cart_App_State = Self_Test;
				}
			break;
			
			case Self_Test:
				if(OK == Get_WiFi_SelfTest_Status())
				{
					Shopping_Cart_App_State = Self_Test;
				}
			break;
			
			case Main_menu:
				// Check WiFi Connection
				
			ShoppingList = NULL;
			break;
			
			case Shopping_menu:

			break;

			case Scanning:
				/*
				
				while(M_Key != PRESSED)
				{
					// Check if Barcoder sends New code
					if(Barcode_New_Data == AVAILABLE)
						{
							// Clear barcode New Data Status
							// Query item from server
							Send_Data_to_Server(*str);	
						
						}
				}
				
				itemno = 1;
				strcpy(itemname, "Chips");
				price_dec = atof("1.99");
			  price_dec = (float)1.99;

				//ShoppingList = insert(ShoppingList, itemno, itemname, price, price_dec);
			*/
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

/*  Function to display the menu of operations */
void LCD_Main_menu(void)
{
	/*
	//LCD line#1
	//LCD line#2
	//LCD line#3
	//LCD line#4
	*/
}
/*  End of menu() */

/*** EOF  ****/
