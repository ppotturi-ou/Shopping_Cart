/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
ESP WiFi Driver file

****************************************************/
#include "WiFi_Driver.h"
#include <string.h>

static WiFi_FSM_States WiFi_State = WiFi_Init;
static int WiFi_Init_State = ERROR;
static int WiFi_SelfTest_State = ERROR;

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Init_WiFi_Driver(void)
{
	Init_UART2(115200);

}

/**********************************
Function: Main
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
				WiFi_Init_State = OK;
				WiFi_State = WiFi_SelfTest;
			break;
			
			case WiFi_SelfTest:
				Send_String((uint8_t *)"AT+RST\r\n");
				Delay(1500);
				while (WiFi_UART_RxQ.Size != 0)
				{ 
					// Clear buffer after reset
					Q_Dequeue(&WiFi_UART_RxQ);
				}
				Send_String((uint8_t *)"AT\r\n");
				Delay(5);
				res=read_data((uint8_t *)"OK");
				if(res == WiFi_RES_SUCCESS)
				{
					// Reset Fail Count
					FailCnt=0;
					// Set the status to let application know
					WiFi_SelfTest_State = OK;
					
					// Goto next state
					WiFi_State = WiFi_Connect;
				}
				else if(res == WiFi_RES_FAIL)
				{
					//Stay in this state and try again
					FailCnt++;
					if (FailCnt >= 0xFFFF)
					{
						//Log Error, like LED RED
						// Turn ON LED RED
						// Display on LCD
					}
				}
				else if(res == WiFi_RES_ERROR)
				{
					//Stay in this state and try again
					FailCnt++;
					if (FailCnt >= 0xFFFF)
					{
						//Log Error, like LED RED
						// Turn ON LED RED
						// Display on LCD					
					}
				}
			break;
			
			case WiFi_Connect:
				Send_String("AT+CWJAP=""\"""MySpectrumWiFi78-2G""\""",""\"""Kprexy2013""\"""\r\n");
			  //Send_String("AT+CWJAP=""\"""Pradeep's iPhone""\""",""\"""Dimboo123$""\"""\r\n");
				Delay(6000);
				res=read_data("WIFI CONNECTED"); 
				if(res == WiFi_RES_SUCCESS)
				{
					// Reset Fail Count
					FailCnt=0;
					
					// Test
					Send_String("AT\r\n");
					WiFi_State = WiFi_FSM4;
				}
				else if(res == WiFi_RES_FAIL)
				{
					//Stay in this state and try again
					FailCnt++;
					if (FailCnt >= 3)
					{
						//Log WiFi connection Error, 
						// Turn ON LED RED
						// Display on LCD
					
					}
				}
				else if(res == WiFi_RES_ERROR)
				{
					//Stay in this state and try again
					FailCnt++;
					if (FailCnt >= 3)
					{
						//Log Error, like LED RED
					
					}
				}

			break;
			
			case WiFi_FSM4:

			break;
			
			case WiFi_Invalid_State:
			//default:
			break;			
	
	}
	
}

int Get_WiFi_Init_Status(void)
{
	return WiFi_Init_State;	
}

int Get_WiFi_SelfTest_Status(void)
{
	return WiFi_SelfTest_State;	
}

uint16_t read_data(uint8_t * str)
{
	int i,buf_size=0,res_size;
	char temp[200];
	uint8_t * bp,*temp2;
	static uint16_t res_len=255u,size_str;
	
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

/** EOF ****/

