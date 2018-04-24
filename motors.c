/*
   motors.c
	- implementation of the two channel PWM driver

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
#include "utils.h"
#include "motors.h"

#define MOTOR_0_PWM_PIN PB3
#define MOTOR_0_DIR_0_PIN PD3
#define MOTOR_0_DIR_1_PIN PD4

#define MOTOR_0_PWM_TIMER_MAX 90

#define MOTOR_1_PWM_PIN PD7
#define MOTOR_1_DIR_0_PIN PD6
#define MOTOR_1_DIR_1_PIN PD5

#define MOTOR_1_PWM_TIMER_MAX 90

static void configure_motor_output()
{
	DDRB |= (1 << MOTOR_0_PWM_PIN);
	DDRD |= (1 << MOTOR_0_DIR_0_PIN);
	DDRD |= (1 << MOTOR_0_DIR_1_PIN);

	DDRD |= (1 << MOTOR_1_PWM_PIN);
	DDRD |= (1 << MOTOR_1_DIR_0_PIN);
	DDRD |= (1 << MOTOR_1_DIR_1_PIN);
}

static void configure_timers()
{
	TCCR0 = (1 << COM01) | (1 << WGM00);
	TCCR0 |= (1 << CS01) | (1 << CS00);

	OCR0 = 0;


	TCCR2 = (1 << COM21) | (1 << WGM20);
	TCCR2 |= (1 << CS22);

	OCR2 = 0;
}

void init_motors()
{
	configure_motor_output();
	configure_timers();
}

void set_motor_direction(const char motor_id, const char motor_dir)
{
	if (motor_id == MOTOR_0) {
		if (motor_dir == MOTOR_DIRECTION_RIGHT) {
			PORTD |= (1 << MOTOR_0_DIR_0_PIN);
			PORTD &= ~(1 << MOTOR_0_DIR_1_PIN);
		} else {
			PORTD |= (1 << MOTOR_0_DIR_1_PIN);
			PORTD &= ~(1 << MOTOR_0_DIR_0_PIN);
		}
	} else {
		if (motor_dir == MOTOR_DIRECTION_RIGHT) {
			PORTD |= (1 << MOTOR_1_DIR_0_PIN);
			PORTD &= ~(1 << MOTOR_1_DIR_1_PIN);
		} else {
			PORTD |= (1 << MOTOR_1_DIR_1_PIN);
			PORTD &= ~(1 << MOTOR_1_DIR_0_PIN);
		}
	}
}

void start_motor(const char motor_id)
{
	if (motor_id == MOTOR_0) {
		OCR0 = MOTOR_0_PWM_TIMER_MAX;
	} else {
		OCR2 = MOTOR_1_PWM_TIMER_MAX;
	}
}

void stop_motor(const char motor_id)
{
	if (motor_id == MOTOR_0) {
		OCR0 = 0;
	} else {
		OCR2 = 0;
	}
}

