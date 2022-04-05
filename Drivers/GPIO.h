/****************************************************
Project: Shopping Cart with Built in billing system
Members: Jonatan Cogiel

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
GPIO header file

****************************************************/

#ifndef GPIO_H
#define GPIO_H

#include <MKL25Z4.h>
#include <stdio.h>

//LEDs
#define R_LED (7) 
#define Y_LED (0) 
#define G_LED (3) 
#define B_LED (4) 
#define LED_ON 1	
#define LED_OFF 0

//Switches
#define SW_MENU   (6)   
#define SW_UP   	(10)   
#define SW_DOWN 	(11)   
#define SW_PRESSED 1
#define SW_RELEASED 0
#define btn_press(x) (!(PTC->PDIR & MASK(x)))

#define MASK(x) (1UL << (x))
void Init_GPIO(void);
void LED_ctrl(int X_LED, int state);
//void Delay (uint32_t dly);
void btn_debounce(void);

int SW_status(int SW);
void Clear_All_LEDs(void);	

#endif
