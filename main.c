/**
 * @file        MyMenu.c
 * @brief       Menu Example
 * @details		(c) anton muehlhofer
 */

/*
 * propose the use of four buttons:  ```[Scroll Left]``` ```[Scroll Right]``` ```[Enter]``` ```[Back]```
 * [Scroll Left] : short keypress key 1
 * [Scroll Right]: short keypress key 2
 * [Enter]       : short keypress key 3
 * [Back]        : long keypress key 3
 *
 */

/***** Includes *********************************************************************/
#include <stdlib.h>				// malloc, free, rand
#include <stdio.h>				// printf, scanf, NULL
#include <stdint.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "gpio.h"
#include "board.h"
#include "tmr.h"

#include "MyConfig.h"
#include "max32660.h"

#include "i2c_lcd.h"
#include "button.h"

#include "CharMenu_settings.h"
#include "CharMenu.h"







/***** Globals ******************************************************************/
/* Variables for SYSTICK */
//volatile unsigned int SYSTICK_counter;


/* Variables for LCD */
/* Parameters for LCD display */
#define I2C_LCD_ADDR 		0x4e  // LCD with PCF8574 expansion

/* Variables for Menu */
CharMenu * charmenu;

/* Variables for gpio */
gpio_cfg_t gpio_in;
gpio_cfg_t gpio_out;

/* Variables for Keyboard */
/* keyboard handler is embedded in SysTick_Handler and called every 8ms
 * keyboard functions elaborates on changes in the global variables
 */
/* Variables for buttons */
Buttons Debounced;

/* Function prototypes for menu */
void menu0_action(void);
void menu1_action(void);
void menu20_action(void);
void Init_menu(void);

/*-------------------------------------------------------------------------------*/
// SysTick_Handler
/*-------------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
	static uint32_t ticks;

    SYSTICK_counter++;
    ticks=SYSTICK_counter;

    // keyboard handler: key detection and debouncing
    if(ticks % 8 == 0)		// detect keyboard every 16ms
    {
    	Button_Handler (&gpio_in);
    }
}


/*-------------------------------------------------------------------------------*/
// SYSTICK_Enable
/*-------------------------------------------------------------------------------*/
void SYSTICK_Enable(void)
{
	uint32_t sysTicks;
	uint32_t error;

	SYSTICK_counter=0;
	if (USE_SYSTEM_CLK)
	{
	    sysTicks = SYSTICK_PERIOD_SYS_CLK;
	}
	else
	{
	    sysTicks = SYSTICK_PERIOD_EXT_CLK;
	}

	error = SYS_SysTick_Config(sysTicks, USE_SYSTEM_CLK, SYSTICK_TIMER);

	printf("SysTick Clock enabled");
	printf("SysTick Clock = %d Hz\n", SYS_SysTick_GetFreq());
	printf("SysTick Period = %d ticks\n", sysTicks);

	if (error != E_NO_ERROR)
	{
	        printf("ERROR: Ticks is not valid");
	}
}


/*-------------------------------------------------------------------------------
 * I2C_LCD_test
 *-------------------------------------------------------------------------------*/
void I2C_LCD_test( void )
{
	uint8_t customChar1[8] = { 0x1F, 0x13, 0x11, 0x14, 0x16, 0x17, 0x17, 0x1F };
	uint8_t customChar2[8] = { 0x1F, 0x1D, 0x11, 0x05, 0x0D, 0x1D, 0x1D, 0x1F };
	const char *text_maxim =    "AX20446 TePaG"; //test pattern generator
	const char *text_hitkey = "hit key to start";

	// Set the LCD address for a 16 chars and 2 line display
	I2C_LCD_define(I2C_LCD_ADDR, 16, 2, LCD_5x8DOTS);
	// initialize the LCD
	I2C_LCD_begin();
	// Turn on the backlight and cursor
	I2C_LCD_backlight();
	//I2C_LCD_cursor();
	//I2C_LCD_blink();

	// send custom data 0 and 1 to display
    I2C_LCD_createChar(0, customChar1);
    I2C_LCD_createChar(1, customChar2);
    I2C_LCD_setCursor(0, 0);
    I2C_LCD_write(0x00);
    I2C_LCD_write(0x01);

    I2C_LCD_printstr(text_maxim);
    I2C_LCD_setCursor(0, 1);
    I2C_LCD_printstr(text_hitkey);
}



/**
 * init routine
 */
void init( void )
{
	// The following code will cause a clear screen event on Hyperterminal and many other terminal emulators
	printf("\033[2J");
	printf("\033[36;1mM A X I M    I N T E G R A T E D\033[0m\n" ); // should set foreground color to cyan and bold

	printf("\033[32m" ); // should set foreground color to green
	printf("--------------------------------------------------------\n");
	printf("Test Pattern Generator v0.1\n");
	printf("(c) Anton Mühlhofer\n");
	printf("--------------------------------------------------------\n");
    printf("Device Setup:\n");
    printf("Timer 1:    SysTick\n" );
    printf("I2C 0A:     LCD 1602\n" );
    printf("GPIO P0.2:  input key1\n");
    printf("GPIO P0.7:  input key2\n");
    printf("GPIO P0.12: input key3\n");
    printf("GPIO P0.13: LED output\n");
    printf("--------------------------------------------------------\n\033[0m");

    SYSTICK_Enable();
}



void Init_menu(void)
{
	// configure level 1 menu's
	Menu *menu0 = new_Menu(menu0);
	menu0->text = "m0: full demo   ";
	menu0->action = &menu0_action;

	Menu *menu1 = new_Menu(menu1);
	menu1->text = "m1: simple demo ";
	menu1->action = &menu1_action;

	Menu *menu2 = new_Menu(menu2);
	menu2->text = "m2: tools       ";

	// configure level 2 menu's and connect with level 1
	Menu *menu20 = new_Menu(menu20);
	menu2->add_child(
				menu2,
				menu20
			);
	menu20->text = "m20: I2C Scan  ";
	menu20->action = &menu20_action;


	// connect root with level 1
	charmenu->menu_root->add_child(
				charmenu->menu_root,
				menu0
			);

	charmenu->menu_root->add_child(
				charmenu->menu_root,
				menu1
			);

	charmenu->menu_root->add_child(
				charmenu->menu_root,
				menu2
			);

}

void menu0_action(void)
{
	I2C_LCD_clear();
	I2C_LCD_setCursor(0,0);
	I2C_LCD_printstr("MAX20446 Test#01");
	I2C_LCD_setCursor(0,1);
	I2C_LCD_printstr("running...      ");

	charmenu->button_read(charmenu);

	I2C_LCD_clear();
	I2C_LCD_setCursor(0, 0);
	I2C_LCD_printstr("Test Generator  ");
}

void menu1_action(void)
{
	I2C_LCD_clear();
	I2C_LCD_setCursor(0,0);
	I2C_LCD_printstr("MAX20446 Test#02");
	I2C_LCD_setCursor(0,1);
	I2C_LCD_printstr("running...      ");

	charmenu->button_read(charmenu);

	I2C_LCD_clear();
	I2C_LCD_setCursor(0, 0);
	I2C_LCD_printstr("Test Generator  ");
}

void menu20_action(void)
{
	I2C_LCD_clear();
	I2C_LCD_setCursor(0,0);
	I2C_LCD_printstr("MAX20444 Test#20");
	I2C_LCD_setCursor(0,1);
	I2C_LCD_printstr("running...      ");

	charmenu->button_read(charmenu);

	I2C_LCD_clear();
	I2C_LCD_setCursor(0, 0);
	I2C_LCD_printstr("Test Generator  ");
}

uint8_t CharMenu__button_read_test(void)
{
	uint8_t output=0;
	volatile uint32_t readkey;

	do{
		readkey = get_key_short(GPIO_PIN_KEY1);
		if (readkey){
			output = CHARMENU_BUTTON_PREV_DOWN;
			printf("[%#06d] %s %s %x\n", SYSTICK_counter,
				   "\033[36mINFO\033[0m",
				   "main: key_short=",
				   readkey);
		}

		readkey = get_key_short(GPIO_PIN_KEY2);
		if (readkey){
			output = CHARMENU_BUTTON_NEXT_DOWN;
			printf("[%#06d] %s %s %x\n", SYSTICK_counter,
				   "\033[36mINFO\033[0m",
				   "main: key_short=",
				   readkey);
		}

		readkey = get_key_short(GPIO_PIN_KEY3);
		if (readkey){
			output = CHARMENU_BUTTON_ENTER_DOWN;
			printf("[%#06d] %s %s %x\n", SYSTICK_counter,
				   "\033[36mINFO\033[0m",
				   "main: key_short=",
				   readkey);
		}

		readkey = get_key_long(GPIO_PIN_KEY3);
		if (readkey){
			output = CHARMENU_BUTTON_BACK_DOWN;
			printf("[%#06d] %s %s %x\n", SYSTICK_counter,
				   "\033[36mINFO\033[0m",
				   "main: key_long=",
				   readkey);
		}
	} while( output == 0);

	return (output);
}


/*-------------------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------------------*/
int main(void)
{


    /* Setup input pin. */
    /* Switch on EV kit is open when non-pressed, and grounded when pressed.  Use an internal pull-up so pin
       reads high when button is not pressed. */
    gpio_in.port = GPIO_PORT_IN;
    gpio_in.mask = GPIO_PIN_KEY1 | GPIO_PIN_KEY2 | GPIO_PIN_KEY3;
    gpio_in.pad = GPIO_PAD_PULL_UP;
    gpio_in.func = GPIO_FUNC_IN;
    GPIO_Config(&gpio_in);

    /* Setup output pin. */
    gpio_out.port = GPIO_PORT_OUT;
    gpio_out.mask = GPIO_PIN_OUT;
    gpio_out.pad = GPIO_PAD_NONE;
    gpio_out.func = GPIO_FUNC_OUT;
    GPIO_Config(&gpio_out);

    Debounced.key_state=0; 		// debounced and inverted key state: bit = 1: key pressed
    Debounced.key_press=0;			// key press detect
    Debounced.key_release=0;		// key release detect
    Debounced.key_rpt=0;           // key long press and repeat
    Debounced.REPEAT_MASK=KEY_REPEAT_MASK;
    Debounced.REPEAT_START=KEY_REPEAT_START;
    Debounced.REPEAT_NEXT=KEY_REPEAT_NEXT;

	init();
	I2C_LCD_test();


	while (!get_key_press( GPIO_PIN_KEYALL ) ); // wait for key gets pressed
	I2C_LCD_setCursor(0, 0);
	I2C_LCD_printstr("Test Generator  ");

	charmenu = new_CharMenu(charmenu);
	Init_menu();
	charmenu->draw(charmenu);

	printf("[%#06d] %s %s %s", SYSTICK_counter,
					"\033[36mINFO\033[0m",
					"main:",
					"start of main-loop\n");
	while(1)
		{
			charmenu->draw(charmenu);
		}


	return 0;
}

