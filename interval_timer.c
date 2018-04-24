/*
   interval_timer.c
	- implementation of the motors interval timer used
      for working and delay cycles

   Copyright 2018  Oleg Kutkov <elenbert@gmail.com>
                   Crimean astrophysical observatory

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "interval_timer.h"

static int seconds_counter_0 = 0;
static int minutes_counter_0 = 0;

static int seconds_counter_1 = 0;
static int minutes_counter_1 = 0;

static int target_minutes_0 = 0;
static int target_minutes_1 = 0;

void interval_expired_0(void);
void interval_expired_1(void);


static void init_timer()
{
	TCCR1A = (1 << WGM12);
	TCCR1B = (1 << CS12);

	TIMSK = (1 << OCIE1A);

	TCNT1 = 0;

	OCR1A = 31250;
}

ISR (TIMER1_COMPA_vect)
{
	seconds_counter_0++;
	seconds_counter_1++;

	if (seconds_counter_0 == 60) {
		PORTA ^= (1 << PA0);

		++minutes_counter_0;
		seconds_counter_0 = 0;

		if (minutes_counter_0 == target_minutes_0) {
			minutes_counter_0 = 0;

			interval_expired_0();
		}
		
	}

	if (seconds_counter_1 == 60) {
		++minutes_counter_1;
		seconds_counter_1 = 0;

		if (minutes_counter_1 == target_minutes_1) {
			minutes_counter_1 = 0;

			interval_expired_1();
		}
		
	}

	TCNT1 = 0;
}

void init_initerval_timer()
{
	init_timer();

	sei();
}

void set_interval_0(const int interval_min, uint8_t reset)
{
	if (reset == RESET_TIMER) {
		seconds_counter_0 = 0;
		minutes_counter_0 = 0;
	}

	target_minutes_0 = interval_min;
}

void set_interval_1(const int inteval_min, uint8_t reset)
{
	if (reset == RESET_TIMER) {
		seconds_counter_1 = 0;
		minutes_counter_1 = 0;
	}

	target_minutes_1 = inteval_min;
}

