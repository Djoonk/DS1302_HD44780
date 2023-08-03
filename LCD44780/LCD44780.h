/*
 * LCD44780.h
 *
 *  Created on: May 28, 2023
 *      Author: oem
 */

#ifndef LCD44780_H_
#define LCD44780_H_

#include <main.h>
#include <string.h>
#include <stdbool.h>


void LCD_En(void);
void LCD_Init(void);							//Initialization display
void SendCmd(uint8_t cmd);						//Send command to display for init
void DisplayOn(void);							//Display ON
void LCD_SendSymbol(uint8_t Dat);				//Send symbol on display
void LCD_PrintText(uint8_t *message);				//Send massage on display
void LCD_Send_command(uint8_t command);			//Send command to display for set cursor
void LCD_SetCursur(uint8_t colum, uint8_t row);	//Set cursor
void LCDClear(void);
char* MakeStringForLCD(uint8_t *Data);

void OutConfigLine(void);
#endif /* LCD44780_H_ */
