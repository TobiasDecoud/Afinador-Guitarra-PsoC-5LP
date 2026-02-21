#include "lcd_i2c.h"
#include "project.h" 


#define LCD_ADDR     0x27   // Dirección I2C del módulo LCD
// Bits de control
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RW        0x02
#define LCD_RS        0x01




static void I2C_WriteByte(uint8 address, uint8 data) {
    uint8 status = I2C_1_MasterSendStart(address, 0); 
    if(status == I2C_1_MSTR_NO_ERROR) {
        I2C_1_MasterWriteByte(data);
    }
    I2C_1_MasterSendStop();
}

static void LCD_WriteNibble(uint8 nibble, uint8 rs) {
    uint8 data = nibble | LCD_BACKLIGHT | rs; 
    
    // Pulso de Enable
    I2C_WriteByte(LCD_ADDR, data | LCD_ENABLE);
    CyDelayUs(1);
    I2C_WriteByte(LCD_ADDR, data & ~LCD_ENABLE);
    CyDelayUs(50);
}

static void LCD_Send(uint8 value, uint8 rs) {
    uint8 highNibble = value & 0xF0;
    uint8 lowNibble = (value << 4) & 0xF0;
    
    LCD_WriteNibble(highNibble, rs);
    LCD_WriteNibble(lowNibble, rs);
}

void LCD_Data(uint8 data) {
    LCD_Send(data, LCD_RS); 
}

void LCD_Command(uint8 cmd) {
    LCD_Send(cmd, 0); 
}

void LCD_Init(void) {
    CyDelay(50); 
    
    
    LCD_WriteNibble(0x30, 0);
    CyDelay(5);
    LCD_WriteNibble(0x30, 0);
    CyDelayUs(150);
    LCD_WriteNibble(0x30, 0);
    LCD_WriteNibble(0x20, 0); 
    

    LCD_Command(0x28); 
    LCD_Command(0x0C); 
    LCD_Command(0x06); 
    LCD_Command(0x01); 
    CyDelay(2);
}

void LCD_SetCursor(uint8 row, uint8 col) {
    uint8 row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    LCD_Command(0x80 | (col + row_offsets[row]));
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_Data(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Command(0x01); 
    CyDelay(2);        
}