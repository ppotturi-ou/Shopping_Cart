/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Shopping List handler header file

****************************************************/
#ifndef _SHOPPING_LIST_H
#define _SHOPPING_LIST_H
#include "common.h"

typedef struct shopping_list
{
    int  ItemId;
    char ItemName[10];
    char itemPrice[7];
	  int  itemPrice_dec;
	  //char dummy[3];
}S_shopping_list;

void insert(S_shopping_list *front, int id, char name[],char price[],int itemPriceDec);
void ClearList(S_shopping_list *front);
void display(S_shopping_list  *front);

#endif //_SHOPPING_LIST_H
