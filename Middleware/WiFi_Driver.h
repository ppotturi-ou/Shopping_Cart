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

typedef enum  
{
	WiFi_FSM1 = 0,
	WiFi_FSM2 = 1,
  WiFi_FSM3 = 2,
	WiFi_FSM4 = 3,
	WiFi_Invalid_State = 4,
}WiFi_FSM_States;

void Run_WiFi_Driver(void);

#endif //_WIFIDRIVER_H
/** EOF ****/

