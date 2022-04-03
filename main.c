/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Lab#3 Application file

****************************************************/
#include "common.h"
#include "Barcode_Driver.h"
#include "LCD_Driver.h"
#include "WiFi_Driver.h"
#include "Shopping_list_struct.h"
#include "lcd.h"
#include "I2C.h"

static SC_FSM_States Shopping_Cart_App_State = Invalid_State;
static int ListCnt = 0;
static char ItemBarcode[12];
S_shopping_list ShoppingList[5];
static int Main_Menu_LCD = FALSE;
static unsigned int Test_cnt = 0;
static unsigned int ReadData_Server_Res=0;
static char test_chr='\0';

void Run_Application(void);
SC_FSM_States Return_App_FSM_State(void);
void LCD_Main_menu(void);
void Clear_Shopping_list(void);
void Display_Receipt(void);
void Display_Item_info(void);

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
int main(void)
{
	// Init Application FSM
	Shopping_Cart_App_State = Init;

	LCD_Init(4,20);
	LCD_GoToLine(0); 
  LCD_DisplayString(" Welcome to OU Mart ");
	LCD_GoToLine(2); 
  LCD_DisplayString("   Shopping Cart   ");
	
	//ShoppingList[0].ItemId = 0;
	//strcpy(&ShoppingList[0].ItemName[0],"Chips");
	//strcpy(&ShoppingList[0].itemPrice[0],"12.00");
	
	//Display_Item_info();
	while(1)
	{
		Run_WiFi_Driver();
		Run_Barcode_Driver();
		//Run_LCD_Driver();
		Run_Application();
		if(Shopping_Cart_App_State == Scanning)
	  Display_Item_info();
		
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
  int price_dec=0;

	switch(Shopping_Cart_App_State)
	{
	
			case Init:
				if(OK == Get_WiFi_Init_Status())
				{
					Shopping_Cart_App_State = Self_Test;
				}
				else
				{
					
				}
			break;
			
			case Self_Test:
				if(Main_Menu_LCD != TRUE)
				{
					LCD_Clear();	
					LCD_GoToLine(0); 
					LCD_DisplayString("Running Self Test...");
					Main_Menu_LCD	= TRUE;
				}
				if(WiFi_SelfTest_Sucess == Get_WiFi_SelfTest_Status())
				{
					Shopping_Cart_App_State = Main_menu;
					LCD_GoToLine(1); 
					LCD_DisplayString("     PASS ");
					Main_Menu_LCD	= FALSE;
					Test_cnt=0 ;
				}
				else if(WiFi_SelfTest_Fail == Get_WiFi_SelfTest_Status())
				{
					LCD_GoToLine(1); 
					LCD_DisplayString("     FAIL ");
					
					LCD_GoToLine(2); 
					LCD_DisplayString("ESP-Failed");	
					Shopping_Cart_App_State = Invalid_State;
				}
				
				
			break;
			
			case Main_menu:
				 //Check WiFi Connection
				if(Main_Menu_LCD != TRUE)
				{
					LCD_Clear();
					LCD_GoToLine(0);
					LCD_DisplayString("Connecting to WiFi");	
					Main_Menu_LCD	= TRUE;	
				}					
			
				if(WiFi_Connection_Sucess == Get_WiFi_SelfTest_Status())
				{
					LCD_GoToLine(1); 
					LCD_DisplayString("Connected!");	
					LCD_GoToLine(2); 
					LCD_DisplayString("M: To Start Shopping");		
					LCD_GoToLine(3); 
					LCD_DisplayString("UP: go to Main Menu");
					Shopping_Cart_App_State = Shopping_menu;
					Main_Menu_LCD	= FALSE;
				}
				else if(WiFi_Connection_Fail == Get_WiFi_SelfTest_Status())
				{
					LCD_GoToLine(1); 
					LCD_DisplayString("Failed!");	
					LCD_GoToLine(2); 
					LCD_DisplayString("Check WiFi and");
					LCD_GoToLine(3); 
					LCD_DisplayString("Restart Board");
					Shopping_Cart_App_State = Invalid_State;
				}
				/*
					
				if(M_KEY == PRESSED)
				{
					//Shopping_Cart_App_State = Shopping_menu;
				}
				else if(UP_KEY == PRESSED)
				{
					Shopping_Cart_App_State = Self_Test;
				}
			  */
				// Todo: replace this in M_KEY Switch Press Testing only Remove this for final Project
				if(Test_cnt > 0x10)
				{
					Shopping_Cart_App_State = Shopping_menu;
					Main_Menu_LCD	= FALSE;
					Test_cnt = 0;
					LCD_Clear();	
					LCD_GoToLine(0); 
					LCD_DisplayString("Connecting to Server");
				}
				else
				{
					Test_cnt++;
				}
				
			break;
			
			case Shopping_menu:
				if(Main_Menu_LCD != TRUE)
				{
					LCD_Clear();	
					LCD_GoToLine(0); 
					LCD_DisplayString("Connecting to Server");	
					Main_Menu_LCD	= TRUE;		
					Test_cnt = 0; // Todo: Testing only Remove this for final Project
					//Clear shopping List
					Clear_Shopping_list();					
				}
				
				if(WiFi_ServerConnection_Sucess == Get_WiFi_SelfTest_Status())
				{
					LCD_GoToLine(0); 
					LCD_DisplayString("Connected to Server ");		
					LCD_GoToLine(2); 
					LCD_DisplayString("M: To Start Shopping");		
					LCD_GoToLine(3); 
					LCD_DisplayString("UP: go to Main Menu");
					Shopping_Cart_App_State = Scanning;
					Main_Menu_LCD	= FALSE;
					
				}
				else if(WiFi_ServerConnection_Fail == Get_WiFi_SelfTest_Status())
				{
					LCD_GoToLine(1); 
					LCD_DisplayString("Failed!");	
					LCD_GoToLine(3); 
					LCD_DisplayString("UP: go to Main Menu");
				}
				// Todo: Testing only Remove this for final Project
				if(Test_cnt > 10)
				{
					Shopping_Cart_App_State = Main_menu;
					Main_Menu_LCD	= FALSE;
					Test_cnt = 0;
				}
				else
				{
					Test_cnt++;
				}
				
				/*
				if(M_KEY == PRESSED)
				{
					//Shopping_Cart_App_State = Scanning;
				}
				*/
			break;

			case Scanning:
				 if(Main_Menu_LCD != TRUE)
				 {
					LCD_Clear();	 
					LCD_DisplayString("Scanning ready...");	
					 Main_Menu_LCD = TRUE;
				 }
				// Check if Barcoder sends New code
				if(BC_New_Data_Ready == TRUE)
				{
					
					// Turn ON Blue LED for successful barcode data
					// Turn OFF RED LED to reset status

					// Clear barcode New Data Status
					BC_New_Data_Ready = FALSE;
					Get_New_BC_Data(&ItemBarcode[0]);
					// Turn ON Yellow LED for successful server request
				  // Query item from server
					ReadData_Server_Res = Get_Item_info(&ItemBarcode[0],&itemname[0],&price[0],&price_dec);	
					LCD_Clear();	
//					LCD_GoToLine(0);
					if(0x10 == ReadData_Server_Res)
					{
						// Turn ON Green LED for successful server response
						//itemno = 1;
						//strcpy(itemname, "Chips");
						//price_dec = atof("1.99");
						//price_dec = (int)1.99;
						if(ListCnt < 5)
						{
							insert(&ShoppingList[ListCnt], ListCnt, itemname, price, price_dec);
							ListCnt++;
						//	Display_Item_info();
							
							//Todo: testing only
							//strcpy(&ItemBarcode[0],"3456789012");
						}
						else
						{
							ListCnt = 0;
							Clear_Shopping_list();
							// Show List full Error on LCD
						}
					}
					else
					{
						//Turn ON RED LED for data not received
					LCD_Clear();	 
					LCD_DisplayString("Scanning Error...");
					Shopping_Cart_App_State = Shopping_Complete;
					}
				}
				else
				{
					//Turn ON RED LED
				}
//				if(M_Key == PRESSED)
//				{
//					//Shopping_Cart_App_State = Shopping_Complete;
//				}

				

			break;
			
			case Shopping_Complete:
				/*
				//Shopping complete go back to Main menu
			  if(M_Key == PRESSED)
				{
					//Shopping_Cart_App_State = Main_menu;
				}
				//Shopping cancelled go back to Main menu
				else if(UP_Key == PRESSED)
				{
					//Shopping_Cart_App_State = Main_menu;
				}*/
			
				// Count Final #items and total Price

				Display_Receipt();
				LCD_GoToLine(2); 
				LCD_DisplayString("M: To Comp Shopping");		
				LCD_GoToLine(3); 
				LCD_DisplayString("UP: To Can Shopping");
				Shopping_Cart_App_State = Invalid_State;

			break;
			
			case Invalid_State:
				Main_Menu_LCD	= FALSE;
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
/**********************************
Function: LCD_Main_menu
Description:  Function to display the menu of operations
Parmeters:
Return:

**********************************/

void LCD_Main_menu(void)
{
	/*
	//LCD line#1
	//LCD line#2
	//LCD line#3
	//LCD line#4
	*/
}
/**********************************
Function: Clear_Shopping_list
Description:
Parmeters:
Return:

**********************************/
void Clear_Shopping_list(void)
{
	int i=0;
	for(i=0;i<5;i++)
	{
		ClearList(&ShoppingList[i]);
	}

}
/**********************************
Function: Clear_Shopping_list
Description:
Parmeters:
Return:

**********************************/
void Display_Item_info(void)
{
	int i=0,LCD_lineCnt=1;
	char str_cat[20],strid[5];
	
	
	for(i=0;i<3;i++)
	{
		
		if(ShoppingList[i].ItemId !=0xFF)
		{
			

			sprintf(str_cat, "%d", (ShoppingList[i].ItemId)+1);
			strcat(str_cat,".");
			strcat(str_cat,&ShoppingList[i].ItemName[0]);
			strcat(str_cat," $");
			strcat(str_cat,&ShoppingList[i].itemPrice[0]);
			
			LCD_DisplayString(str_cat);
			LCD_GoToLine(LCD_lineCnt++);
		}
	}

}
/**********************************
Function: Display_Receipt
Description:
Parmeters:
Return:

**********************************/
void Display_Receipt(void)
{
	int i=0;
	int TotalItems=0, TotalPrice=0;
	
	for(i=0;i<5;i++)
	{
		if(ShoppingList[i].ItemId !=0xFF)
		{
			TotalItems = TotalItems + 1;
			TotalPrice = TotalPrice + ShoppingList[i].itemPrice_dec;
		}
	}
	
	LCD_Clear();
	LCD_DisplayString("Total Items: ");
	LCD_Printf("%2d",TotalItems);
	LCD_GoToLine(1);
  LCD_DisplayString("Total Price: $");
	if((TotalPrice/100)!=0)
	{
		LCD_Printf("%2d",(TotalPrice/100));
	}
	else
	{
		LCD_DisplayString("00");
	}
	LCD_DisplayString(".");
	if((TotalPrice%100)!=0)
	{
		LCD_Printf("%2d",(TotalPrice%100));
	}
	else
	{
		LCD_DisplayString("00");
	}
	

}

//void HardFault_Handler(void)
//{
//	
//	while(1);
//}

/*** EOF  ****/
