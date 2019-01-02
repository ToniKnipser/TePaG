#ifndef CHAR_MENU_SETTINGS_H
#define CHAR_MENU_SETTINGS_H


///Menu uses 4 cm_Button, comprises Enter, Back, Previous, and Next, which defined here:

///key mapping
/// [Scroll Left] : short keypress key 1
/// [Scroll Right]: short keypress key 2
/// [Enter]       : short keypress key 3
/// [Back]        : long keypresss key 3
#define CHARMENU_BUTTON_PREV_DOWN	1
#define CHARMENU_BUTTON_NEXT_DOWN	2
#define CHARMENU_BUTTON_ENTER_DOWN	3
#define CHARMENU_BUTTON_BACK_DOWN	4

/**
 * wrap functions for LCD operation you use for library here
 * __lcd_gotoXY: to set cursor to (x,y)
 * __lcd_string: to print string to LCD
 * 
 * for example you have lcd_lib.h from scienceprog, you will find its method:
 *  LCDstring(string, len);
 *  LCDGotoXY(x,y);
 * then you have to define it 
 * 	__lcd_string(string, length) LCDstring(string,length)
 * 	__lcd_gotoXY(x, y) LCDGotoXY(x,y)
 */

/*
#define __lcd_string(string)	I2C_LCD_printstr(string)
#define __lcd_gotoXY(x, y) 		I2C_LCD_setCursor(x, y)
*/

#endif
