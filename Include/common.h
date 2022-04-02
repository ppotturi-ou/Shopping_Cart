/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Lab#3 common header file

****************************************************/
#ifndef _COMMON_H
#define _COMMON_H

#include <MKL25Z4.h>  // Device header
#include "delay.h"

#define MASK(x) (1UL << (x))

#define ERROR 0
#define OK    1
#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

typedef enum  
{
	Init = 0,
	Self_Test = 1,
  Main_menu = 2,
	Shopping_menu = 3,
	Scanning = 4,
	Shopping_Complete = 5,
	Invalid_State = 6,
}SC_FSM_States;

extern SC_FSM_States Return_App_FSM_State(void);
	
#endif //_COMMON_H
/** EOF **/
