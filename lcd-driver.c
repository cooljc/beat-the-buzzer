/*
 * Filename		: lcd-driver.c
 * Author		: Jon Cross
 * Date			: 17/08/2012
 * Description	: Serial bit banger for 2x16 LCD display
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA  02110-1301  USA.
 */


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
//#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

//#include "lcd-driver.h"

/* ------------------------------------------------------------------ */
/* LCD PORT/PIN */
/* ------------------------------------------------------------------ */
#define LCDPIN			(1 << PC2)
#define InitLCD()		(DDRC |= LCDPIN)
#ifndef LCD_INVERSE_LOGIC
	#define LcdPinHigh()	(PORTC |= LCDPIN)
	#define LcdPinLow()		(PORTC &= ~LCDPIN)
#else
	#define LcdPinHigh()	(PORTC &= ~LCDPIN)
	#define LcdPinLow()		(PORTC |= LCDPIN)
#endif


/* ------------------------------------------------------------------ */
/* TX Delay (F_CPU = 16MHz, Baud = 9600) */
/* ------------------------------------------------------------------ */
#if F_CPU == 16000000
const uint16_t lcd_tx_delay = 233;
const int XMIT_START_ADJUSTMENT = 5;
#else
#error "lcd_tx_delay missing!!"
#endif


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void lcd_tunedDelay(uint16_t delay)
{
	uint8_t tmp=0;

	asm volatile("sbiw    %0, 0x01 \n\t"
	"ldi %1, 0xFF \n\t"
	"cpi %A0, 0xFF \n\t"
	"cpc %B0, %1 \n\t"
	"brne .-10 \n\t"
	: "+r" (delay), "+a" (tmp)
	: "0" (delay)
	);
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void lcd_write(uint8_t b)
{
	/* turn off interrupts for a clean txmit */
	uint8_t oldSREG = SREG;
	cli();

	/* Write the start bit */
	LcdPinLow();
	lcd_tunedDelay(lcd_tx_delay + XMIT_START_ADJUSTMENT);

	for (uint8_t mask = 0x01; mask; mask <<= 1)	{
		/* choose bit */
		if (b & mask) {
			/* send 1 */
			LcdPinHigh();
		}
		else {
			/* send 0 */
			LcdPinLow();
		}

		lcd_tunedDelay(lcd_tx_delay);
	}
	/* restore pin to natural state */
	LcdPinHigh();

	/* turn interrupts back on */
	SREG = oldSREG;
	lcd_tunedDelay(lcd_tx_delay);
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void LCD_Init(void)
{
	InitLCD();
	/* clear screen */
	lcd_write(0xFE);
	lcd_write(0x01);
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void LCD_WriteLine(uint8_t line, uint8_t len, char *str)
{
	/* check length is less than 16 */
	if (len > 16 ) return;

	/* set cursor to line */
	lcd_write(0xFE);
	if (line == 0) {
		/* top line */
		lcd_write(0x80);
	}
	else {
		/* bottom line */
		lcd_write(0xC0);
	}

	/* write data */
	for (uint8_t i = 0; i<len; i++) {
		lcd_write(str[i]);
	}
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */


/* EOF */
