/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

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

typedef enum  
{
	WiFi_Init = 0,
	WiFi_SelfTest = 1,
  WiFi_Connect = 2,
	WiFi_FSM4 = 3,
	WiFi_Invalid_State = 4,
}WiFi_FSM_States;

void Init_WiFi_Driver(void);
void Run_WiFi_Driver(void);
int Get_WiFi_Init_Status(void);
int Get_WiFi_SelfTest_Status(void);
uint16_t read_data(uint8_t * str);

#endif //_WIFIDRIVER_H
/** EOF ****/

