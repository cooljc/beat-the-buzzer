/*
 * Filename		: speaker-driver.c
 * Author		: Jon Cross
 * Date			: 18/08/2012
 * Description	: Driver for the piezo..
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>

#include "common.h"
#include "timer0.h"
#include "speaker-driver.h"

/* ------------------------------------------------------------------ *
 * Piezo PIN = PORTC0
 * ------------------------------------------------------------------ */
#define SP_PORT	PORTC
#define SP_DDR	DDRC
#define SP_BIT	(1 << PC0)


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void SPEAKER_Init(void)
{
	/* set piezo pin to output */
	SP_DDR |= SP_BIT;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void SPEAKER_Beep(void)
{
	unsigned char i;
	for (i = 0; i < 100; i++) {
		SP_PORT |= SP_BIT;
		_delay_ms(1);
		SP_PORT &= ~SP_BIT;	
		_delay_ms(1);
	}
}
/* EOF */
