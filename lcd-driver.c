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


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

/* ------------------------------------------------------------------ */
/* LCD PORT/PIN */
/* ------------------------------------------------------------------ */
#define LCDPIN			(1 << PC2)
#define InitLCD()		(DDRC |= LCDPIN)
#define LcdPinHigh()	(PORTC |= LCDPIN)	
#define LcdPinLow()		(PORTC &= ~LCDPIN)

/* ------------------------------------------------------------------ */
/* TX Delay (F_CPU = 16MHz, Baud = 9600) */
/* ------------------------------------------------------------------ */
#if F_CPU == 16000000
uint16_t lcd_tx_delay = 233;
const int XMIT_START_ADJUSTMENT = 5;
#else
#error "lcd_tx_delay missing!!"
#endif

/* ------------------------------------------------------------------ */
/* if there is a transister or something that inverts the pin set 
 * this 1 */
/* ------------------------------------------------------------------ */

uint16_t lcd_inverse_logic = 0;

/* ------------------------------------------------------------------ */
/* BIT state */
/* ------------------------------------------------------------------ */
enum {
	LOW = 0,
	HIGH
};

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
inline void lcd_tunedDelay(uint16_t delay) { 
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
void lcd_txPinWrite(uint8_t pin_state)
{
  if (pin_state == LOW)
    LcdPinLow();
  else
    LcdPinHigh();
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
size_t lcd_write(uint8_t b)
{
  if (lcd_tx_delay == 0) {
    //setWriteError();
    return 0;
  }

  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  lcd_txPinWrite(lcd_inverse_logic ? HIGH : LOW);
  lcd_tunedDelay(lcd_tx_delay + XMIT_START_ADJUSTMENT);

  // Write each of the 8 bits
  if (lcd_inverse_logic)
  {
    for (uint8_t mask = 0x01; mask; mask <<= 1)
    {
      if (b & mask) // choose bit
        lcd_txPinWrite(LOW); // send 1
      else
        lcd_txPinWrite(HIGH); // send 0
    
      lcd_tunedDelay(lcd_tx_delay);
    }

    lcd_txPinWrite(LOW); // restore pin to natural state
  }
  else
  {
    for (uint8_t mask = 0x01; mask; mask <<= 1)
    {
      if (b & mask) // choose bit
        lcd_txPinWrite(HIGH); // send 1
      else
        lcd_txPinWrite(LOW); // send 0
    
      lcd_tunedDelay(lcd_tx_delay);
    }

    lcd_txPinWrite(HIGH); // restore pin to natural state
  }

  SREG = oldSREG; // turn interrupts back on
  lcd_tunedDelay(lcd_tx_delay);
  
  return 1;
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
