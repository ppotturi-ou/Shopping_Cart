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
	Self_Test = 10,
  Main_menu = 20,
	Shopping_menu = 30,
	Scanning = 40,
	Shopping_Complete = 50,
	Redo_Self_Test = 60,
	Invalid_State = 70,
}SC_FSM_States;

extern SC_FSM_States Return_App_FSM_State(void);
	
#endif //_COMMON_H
/** EOF **/
