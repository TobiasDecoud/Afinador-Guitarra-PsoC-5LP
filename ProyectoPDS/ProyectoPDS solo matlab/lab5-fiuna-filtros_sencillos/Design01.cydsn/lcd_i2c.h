#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "project.h"



void LCD_Init(void);
void LCD_SetCursor(uint8 row, uint8 col);
void LCD_Print(char *str);
void LCD_Clear(void);
void LCD_Command(uint8 cmd);


#endif 