/****************************************************
Project: Shopping Cart with Built in billing system
Members: ppotturi@oakland.edu

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Shopping List handler file

****************************************************/
//#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "Shopping_list_struct.h"

//#define MAX 10

/* ********************************************************************/
/*  Function to insert a node at the front of the linked list.        */
/*  front: front pointer, id: employee ID, name: employee name        */
/*  desg: Employee itemPrice                                          */
/*  Returns the new front pointer.                                    */
/* ********************************************************************/
void insert(S_shopping_list *front, int id, char name[],char price[],int itemPriceDec)
{
	struct shopping_list *temp;

	temp = front;

	if (temp == NULL)
	{
			//Record error in Memory
			//exit(2);
	}
	temp->ItemId = id;
	strcpy(temp->ItemName, name);
	strcpy(temp->itemPrice, price);
	temp->itemPrice_dec = itemPriceDec;

}
/*  End of insert() */
 
 
/***************************************************/
/*  Function to Clear List                         */
/*  front: front pointer,                          */
/*  Returns: void                                  */
/***************************************************/
void ClearList(S_shopping_list *front)
{

	front->ItemId = 0xFF;
	strcpy(front->ItemName, "");
	strcpy(front->itemPrice, "");
	front->itemPrice_dec = 0;

}

 
/*  ********************************************************/
/*  Function to display the linked list                    */
/*  front: front pointer,                                  */
/*  Returns: the modified list.                            */
/*  ********************************************************/
void display(S_shopping_list  *front)
{
    struct shopping_list *ptr;
 
    //for (ptr = front; ptr != NULL; ptr = ptr->next)
    {
        //printNode(ptr);
    }
}

 

/*** EOF  ****/

