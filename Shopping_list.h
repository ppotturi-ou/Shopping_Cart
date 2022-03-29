/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
WiFi UART header file

****************************************************/
#ifndef _SHOPPING_LIST_H
#define _SHOPPING_LIST_H
#include "common.h"


struct shopping_list *insert(struct shopping_list *front, int id, char name[],char price[],float itemPriceDec);
struct shopping_list* deleteNode(struct shopping_list *front, int id);
void display(struct shopping_list  *front);

#endif //_SHOPPING_LIST_H
