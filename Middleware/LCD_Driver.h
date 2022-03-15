/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
LCD Driver header file

****************************************************/
#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "common.h"
#include "I2C.h"

typedef enum  
{
	LCD_FSM1 = 0,
	LCD_FSM2 = 1,
  LCD_FSM3 = 2,
	LCD_FSM4 = 3,
	LCD_Invalid_State = 4,
}LCD_FSM_States;

void Run_LCD_Driver(void);

#endif //_LCD_DRIVER_H


/** EOF ****/

