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
#include "GPIO.h"

static WiFi_FSM_States WiFi_State = WiFi_Init;
static int WiFi_Init_State = ERROR;
static int WiFi_SelfTest_Status = ERROR;
static int WiFi_Connection_Status = ERROR;
static int Server_Status = ERROR;

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
						WiFi_SelfTest_Status = WiFi_SelfTest_Sucess;
						
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
							WiFi_SelfTest_Status = WiFi_SelfTest_Fail;
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
							WiFi_SelfTest_Status = WiFi_SelfTest_Fail;						
						}
					}
				}
			break;
			
			case WiFi_Connect:
				if(Return_App_FSM_State() == Main_menu)
				{
						Send_String("AT+CIFSR\r\n");
						Delay(500);
						res=read_data("+CIFSR:STAIP,""\"""0.0.0.0""\""); 
						if(res == WiFi_RES_SUCCESS)// WiFi not connected
						{	
							LCD_Clear();
							LCD_GoToLine(0);
							LCD_DisplayString("Connecting to WiFi");
							//Send_String("AT+CWJAP=""\"""MySpectrumWiFi78-2G""\""",""\"""Kprexy2013""\"""\r\n");
							//Send_String("AT+CWJAP=""\"""Pradeep iPhone""\""",""\"""Dimboo123$""\"""\r\n");
							//Send_String("AT+CWJAP=""\"""WAVLINK-N""\""",""\"""Shopping123$""\"""\r\n");
							Send_String("AT+CWJAP=""\"""Pradeep iPhone""\""",""\"""Dimboo123$""\"""\r\n");
							Delay(2000);
							res=read_data("WIFI CONNECTED"); 
							if(res == WiFi_RES_SUCCESS)
							{
								// Reset Fail Count
								FailCnt=0;
								WiFi_Connection_Status = WiFi_Connection_Sucess;
								WiFi_State = WiFi_Connect_Server;
							}
							else if((res == WiFi_RES_FAIL)|(res == 0xFF))
							{
								//Stay in this state and try again
								FailCnt++;
								if (FailCnt >= 1)
								{
									//Log WiFi connection Error, 
									// Turn ON LED RED
									// Display on LCD
									WiFi_Connection_Status = WiFi_Connection_Fail;
									//WiFi_State = WiFi_ReConnect;
									WiFi_State = WiFi_Invalid_State;
								}
							}
							else if((res == WiFi_RES_ERROR)|(res == 0xFF))
							{
								//Stay in this state and try again
								FailCnt++;
								if (FailCnt >= 1)
								{
									//Log Error, like LED RED
									//WiFi_State = WiFi_ReConnect;
									WiFi_Connection_Status = WiFi_Connection_Fail;
									WiFi_State = WiFi_Invalid_State;
								}
							}
							else
							{
								Delay(4000);
							}
						}
						else
						{
								// Reset Fail Count
								FailCnt=0;
								WiFi_Connection_Status = WiFi_Connection_Sucess;
								WiFi_State = WiFi_Connect_Server;
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
					//Send_String("AT+CIPSTART=""\"""TCP""\""",""\"""192.168.1.186""\""",""5002""\r\n");
					//Send_String("AT+CIPSTART=""\"""TCP""\""",""\"""192.168.10.109""\""",""5002""\r\n");
					Send_String("AT+CIPSTART=""\"""TCP""\""",""\"""172.20.10.8""\""",""5002""\r\n");//iPhone WiFi
					Delay(1000);
					res=read_data("OK"); 
					if(res == WiFi_RES_SUCCESS)
					{
						// Reset Fail Count
						FailCnt=0;
						Server_Status = WiFi_ServerConnection_Sucess;
						WiFi_State = WiFi_ReConnect;
					}
					else if((res == WiFi_RES_FAIL)|(res == 0xFF))
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log WiFi connection Error, 
							// Turn ON LED RED
							// Display on LCD
							Server_Status = WiFi_ServerConnection_Fail;
							WiFi_State = WiFi_ReConnect;
						}
					}
					else if((res == WiFi_RES_ERROR)|(res == 0xFF))
					{
						//Stay in this state and try again
						FailCnt++;
						if (FailCnt >= 1)
						{
							//Log Error, like LED RED
							Server_Status = WiFi_ServerConnection_Fail;
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
					//WiFi_Connection_Status = ERROR;
					FailCnt = 0;
				}

			break;
			
			case WiFi_Invalid_State:
			//default:
			break;			
	
	}
	if(Return_App_FSM_State() == Redo_Self_Test)
	{
		WiFi_State = WiFi_SelfTest;
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
	return WiFi_SelfTest_Status;	
}

/**********************************
Function: Get_WiFi_SelfTest_Status
Description:
Parmeters:
Return:

**********************************/
int Get_WiFi_Connection_Status(void)
{
	return WiFi_Connection_Status;	
}

/**********************************
Function: Get_WiFi_SelfTest_Status
Description:
Parmeters:
Return:

**********************************/
int Get_Server_Connection_Status(void)
{
	return Server_Status;	
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
uint16_t Get_Item_info(uint8_t Barcode_ID[12], char name[],char price[],int* itemPriceDec)
{
	int server_res = 0,i=0,j=0;
	char str[15];
	int tem_dec=0;
	
	*itemPriceDec = 0;
	//sprintf(str, "%d", 42);
	//Connect to Server
	
	//Todo: updated the legth of cmd
	// Send Barcode Query
	Send_String("AT+CIPSEND=12\r\n");
	
	Delay(50);
	server_res=read_data("OK");
	if(server_res == WiFi_RES_SUCCESS)
	{
		Send_String(&Barcode_ID[0]);
		LED_ctrl(Y_LED,LED_ON);
		Delay(50);
		server_res=read_data("SEND OK");
		if(server_res == WiFi_RES_SUCCESS)
		{
			
			//Wait for response from Server
			Delay(1000);
			server_res=read_data_from_Server("+IPD,",&Barcode_ID[0],&name[0],&price[0]);
			if(server_res == WiFi_RES_SUCCESS)
			{
					// Integer value
					i=0;
					while((price[i]!='.')&& (i<5))
					{
						*itemPriceDec *=10;
						*itemPriceDec += (price[i++]-0x30);
					}
					//Decimal Value
					i++;
					for(j=1;j<3;j++)
					{
						*itemPriceDec *=10;
						*itemPriceDec += (price[i++]-0x30);
					}
				return 0x10;
			}
		}
		else
		{
			return 0xFF;
		
		}
	}
		

}

/**********************************
Function: read_data_from_Server
Description:
Parmeters:
Return:

**********************************/
uint16_t read_data_from_Server(uint8_t * str, uint8_t Check_Barcode_ID[], char name[],char price[])
{
	int i,buf_size=0,res_size,str_cnt=0;
	char temp[200],res_len[3],res_id[12];
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
	/*** Check response +IPD,  ******/
	size_str = strlen(str);
	for(i=0;i<buf_size;i++,str_cnt++)
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
			str_cnt+=size_str;
			break;
		}
		else if(0 == strncmp("ERROR",temp2,5))
		{
			Server_res = WiFi_RES_ERROR;
			return Server_res;
			//break;
		}
		else if(0 == strncmp("FAIL",temp2,4))
		{
			Server_res = WiFi_RES_FAIL;
			return Server_res;
			//break;
		}
		/*else if(0 == strncmp("ERROR",temp2,size_str))
		{
			res_len = ERROR;
		}*/
		else
		{
		
		}
	
	}
	
	/*** Check response :,  ******/
	i=0;
	while((temp[str_cnt]!=':')&& (str_cnt<199))
	{
		res_len[i++]= temp[str_cnt++];
	}
	str_cnt++;
	
	// Save res barcode id to check is the response is for the same request
	for(i=0;i<12;i++)
	{
		res_id[i]=0xFF;
	}
	i=0;
	while((temp[str_cnt]!=',')&& (str_cnt<199))
	{
		res_id[i++]= temp[str_cnt++];
	}
	res_id[i++]='\0';
	
	
	size_str = strlen(Check_Barcode_ID);
	if(0 != strncmp(Check_Barcode_ID,res_id,size_str))
	{
			Server_res = WiFi_RES_FAIL;
			return Server_res;
	}
	
	str_cnt++;
	i=0;
	while((temp[str_cnt]!=',')&& (str_cnt<199))
	{
		name[i++]= temp[str_cnt++];
	}
	
	str_cnt++;
	i=0;
	while((temp[str_cnt]!='*')&& (str_cnt<199))
	{
		price[i++]= temp[str_cnt++];
	}
	
	if(str_cnt>199)
	{
		Server_res = WiFi_RES_FAIL;
		return Server_res;
	}
	
	//res_strcmp = strcmp(str,temp);
	
	//res_len = strlen(str);
	
	
	return Server_res;

}

/**********************************
Function: Disconnect_Server
Description:
Parmeters:
Return:

**********************************/

void Disconnect_Server(void)
{
	Send_String("AT+CIPCLOSE\r\n");
	Delay(500);
	read_data("OK");	
}
/** EOF ****/

