/*
 * C program to illustrate the operations of singly linked list
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "Shopping_list.h"

//#define MAX 10
 
struct shopping_list
{
    int  ItemId;
    char ItemName[10];
    char itemPrice[7];
	  float  itemPrice_dec;
	  char dummy[3];
    struct shopping_list *next;
};



/* ********************************************************************/
/*  Function to insert a node at the front of the linked list.        */
/*  front: front pointer, id: employee ID, name: employee name        */
/*  desg: Employee itemPrice                                          */
/*  Returns the new front pointer.                                    */
/* ********************************************************************/
struct shopping_list *insert(struct shopping_list *front, int id, char name[],char price[],float itemPriceDec)
{
    struct shopping_list *temp;
 
    temp = (struct shopping_list*)malloc(sizeof(struct shopping_list));
 
    if (temp == NULL)
    {
        //Record error in Memory
        //exit(2);
    }
    temp->ItemId = id;
    strcpy(temp->ItemName, name);
    strcpy(temp->itemPrice, price);
		temp->itemPrice_dec = itemPriceDec;
    temp->next = front;
    front = temp ;
    return(front);
}
/*  End of insert() */
 
 
/*  ********************************************************/
/*  Function to deleteNode a node based on employee number */
/*  front: front pointer, id: Key value                    */
/*  Returns: the modified list.                            */
/*  ********************************************************/
struct shopping_list* deleteNode(struct shopping_list *front, int id)
{
    struct shopping_list *ptr;
    struct shopping_list *bptr;
 
    if (front->ItemId == id)
    {
        ptr = front;
        //printf("\n Node deleted:");
        //printNode(front);
        front = front->next;
        free(ptr);
        return(front);
    }
    for (ptr = front->next, bptr = front; ptr != NULL; ptr = ptr->next, bptr = bptr->next)
    {
        if (ptr->ItemId == id)
        {
            bptr->next = ptr->next;
            free(ptr);
            return(front);
        }
    }
    return(front);
}

 
/*  ********************************************************/
/*  Function to display the linked list                    */
/*  front: front pointer,                                  */
/*  Returns: the modified list.                            */
/*  ********************************************************/
void display(struct shopping_list  *front)
{
    struct shopping_list *ptr;
 
    for (ptr = front; ptr != NULL; ptr = ptr->next)
    {
        //printNode(ptr);
    }
}

 

/*** EOF  ****/

