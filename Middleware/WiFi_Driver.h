/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
ESP WiFi Driver header file

****************************************************/
#ifndef _WIFIDRIVER_H
#define _WIFIDRIVER_H

#include "common.h"
#include "WiFi_UART.h"

#define WiFi_RES_FAIL 0x34
#define WiFi_RES_ERROR 0x67
#define WiFi_RES_SUCCESS 0x0A
#define WiFi_SelfTest_Sucess 0x10
#define WiFi_SelfTest_Fail 0x12
#define WiFi_Connection_Sucess 0x14
#define WiFi_Connection_Fail 0x16
#define WiFi_ServerConnection_Sucess 0x18
#define WiFi_ServerConnection_Fail 0x1A

typedef enum  
{
	WiFi_Init = 0,
	WiFi_SelfTest = 1,
  WiFi_Connect = 2,
	WiFi_ReConnect = 3,
	WiFi_Connect_Server = 4,
	WiFi_Invalid_State = 5,
}WiFi_FSM_States;

void Init_WiFi_Driver(void);
void Run_WiFi_Driver(void);
int Get_WiFi_Init_Status(void);
int Get_WiFi_SelfTest_Status(void);
uint16_t read_data(uint8_t * str);
uint16_t read_data_from_Server(uint8_t * str,uint8_t Check_Barcode_ID[], char name[],char price[]);
uint16_t Get_Item_info(uint8_t Barcode_ID[12], char name[],char price[],int* itemPriceDec);

#endif //_WIFIDRIVER_H
/** EOF ****/

