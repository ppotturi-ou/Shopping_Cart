#ifndef GPIO_H
#define GPIO_H

#include <MKL25Z4.h>
#include <stdio.h>

//LEDs
#define R_LED (7) 
#define Y_LED (0) 
#define G_LED (3) 
#define B_LED (4) 

//Switches
#define SW1   (6)   
#define SW2   (10)   
#define SW3 	(11)   
#define btn_press(x) (!(PTC->PDIR & MASK(x)))

#define MASK(x) (1UL << (x))
void Init_GPIO(void);
void led(int X_LED, int state);
//void Delay (uint32_t dly);
void btn_debounce(void);

static int on = 1;
static int off = 2;

#endif
