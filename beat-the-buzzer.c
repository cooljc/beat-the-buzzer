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
#include "common.h"
#include "timer0.h"
#include "button-driver.h"
#include "lcd-driver.h"
#include "speaker-driver.h"

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
typedef uint8_t (*func_p)(uint8_t);

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
uint8_t topMenu(uint8_t key);
uint8_t gameRunning(uint8_t key);
uint8_t gameOver(uint8_t key);
enum {
	ST_TOP_MENU = 0,
	ST_GAME_RUNNING,
	ST_GAME_OVER,
	ST_MAX
};
func_p states[ST_MAX] = {topMenu, gameRunning, gameOver};


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
#define Clear_prescaler() (CLKPR = (1<<CLKPCE),CLKPR = 0)

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
uint8_t topMenu(uint8_t key)
{
	static int enter = 1;
	
	if (enter) {
		LCD_WriteLine(0, 16, "Beat The Buzzer ");
		LCD_WriteLine(1, 16, "Press Start     ");
		enter = 0;
	}
	
	if (key == KEY_START) {
		enter = 1;
		return ST_GAME_RUNNING;
	}
	return ST_TOP_MENU;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
uint8_t gameRunning(uint8_t key)
{
	static int enter = 1;
	
	if (enter) {
		LCD_WriteLine(0, 16, "Beat The Buzzer ");
		LCD_WriteLine(1, 16, "Lives Left:     ");
		enter = 0;
	}
	
	if (key == KEY_RESET) {
		enter = 1;
		return ST_TOP_MENU;
	}
	else if (key == KEY_LOOP) {
		// TODO: Sound buzzer
	}
	return ST_GAME_RUNNING;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
uint8_t gameOver(uint8_t key)
{
	return ST_GAME_OVER;
}


/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
int main (void)
{
	uint8_t state = ST_TOP_MENU;
	uint8_t nextstate = ST_TOP_MENU;
	uint8_t key = KEY_NONE;
	func_p pStateFunc = states[state];
	
	
	/* disable watchdog */
	wdt_reset();
	Wdt_clear_flag();
	Wdt_change_enable();
	Wdt_stop();
		
	Clear_prescaler();
	InitLED();
	Led1On();
	
	Timer0_Init();
	SPEAKER_Init();
	BUTTON_Init();
	LCD_Init();
	
	sei();
	
	while (1)
	{
		key = BUTTON_GetKey();
		nextstate = pStateFunc(key);
		
		if (nextstate != state) {
			pStateFunc = states[nextstate];
			state = nextstate;
		}
		
	} /* end of while(1) */
	return 0;
}


/* EOF */
