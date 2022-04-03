/****************************************************
Project: Shopping Cart with Built in billing system
Members: 

Course: ECE 5721/4721 Winter 2022
Oakland University

Description:
Barcode file

****************************************************/
#include "Barcode_Driver.h"
static BC_FSM_States Barcode_State = BC_Init;
static int BC_New_Code_Available = FALSE;
static char Barcode_Data[BARCODE_RX_SIZE]={"00000000000000"};
int BC_New_Data_Ready = FALSE;

uint16_t Extract_BC_data(void);

/**********************************
Function: Main
Description:
Parmeters:
Return:

**********************************/
void Run_Barcode_Driver(void)
{

	switch(Barcode_State)
	{
	
			case BC_Init:
				Init_UART0(9600);
				Barcode_State = BC_ReadCode;
			break;
			
			case BC_ReadCode:
				if(Get_BC_Available_Flag() == TRUE)
				{
					if(Extract_BC_data()==TRUE)
					{
						Set_BC_Available_Flag(FALSE);
						BC_New_Data_Ready = TRUE;
						
					}
					
				}
			break;
			
			case BC_FSM3:

			break;
			
			case BC_FSM4:

			break;
			
			case BC_Invalid_State:
			//default:
			break;			
	
	}
	
}

void Set_BC_Available_Flag(int BC_Flag)
{
	BC_New_Code_Available = BC_Flag;

}

int Get_BC_Available_Flag(void)
{
	return BC_New_Code_Available;

}


/**********************************
Function: read_data
Description:
Parmeters:
Return:

**********************************/
uint16_t Extract_BC_data(void)
{
	uint16_t BC_res=FALSE;
	char * bp;
	
	bp = (char *)Barcode_Data;

	if(BC_RxQ.Size == 0)
	{
		BC_res = FALSE;
		return BC_res;
	}
	while (BC_RxQ.Size != 0)
	{ 
		// copy characters up to null terminator
		*bp = Q_Dequeue(&BC_RxQ);
		bp++;
	}
	
	BC_res = TRUE;
		
	return BC_res;
}

void Get_New_BC_Data(char* BC_Dest)
{
	strncpy(BC_Dest,Barcode_Data,BARCODE_RX_SIZE-2);
	
}

/** EOF ****/
