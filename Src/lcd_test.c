/*
 * lcd_test.c
 *
 *  Created on: 04-Jun-2026
 *      Author: Subarno Pal
 */
#include "lcd.h"
int main(void)
{
    lcd_init();

   lcd_set_cursor(1, 1);
   lcd_print_string("Hi");
}
