/*
 * Filename		: beat-the-buzzer.c
 * Author		: Jon Cross
 * Date			: 17/08/2012
 * Description	: Main entry point and control loop beat the buzzer game.
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
 * 
 */

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
#include "wdt_drv.h"
#include "lcd-driver.h"

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

// LEDs on POP-168 board: PD2 (Di2) & PD4 (Di4) - Tided high
// Switches on POP-168 board: PD2 (Di2) & PD4 (Di4) 
/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */

#define LED1      		(1 << PD2)
#define LED2			(1 << PD4)
#define InitLED()		(DDRD |= (LED1 | LED2))
#define Led1On()		(PORTD |= LED1)
#define Led1Off()		(PORTD &= ~LED1)
#define Led1Toggle()	(PIND |= LED1)
#define Led2On()		(PORTD |= LED2)
#define Led2Off()		(PORTD &= ~LED2)
#define Led2Toggle()	(PIND |= LED2)




/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
#define Clear_prescaler() (CLKPR = (1<<CLKPCE),CLKPR = 0)

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
int main (void)
{
	
	/* disable watchdog */
	wdt_reset();
	Wdt_clear_flag();
	Wdt_change_enable();
	Wdt_stop();
		
	Clear_prescaler();

	InitLED();
	Led1On();
	
	LCD_Init();
	LCD_WriteLine(0, 11, "Hello World");
	LCD_WriteLine(1, 15, "Beat The Buzzer");
		
	
	while (1)
	{
		_delay_ms(1000);
		Led1Off();
		Led2On();
		_delay_ms(1000);
		Led1On();
		Led2Off();
		
	} /* end of while(1) */
	return 0;
}


/* EOF */
