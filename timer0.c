/*
 * Filename		: timer0.h
 * Author		: Jon Cross
 * Date			: 18/08/2012
 * Description	: Generic timer.
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

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "timer0.h"

TIMER_CALLBACK_FUNC CallbackFunc[TIMER0_NUM_CALLBACKS];

/* ------------------------------------------------------------------ *
 * Value definition:
 * 0      The timer has expired
 * 1-254  The timer is counting down
 * 255    Free timer
 * ------------------------------------------------------------------ */
uint8_t CountDownTimers[TIMER0_NUM_COUNTDOWNTIMERS];

/* ------------------------------------------------------------------ *
 *
 *   Function name : Timer0_Init
 *   Returns :       None
 *   Parameters :    None
 *   Purpose :       Initialize Timer/Counter 0
 *
 * ------------------------------------------------------------------ */
void Timer0_Init(void)
{
    uint8_t i;

    // Initialize array of callback functions
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
        CallbackFunc[i] = NULL;

    // Initialize countdown timers
    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        CountDownTimers[i] = 255;


    // Initialize Timer0.
    // Used to give the correct time-delays in the song

    // Enable timer0 compare interrupt
    TIMSK0 = (1<<OCIE0A);

    // Sets the compare value
    OCR0A = 38;

    // Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
    TCCR0A = (1<<WGM01)|(0<<WGM00)|(4<<CS00);
}


/* ------------------------------------------------------------------ *
 *
 *   Function name : TIMER0_COMP_interrupt
 *   Returns :       None
 *   Parameters :    None
 *   Purpose :       Check if any functions are to be called
 *
 * ------------------------------------------------------------------ */
ISR(TIMER0_COMPA_vect)
{
    uint8_t i;

    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
        if (CallbackFunc[i] != NULL)
            CallbackFunc[i]();

    // Count down timers
    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        if (CountDownTimers[i] != 255 && CountDownTimers[i] != 0)
            CountDownTimers[i]--;

}


/* ------------------------------------------------------------------ *
 *
 *   Function name : Timer0_RegisterCallbackFunction
 *   Returns :       None
 *   Parameters :    pFunc
 *   Purpose :       Set up functions to be called from the
 *                   TIMER0_COMP_interrupt
 *
 * ------------------------------------------------------------------ */
bool Timer0_RegisterCallbackFunction(TIMER_CALLBACK_FUNC pFunc)
{
    uint8_t i;

    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == pFunc)
            return TRUE;
    }

    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == NULL)
        {
            CallbackFunc[i] = pFunc;
            return TRUE;
        }
    }

    return FALSE;
}


/* ------------------------------------------------------------------ *
 *
 *   Function name : Timer0_RemoveCallbackFunction
 *   Returns :       None
 *   Parameters :    pFunc
 *   Purpose :       Remove functions from the list which is called int the
 *                   TIMER0_COMP_interrupt
 *
 * ------------------------------------------------------------------ */
bool Timer0_RemoveCallbackFunction(TIMER_CALLBACK_FUNC pFunc)
{
    uint8_t i;

    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == pFunc)
        {
            CallbackFunc[i] = NULL;
            return TRUE;
        }
    }

    return FALSE;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
char Timer0_AllocateCountdownTimer()
{
    uint8_t i;

    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        if (CountDownTimers[i] == 255)
        {
            CountDownTimers[i] = 0;
            return i+1;
        }

    return 0;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void Timer0_SetCountdownTimer(char timer, char value)
{
    cli();
    CountDownTimers[timer-1] = value;
    sei();
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
char Timer0_GetCountdownTimer(char timer)
{
    char t;
    cli();
    t = CountDownTimers[timer-1];
    sei();

    return t;
}

/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
void Timer0_ReleaseCountdownTimer(char timer)
{
    cli();
    CountDownTimers[timer-1] = 255;
    sei();
}

/* EOF */
