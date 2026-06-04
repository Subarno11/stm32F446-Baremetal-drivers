/*
 * lcd.h
 *
 *  Created on: 03-Jun-2026
 *      Author: Subarno Pal
 */

#ifndef LCD_H_
#define LCD_H_

#include "stm32f446xx.h"

/*
 * Application Configurable Lines
 */

/*
 *
RS	PA3
EN	PA2
D4	PA10
D5	PA8
D6	PA9
D7	PA7
 */

#define LCD_GPIO_PORT				GPIOA
#define LCD_GPIO_PIN_RS				GPIO_PIN_NO_3
#define LCD_GPIO_PIN_EN				GPIO_PIN_NO_5
#define LCD_GPIO_PIN_D4				GPIO_PIN_NO_10
#define LCD_GPIO_PIN_D5				GPIO_PIN_NO_8
#define LCD_GPIO_PIN_D6				GPIO_PIN_NO_9
#define LCD_GPIO_PIN_D7				GPIO_PIN_NO_7

/*
 * LCD Commands
 */
#define LCD_CMD_4DL_2N_5X8F			0x28 //Function send Command
#define LCD_CMD_DON_CURON			0x0E
#define LCD_CMD_INCADD				0x06
#define LCD_CMD_DIS_CLEAR			0x01
#define LCD_CMD_DIS_RETURN_HOME		0x02

/*
 * bsp exposed APIs
 */
void lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_display_clear_cmd(void);
void lcd_display_clear_cmd(void);
void lcd_display_return_home(void);
void lcd_print_char(uint8_t data);
void lcd_print_string(char *msg);
void lcd_set_cursor(uint8_t row , uint8_t col);
void mdelay(uint32_t cnt);
void udelay(uint32_t cnt);

#endif /* LCD_H_ */
