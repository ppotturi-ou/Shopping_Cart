/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
ESP WiFi Driver file

****************************************************/
#include "WiFi_Driver.h"
#include <string.h>
#include "lcd.h"

static WiFi_FSM_States WiFi_State = WiFi_Init;
static int WiFi_Init_State = ERROR;
static int WiFi_Status = ERROR;

/**********************************
Function: Init_WiFi_Driver
Description:
Parmeters:
Return:

**********************************/
void Init_WiFi_Driver(void)
{
	Init_UART2(115200);

}

/**********************************
Function: Run_WiFi_Driver
Description:
Parmeters:
Return:

**********************************/
void Run_WiFi_Driver(void)
{
	int res = ERROR;
	static uint16_t FailCnt=0;
	switch(WiFi_State)
	{
			case WiFi_Init:
				Init_WiFi_Driver();
				Send_String((uint8_t *)"AT+RST\r\n");
				Delay(1000);
				while (WiFi_UART_RxQ.Size != 0)
				{ 
					// Clear buffer after reset
					Q_Dequeue(&WiFi_UART_RxQ);
				}
				WiFi_Init_State = OK;
				WiFi_State = WiFi_SelfTest;
			break;
			
			case WiFi_SelfTest:
				if(Return_App_FSM_State() == Self_Test)
				{
					Send_String((uint8_t *)"AT\r\n");
					Delay(5);
					res=read_data((uint8_t *)"OK");
					if(res == WiFi_RES_SUCCESS)
					{
						// Reset Fail Count
						FailCnt=0;
						// Set the status to let application know
						WiFi_Status = WiFi_SelfTest_Sucess;
						
						// Goto next state
						WiFi_State = WiFi_Connect;
					}
					else if(res == WiFi_RES_FAIL)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 0xFF)
						{
							//Log Error, like LED RED
							// Turn ON LED RED
							// Display on LCD
							WiFi_Status = WiFi_SelfTest_Fail;
						}
					}
					else if(res == WiFi_RES_ERROR)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 0xFF)
						{
							//Log Error, like LED RED
							// Turn ON LED RED
							// Display on LCD		
							WiFi_Status = WiFi_SelfTest_Fail;						
						}
					}
				}
			break;
			
			case WiFi_Connect:
				if(Return_App_FSM_State() == Main_menu)
				{
					LCD_Clear();
					LCD_GoToLine(0);
					LCD_DisplayString("Connecting to WiFi");
					Send_String("AT+CWJAP=""\"""MySpectrumWiFi78-2G""\""",""\"""Kprexy2013""\"""\r\n");
					//Send_String("AT+CWJAP=""\"""Pradeep's iPhone""\""",""\"""Dimboo123$""\"""\r\n");
					Delay(2000);
					res=read_data("WIFI CONNECTED"); 
					if(res == WiFi_RES_SUCCESS)
					{
						// Reset Fail Count
						FailCnt=0;
						WiFi_Status = WiFi_Connection_Sucess;
						WiFi_State = WiFi_Connect_Server;
					}
					else if(res == WiFi_RES_FAIL)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log WiFi connection Error, 
							// Turn ON LED RED
							// Display on LCD
							WiFi_Status = WiFi_Connection_Fail;
							//WiFi_State = WiFi_ReConnect;
							WiFi_State = WiFi_Invalid_State;
						}
					}
					else if(res == WiFi_RES_ERROR)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log Error, like LED RED
							//WiFi_State = WiFi_ReConnect;
							WiFi_Status = WiFi_Connection_Fail;
							WiFi_State = WiFi_Invalid_State;
						}
					}
					else
					{
						Delay(4000);
					}
				}
			break;

			case WiFi_Connect_Server:
				if(Return_App_FSM_State() == Shopping_menu)
				{
					LCD_Clear();
					LCD_GoToLine(0);
					LCD_DisplayString("Connecting to Server");
					// Connect Server in TCP protocol using IP and Port
					Send_String("AT+CIPSTART=""\"""TCP""\""",""\"""192.168.1.186""\""",""5002""\r\n");
					//Send_String("AT+CWJAP=""\"""Pradeep's iPhone""\""",""\"""Dimboo123$""\"""\r\n");
					Delay(2000);
					res=read_data("OK"); 
					if(res == WiFi_RES_SUCCESS)
					{
						// Reset Fail Count
						FailCnt=0;
						WiFi_Status = WiFi_ServerConnection_Sucess;
						WiFi_State = WiFi_ReConnect;
					}
					else if(res == WiFi_RES_FAIL)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log WiFi connection Error, 
							// Turn ON LED RED
							// Display on LCD
							WiFi_Status = WiFi_ServerConnection_Fail;
							WiFi_State = WiFi_ReConnect;
						}
					}
					else if(res == WiFi_RES_ERROR)
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log Error, like LED RED
							WiFi_Status = WiFi_ServerConnection_Fail;
							WiFi_State = WiFi_ReConnect;
						}
					}
					else
					{
						Delay(4000);
					}
				}
			break;
				
			case WiFi_ReConnect:
				if(Return_App_FSM_State() == Main_menu)
				{
					// Go to connect state
					WiFi_State = WiFi_Connect;
					WiFi_Status= OK;
					FailCnt = 0;
				}

			break;
			
			case WiFi_Invalid_State:
			//default:
			break;			
	
	}

}

/**********************************
Function: Get_WiFi_Init_Status
Description:
Parmeters:
Return:

**********************************/

int Get_WiFi_Init_Status(void)
{
	return WiFi_Init_State;	
}

/**********************************
Function: Get_WiFi_SelfTest_Status
Description:
Parmeters:
Return:

**********************************/
int Get_WiFi_SelfTest_Status(void)
{
	return WiFi_Status;	
}

/**********************************
Function: read_data
Description:
Parmeters:
Return:

**********************************/
uint16_t read_data(uint8_t * str)
{
	int i,buf_size=0,res_size;
	char temp[200];
	uint8_t * bp,*temp2;
	uint16_t res_len=255u,size_str=0;
	
	for(i=0;i<200;i++)
	{
		temp[i] = 0xFF;
	}

	bp = (uint8_t *)temp;

	if(WiFi_UART_RxQ.Size == 0)
	{
		res_len = WiFi_RES_FAIL;
		return res_len;
	}
	while (WiFi_UART_RxQ.Size != 0)
	{ 
		// copy characters up to null terminator
		*bp = Q_Dequeue(&WiFi_UART_RxQ);
		bp++;
		buf_size++;
	}
	size_str = strlen(str);
	for(i=0;i<buf_size;i++)
	{
		/*if(str[0] == temp[i])
		{
			if(str[1] == temp[i+1])
			{
				break;
			}
		}*/
		temp2 = &temp[i];

		if(0 == strncmp(str,temp2,size_str))
		{
			res_len = WiFi_RES_SUCCESS;
			break;
		}
		else if(0 == strncmp("ERROR",temp2,5))
		{
			res_len = WiFi_RES_ERROR;
			break;
		}
		else if(0 == strncmp("FAIL",temp2,4))
		{
			res_len = WiFi_RES_FAIL;
			break;
		}
		/*else if(0 == strncmp("ERROR",temp2,size_str))
		{
			res_len = ERROR;
		}*/
		else
		{
		
		}
	
	}
	//res_strcmp = strcmp(str,temp);
	
	//res_len = strlen(str);
	
	
	return res_len;

}

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Get_Item_info(uint8_t *Barcode_ID[], int *id, char *name[],char *price[],float *itemPriceDec)
{
	int server_res = 0;
	char str[15];
	sprintf(str, "%d", 42);
	//Connect to Server
	
	// Send Barcode Query
	Send_String("AT+CIPSEND=4\r\n");
	
	Delay(50);
	server_res=read_data("OK");
	if(server_res == WiFi_RES_SUCCESS)
	{
		Send_String(Barcode_ID[0]);
		Delay(50);
		server_res=read_data("SEND OK");
		if(server_res == WiFi_RES_SUCCESS)
		{
			//Wait for response from Server
			Delay(1000);
			server_res=read_data_from_Server("+IPD,");
			
		}
	}
		

}

/**********************************
Function: read_data_from_Server
Description:
Parmeters:
Return:

**********************************/
uint16_t read_data_from_Server(uint8_t * str)
{
	int i,buf_size=0,res_size;
	char temp[200];
	uint8_t * bp,*temp2;
	uint16_t Server_res=0,size_str;
	
	for(i=0;i<200;i++)
	{
		temp[i] = 0xFF;
	}

	bp = (uint8_t *)temp;

	while (WiFi_UART_RxQ.Size != 0)
	{ 
		// copy characters up to null terminator
		*bp = Q_Dequeue(&WiFi_UART_RxQ);
		bp++;
		buf_size++;
	}
	size_str = strlen(str);
	for(i=0;i<buf_size;i++)
	{
		/*if(str[0] == temp[i])
		{
			if(str[1] == temp[i+1])
			{
				break;
			}
		}*/
		temp2 = &temp[i];

		if(0 == strncmp(str,temp2,size_str))
		{
			Server_res = WiFi_RES_SUCCESS;
			break;
		}
		else if(0 == strncmp("ERROR",temp2,5))
		{
			Server_res = WiFi_RES_ERROR;
			break;
		}
		else if(0 == strncmp("FAIL",temp2,4))
		{
			Server_res = WiFi_RES_FAIL;
			break;
		}
		/*else if(0 == strncmp("ERROR",temp2,size_str))
		{
			res_len = ERROR;
		}*/
		else
		{
		
		}
	
	}
	//res_strcmp = strcmp(str,temp);
	
	//res_len = strlen(str);
	
	
	return Server_res;

}
	
/** EOF ****/

