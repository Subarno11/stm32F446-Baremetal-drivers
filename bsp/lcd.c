/*
 * lcd.c
 *
 *  Created on: 03-Jun-2026
 *      Author: Subarno Pal
 */


#include "lcd.h"
static void write_4_bits(uint8_t value);
static void lcd_enable(void);

#define SYST_CSR   (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR   (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR   (*(volatile uint32_t*)0xE000E018)

#define SYSTICK_TIM_CLK 16000000U

void lcd_send_cmd(uint8_t cmd){
	//RS = 0 , for LCD Command
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_RS, GPIO_PIN_RESET);
	//Already RW = 0 (Directly Grounded)

	//First send the HIgher Nibble
	write_4_bits(cmd >> 4);
	//First send the Lower Nibble
	write_4_bits(cmd & 0x0F);

	udelay(50); //Execution time > 37 micro sec

}

void lcd_print_char(uint8_t data){
	//RS = 0 , for LCD Command
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_RS, GPIO_PIN_SET);
	//Already RW = 0 (Directly Grounded)
	//First send the HIgher Nibble
	write_4_bits(data >> 4);
	//First send the Lower Nibble
	write_4_bits(data & 0x0F);

	udelay(50); //Execution time > 37 micro sec
}

void lcd_print_string(char *msg)
{
    while(*msg)
    {
        lcd_print_char((uint8_t)*msg++);
    }
}

void lcd_display_clear_cmd(void){
	lcd_send_cmd(LCD_CMD_DIS_CLEAR);

	mdelay(2);
}

//Cursor returns to the Home Position:
void lcd_display_return_home(void){
	lcd_send_cmd(LCD_CMD_DIS_RETURN_HOME);

	mdelay(2);
}

/*
 * Set LCD to a specified location given by row and column coordinates
 * Row number (1 to 2)
 * Column number (1 to 16)
 */
void lcd_set_cursor(uint8_t row , uint8_t col){
	col--;
	switch (row) {
		case 1:
			lcd_send_cmd((col |= 0x80));
			break;
		case 2:
			lcd_send_cmd((col |= 0xC0));
			break;

		default:
			break;
	}
}
void lcd_init(void){
	//1. Configure the GPIO Pins which are used for LCD connections
	GPIO_Handle_t lcd_gpio_handle;
	lcd_gpio_handle.pGPIOx = LCD_GPIO_PORT;
	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_RS;
	GPIO_Init(&lcd_gpio_handle);

	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_EN;
	GPIO_Init(&lcd_gpio_handle);

	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_D7;
	GPIO_Init(&lcd_gpio_handle);

	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_D6;
	GPIO_Init(&lcd_gpio_handle);

	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_D5;
	GPIO_Init(&lcd_gpio_handle);
	lcd_gpio_handle.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_PIN_D4;
	GPIO_Init(&lcd_gpio_handle);

	//Reset All the GPIOs initially
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_D7, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_D4, GPIO_PIN_RESET);

	//2. Do the LCD Initialisation (According to the  4-Bit Interface Flowchart for the LCD DS):
	mdelay(40);

	//RS = 0 , for LCD Command
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_RS, GPIO_PIN_RESET);
	//Already RW = 0 (Directly Grounded)

	//Write 4 bits of data/command on the D7, D6 , D5, D4 lines
	write_4_bits(0x03); //i.e 0011

	//Wait for 5 ms
	mdelay(5);

	write_4_bits(0x03);

	//wait for 100 us
	udelay(150);

	write_4_bits(0x03);
	udelay(150);
	write_4_bits(0x02);
	udelay(150);

	//Function Set Command
	lcd_send_cmd(LCD_CMD_4DL_2N_5X8F);

	//Display On and Curesor ON Command
	lcd_send_cmd(LCD_CMD_DON_CURON);

	//Display clear
	lcd_display_clear_cmd();

	//Entry Mode set Cmd
	lcd_send_cmd(LCD_CMD_INCADD);
}

static void write_4_bits(uint8_t value){
	GPIO_WriteToOutputPin(LCD_GPIO_PORT , LCD_GPIO_PIN_D4 , ((value >> 0) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT , LCD_GPIO_PIN_D5 , ((value >> 1) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT , LCD_GPIO_PIN_D6 , ((value >> 2) & 0x1));
	GPIO_WriteToOutputPin(LCD_GPIO_PORT , LCD_GPIO_PIN_D7 , ((value >> 3) & 0x1));

	 lcd_enable();
}

static void lcd_enable(void)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_EN, GPIO_PIN_SET);
	udelay(2);

	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_PIN_EN, GPIO_PIN_RESET);
	udelay(50);
}


void udelay(uint32_t us)
{
	SYST_RVR = (SYSTICK_TIM_CLK / 1000000U) - 1;
	SYST_CVR = 0;

	SYST_CSR = (1 << 2) | (1 << 0);

	while(us--)
	{
		while(!(SYST_CSR & (1 << 16)));
	}

	SYST_CSR = 0;
}

void mdelay(uint32_t ms)
{
	while(ms--)
	{
		udelay(1000);
	}
}
