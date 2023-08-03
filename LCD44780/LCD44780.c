/*
 * LCD44780.c
 *
 *  Created on: May 28, 2023
 *      Author: oem
 */
#include "LCD44780.h"
#include <string.h>
/*-----------------------------------------------------------------*/
#define LCD_ADDR (0x27<<1)
extern I2C_HandleTypeDef hi2c1;

/*==== mdelay ====*/
void mdelay(uint32_t cnt)
{
	for (uint32_t i = 0; i < (cnt * 1000); i++)
	{

	}
}
/*==== mdelay ====*/

/*-----------------------------------------------------------------*/

/*==== udelay ====*/
void udelay(uint32_t cnt)
{
	for (uint32_t i = 0; i < (cnt * 1); i++)
	{

	}
}
/*==== udelay ====*/

/*-----------------------------------------------------------------*/

/*==== Function EN ====*/
void LCD_En(void)
{

    uint8_t TxBuff[1] = { 0x08 }; // включение подсветки
    TxBuff[0] |= 0x04;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, TxBuff, sizeof(TxBuff), 10);
    TxBuff[0] &= ~0x04;
    HAL_Delay(5);
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, TxBuff, sizeof(TxBuff), 10);
}
/*==== Function EN ====*/


/*-----------------------------------------------------------------*/

/*==== Send Command ====*/
void LCD_SendCmd(uint8_t cmd)
{
	uint8_t data = 0;
	data = cmd;
	data |= 0x04 | 0x08;
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, &data, 1, 10);
	data &= ~0x04;
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, &data, 1, 10);
}
/*==== Send Command ====*/


/*-----------------------------------------------------------------*/


/*========Display Init========*/

void LCD_Init(void)
{
	//Start init//
	HAL_Delay(20);
	LCD_SendCmd(0x30);
	HAL_Delay(5);
	LCD_SendCmd(0x30);
	HAL_Delay(150);
	LCD_SendCmd(0x30);
	//Start init//

	//Set 4bit interface//
	LCD_SendCmd(0x20);
	//Set 4bit interface//

	//Function set//
	LCD_SendCmd(0x20);
	LCD_SendCmd(0xC0);
	HAL_Delay(20);
	//Function set//

	//Display off//
	LCD_SendCmd(0x00);
	LCD_SendCmd(0x80);
	HAL_Delay(20);
//	Display off//

	//Display clear//
	LCD_SendCmd(0x00);
	LCD_SendCmd(0x10);
	HAL_Delay(20);
	/*===Display clear===*/

	//Entry mode set//
	LCD_SendCmd(0x00);
	LCD_SendCmd(0x60);
	HAL_Delay(20);
	//Entry mode set//

	//Return_Home//
	LCD_SendCmd(0x00);
	LCD_SendCmd(0x30);
    HAL_Delay(2);
	//Return_Home//

	//Display on//
	LCD_SendCmd(0x00);
	LCD_SendCmd(0xf0);
	HAL_Delay(20);
//	Display on//

	//Инициализация завершена.//

}
/*========Display Init========*/


/*-----------------------------------------------------------------*/

/*==== Display On, Cursor On, BlincCurs On ====*/
void DisplayOn(void)
{
	LCD_SendCmd(0x00);
//HAL_Delay(20);
	LCD_SendCmd(0xF0);
	HAL_Delay(20);
}
/*==== Display On, Cursor On, BlincCurs On ====*/

/*-----------------------------------------------------------------*/

/*==== Command Send Data ====*/
void LCD_Write(uint8_t dat)
{
	uint8_t data = 0;
    data = dat;
    data |= 0x04|0x08|0x01;
   	HAL_I2C_Master_Transmit(&hi2c1,LCD_ADDR, &data, 1, 10);
   	data &= ~0x04;
   	HAL_I2C_Master_Transmit(&hi2c1,LCD_ADDR, &data, 1, 10);
}
/*==== Command Send Data ====*/

/*==== LCD_SendSymbol ====*/
void LCD_SendSymbol(uint8_t Dat)
{
	uint8_t sendData[1] =  { 0 };
	sendData[0] = Dat & 0xf0;
	sendData[1] = Dat << 4;
	LCD_Write(sendData[0]);
	LCD_Write(sendData[1]);
}
/*================ LCD_SendSymbol ================*/


/*-----------------------------------------------------------------*/

/*================ Send Command ================*/
void LCD_Send_command(uint8_t command)
{
	uint8_t sendData[1] =  { 0 };
	sendData[0] = (command & 0xf0);
	sendData[1] = (command << 4);
	LCD_SendCmd(sendData[0]);
	LCD_SendCmd(sendData[1]);
}
/*================ Send Command ================*/

/*-----------------------------------------------------------------*/

/*================ Send Message ================*/
void LCD_PrintText(uint8_t *message)
{
    while (*message)
    {
    	LCD_SendSymbol(*message++);
    }
}
/*================ Send Message ================*/

/*-----------------------------------------------------------------*/

/*================ Set Cursor ================*/
void LCD_SetCursur(uint8_t row, uint8_t colum)
{
	colum--;
	switch(row)
	{
	case 1:
		LCD_Send_command(colum |= 0x80 );		//0x80 - d7 = 1, when changing address DDRAM
	break;

	case 2:
		LCD_Send_command(colum |=0xc0);			//0xC0 - address 2 row
	break;
	default:
	break;
	}



}
/*================ Set Cursor ================*/

/*================ Make string for send to Display ================*/
char* MakeStringForLCD(uint8_t *Data)
{

	int n = strlen((const char*)Data);
	Data[n-1] = 0;
	return (char*)Data;
}

/*================ Make string for send to Display ================*/


/*================ Clear Display ================*/
void LCDClear(void)
{
//	LCD_Send_command(0x01);
	LCD_SendCmd(0x00);
	LCD_SendCmd(0x10);
	HAL_Delay(20);


}

/*================ Clear Display ================*/
