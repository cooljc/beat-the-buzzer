/*
 * Filename		: timer0.h
 * Author		: Jon Cross
 * Date			: 18/08/2012
 * Description	: Driver for the keys and game loop..
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
#ifndef _TIMER0_H
#define _TIMER0_H

/* Typedef for Timer callback function */
typedef void (*TIMER_CALLBACK_FUNC) (void);

#define TIMER0_NUM_CALLBACKS        4
#define TIMER0_NUM_COUNTDOWNTIMERS  4

void Timer0_Init(void);

bool Timer0_RegisterCallbackFunction(TIMER_CALLBACK_FUNC pFunc);
bool Timer0_RemoveCallbackFunction(TIMER_CALLBACK_FUNC pFunc);

/* char Timer0_AllocateCountdownTimer(); */
char Timer0_AllocateCountdownTimer(void);
char Timer0_GetCountdownTimer(char timer);
void Timer0_SetCountdownTimer(char timer, char value);
void Timer0_ReleaseCountdownTimer(char timer);

#endif /* #ifndef _TIMER0_H */

/* EOF */
