/*
 * Filename		: lcd-driver.c
 * Author		: Jon Cross
 * Date			: 17/08/2012
 * Description	: Serial bit banger for 2x16 LCD display
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

void LCD_Init(void);
void LCD_WriteLine(uint8_t line, uint8_t len, char *str);

#endif /* #ifndef _LCD_DRIVER_H */
