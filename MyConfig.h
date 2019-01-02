/* MyConfig.h
 */
#ifndef MYCONFIG_H
#define MYCONFIG_H

/* Parameters for GPIO */
#define GPIO_PORT_IN                PORT_0
#define GPIO_PIN_KEY1               PIN_12
#define GPIO_PIN_KEY2               PIN_2
#define GPIO_PIN_KEY3				PIN_7
#define GPIO_PIN_KEYALL             (GPIO_PIN_KEY1 | GPIO_PIN_KEY2 | GPIO_PIN_KEY3)

#define GPIO_PORT_OUT               PORT_0
#define GPIO_PIN_OUT                PIN_13

#define KEY_REPEAT_MASK   (GPIO_PIN_KEY1 | GPIO_PIN_KEY2 | GPIO_PIN_KEY3) // repeat: key1, key2
#define KEY_REPEAT_START 64          // after 500ms
#define KEY_REPEAT_NEXT  25          // every 200ms


/* Parameters for I2C  */
#define I2C_MASTER	    	MXC_I2C0
#define I2C_MASTER_IDX		0

#define I2C_TIMEOUT       	MXC_DELAY_MSEC(1)

#define I2C_ERROR   (MXC_F_I2C_INT_FL0_ARB_ER | MXC_F_I2C_INT_FL0_TO_ER | MXC_F_I2C_INT_FL0_ADDR_NACK_ER | \
                    MXC_F_I2C_INT_FL0_DATA_ER | MXC_F_I2C_INT_FL0_DO_NOT_RESP_ER | MXC_F_I2C_INT_FL0_START_ER | \
                    MXC_F_I2C_INT_FL0_STOP_ER)


/* Parameters for SysTick */
#define SYSTICK_TIMER 		MXC_TMR1   	/* Any timer other than PWM_TIMER */

/* Parameters for PWM timer */
#define PWM_TIMER  			MXC_TMR0  /* The timer selection must match the available timer for PORT_PWM.PIN_PWM */


/* Parameters for SysTick */
// #define SYSTICK_TIMER 		MXC_TMR1   	/* defined in config.h */
#define USE_SYSTEM_CLK 		1
#define SYSTICK_PERIOD_SYS_CLK 96000 	//1ms with 96MHz system clock
#define SYSTICK_PERIOD_EXT_CLK 3277    	//100ms with 32768Hz external RTC clock
#define SYSTICK_LED_IDX  	0


#endif //MYCONFIG_H
