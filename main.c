/*
   main.c
	- program entry point and main cycle

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
#include <avr/eeprom.h>
#include "utils.h"
#include "motors.h"
#include "interval_timer.h"
#include "fp_led_controller.h"
#include "fp_buttons.h"

///

#define MOTOR_0_RUN_TIME_MIN 2
#define MOTOR_1_RUN_TIME_MIN 2

///

#define MOTOR_MODE_STOP 0x10
#define MOTOR_MODE_RIGHT 0x11
#define MOTOR_MODE_LEFT 0x12
#define MOTOR_MODE_RIGHT_LEFT 0x13
#define MOTOR_MODE_UNDEFINED 0x14

#define FP_LED_MOTOR_0_MODE_DISABLE_ALL() \
	(fp_switch_led_mode_disable_all(FP_LED_CHANNEL_0))

#define FP_LED_MOTOR_0_STOP_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_0, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_RIGHT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_1, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_LEFT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_2, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_RIGHT_LEFT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_3, FP_LED_ENABLE))


#define FP_LED_MOTOR_0_DELAY_DISABLE_ALL() \
	(fp_switch_led_delay_disable_all(FP_LED_CHANNEL_0))

#define FP_LED_MOTOR_0_DELAY_0_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_4, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_DELAY_1_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_5, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_DELAY_2_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_6, FP_LED_ENABLE))

#define FP_LED_MOTOR_0_DELAY_3_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_0, FP_LED_7, FP_LED_ENABLE))


//

#define FP_LED_MOTOR_1_MODE_DISABLE_ALL() \
	(fp_switch_led_mode_disable_all(FP_LED_CHANNEL_1))

#define FP_LED_MOTOR_1_STOP_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_0, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_RIGHT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_1, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_LEFT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_2, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_RIGHT_LEFT_MODE_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_3, FP_LED_ENABLE))


#define FP_LED_MOTOR_1_DELAY_DISABLE_ALL() \
	(fp_switch_led_delay_disable_all(FP_LED_CHANNEL_1))

#define FP_LED_MOTOR_1_DELAY_0_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_4, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_DELAY_1_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_5, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_DELAY_2_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_6, FP_LED_ENABLE))

#define FP_LED_MOTOR_1_DELAY_3_ENABLE() \
	(fp_switch_led(FP_LED_CHANNEL_1, FP_LED_7, FP_LED_ENABLE))

///

const static uint8_t MOTOR_DELAY_TIMEOUTS[] = { 10, 22, 34, 46, 0 };

//

static uint8_t motor0_started = 0;
static uint8_t motor1_started = 0;

static uint8_t motor_0_current_mode = MOTOR_MODE_STOP;
static uint8_t motor_0_rotation_mode_swings = 0;
static uint8_t motor_0_rotation_mode_swings_state = 0;

static uint8_t motor_0_current_delay_pos = 0;
static uint8_t motor_0_current_delay_timeout = 0;

uint8_t EEMEM EERPOM_MOTOR_0_MODE = MOTOR_MODE_STOP;
uint8_t EEMEM EERPOM_MOTOR_0_DELAY_POS = 0;
uint8_t EEMEM EERPOM_MOTOR_0_DELAY_TIMEOUT = 10;

//

static uint8_t motor_1_current_mode = MOTOR_MODE_STOP;
static uint8_t motor_1_rotation_mode_swings = 0;
static uint8_t motor_1_rotation_mode_swings_state = 0;

static uint8_t motor_1_current_delay_pos = 0;
static uint8_t motor_1_current_delay_timeout = 0;

uint8_t EEMEM EERPOM_MOTOR_1_MODE = MOTOR_MODE_STOP;
uint8_t EEMEM EERPOM_MOTOR_1_DELAY_POS = 0;
uint8_t EEMEM EERPOM_MOTOR_1_DELAY_TIMEOUT = 10;

//

void interval_expired_0(void)
{
	if (motor0_started) {
		stop_motor(MOTOR_0);
		set_interval_0(motor_0_current_delay_timeout, RESET_TIMER);
		motor0_started = 0;
	} else {
		if (motor_0_rotation_mode_swings) {
			if (motor_0_rotation_mode_swings_state == MOTOR_MODE_RIGHT) {
				set_motor_direction(MOTOR_0, MOTOR_DIRECTION_RIGHT);
				motor_0_rotation_mode_swings_state = MOTOR_MODE_LEFT;
			} else {
				set_motor_direction(MOTOR_0, MOTOR_DIRECTION_LEFT);
				motor_0_rotation_mode_swings_state = MOTOR_MODE_RIGHT;
			}
		}

		start_motor(MOTOR_0);
		set_interval_0(MOTOR_0_RUN_TIME_MIN, RESET_TIMER);
		motor0_started = 1;
	}
}

void interval_expired_1(void)
{
	if (motor1_started) {
		stop_motor(MOTOR_1);
		set_interval_1(motor_1_current_delay_timeout, RESET_TIMER);
		motor1_started = 0;
	} else {
		if (motor_1_rotation_mode_swings) {
			if (motor_1_rotation_mode_swings_state == MOTOR_MODE_RIGHT) {
				set_motor_direction(MOTOR_1, MOTOR_DIRECTION_RIGHT);
				motor_1_rotation_mode_swings_state = MOTOR_MODE_LEFT;
			} else {
				set_motor_direction(MOTOR_1, MOTOR_DIRECTION_LEFT);
				motor_1_rotation_mode_swings_state = MOTOR_MODE_RIGHT;
			}
		}

		start_motor(MOTOR_1);
		set_interval_1(MOTOR_1_RUN_TIME_MIN, RESET_TIMER);
		motor1_started = 1;
	}
}

static void set_eeprom_motor_0_mode()
{
	eeprom_write_byte(&EERPOM_MOTOR_0_MODE, motor_0_current_mode);
}

static void set_eeprom_motor_0_delay_pos_timeout()
{
	eeprom_write_byte(&EERPOM_MOTOR_0_DELAY_POS, motor_0_current_delay_pos);
	eeprom_write_byte(&EERPOM_MOTOR_0_DELAY_TIMEOUT, motor_0_current_delay_timeout);
}

static void set_eeprom_motor_1_mode()
{
	eeprom_write_byte(&EERPOM_MOTOR_1_MODE, motor_1_current_mode);
}

static void set_eeprom_motor_1_delay_pos_timeout()
{
	eeprom_write_byte(&EERPOM_MOTOR_1_DELAY_POS, motor_1_current_delay_pos);
	eeprom_write_byte(&EERPOM_MOTOR_1_DELAY_TIMEOUT, motor_1_current_delay_timeout);
}

void motor_0_execute_mode()
{
	FP_LED_MOTOR_0_MODE_DISABLE_ALL();

	switch (motor_0_current_mode) {
		case MOTOR_MODE_STOP:
			set_interval_0(0, RESET_TIMER);

			set_eeprom_motor_0_mode();

			FP_LED_MOTOR_0_STOP_MODE_ENABLE();

			stop_motor(MOTOR_0);
			motor0_started = 0;
			motor_0_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_RIGHT:
			set_interval_0(0, RESET_TIMER);

			set_eeprom_motor_0_mode();

			stop_motor(MOTOR_0);

			FP_LED_MOTOR_0_RIGHT_MODE_ENABLE();
			set_motor_direction(MOTOR_0, MOTOR_DIRECTION_RIGHT);

PORTA ^= (1 << PA0);

			start_motor(MOTOR_0);
			set_interval_0(2, RESET_TIMER);
			motor0_started = 1;
			motor_0_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_LEFT:
			set_interval_0(0, RESET_TIMER);

			set_eeprom_motor_0_mode();

			stop_motor(MOTOR_0);

			FP_LED_MOTOR_0_LEFT_MODE_ENABLE();
			set_motor_direction(MOTOR_0, MOTOR_DIRECTION_LEFT);

			start_motor(MOTOR_0);
			set_interval_0(2, RESET_TIMER);
			motor0_started = 1;
			motor_0_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_RIGHT_LEFT:
			set_interval_0(0, RESET_TIMER);

			set_eeprom_motor_0_mode();

			stop_motor(MOTOR_0);

			FP_LED_MOTOR_0_RIGHT_LEFT_MODE_ENABLE();
			set_motor_direction(MOTOR_0, MOTOR_DIRECTION_RIGHT);

			start_motor(MOTOR_0);
			set_interval_0(2, RESET_TIMER);
			motor0_started = 1;
			motor_0_rotation_mode_swings = 1;
			motor_0_rotation_mode_swings_state = MOTOR_MODE_LEFT;
			break;
	}
}

void motor_0_execute_delay()
{
	FP_LED_MOTOR_0_DELAY_DISABLE_ALL();
	motor_0_current_delay_timeout = MOTOR_DELAY_TIMEOUTS[motor_0_current_delay_pos];

	set_eeprom_motor_0_delay_pos_timeout();

	if (!motor0_started) {
		set_interval_0(motor_0_current_delay_timeout, NO_RESET_TIMER);
	}

	switch (motor_0_current_delay_pos) {
		case 0:
			FP_LED_MOTOR_0_DELAY_0_ENABLE();
			break;

		case 1:
			FP_LED_MOTOR_0_DELAY_1_ENABLE();
			break;

		case 2:
			FP_LED_MOTOR_0_DELAY_2_ENABLE();
			break;

		case 3:
			FP_LED_MOTOR_0_DELAY_3_ENABLE();
			break;
	}
}

void motor_1_execute_mode()
{
	FP_LED_MOTOR_1_MODE_DISABLE_ALL();

	switch (motor_1_current_mode) {
		case MOTOR_MODE_STOP:
			set_interval_1(0, RESET_TIMER);

			set_eeprom_motor_1_mode();

			FP_LED_MOTOR_1_STOP_MODE_ENABLE();

			stop_motor(MOTOR_1);
			motor1_started = 0;
			motor_1_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_RIGHT:
			set_interval_1(0, RESET_TIMER);

			set_eeprom_motor_1_mode();

			stop_motor(MOTOR_1);

			FP_LED_MOTOR_1_RIGHT_MODE_ENABLE();
			set_motor_direction(MOTOR_1, MOTOR_DIRECTION_RIGHT);

			start_motor(MOTOR_1);
			set_interval_1(2, RESET_TIMER);
			motor1_started = 1;
			motor_1_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_LEFT:
			set_interval_1(0, RESET_TIMER);

			set_eeprom_motor_1_mode();

			stop_motor(MOTOR_1);

			FP_LED_MOTOR_1_LEFT_MODE_ENABLE();
			set_motor_direction(MOTOR_1, MOTOR_DIRECTION_LEFT);

			start_motor(MOTOR_1);
			set_interval_1(2, RESET_TIMER);
			motor1_started = 1;
			motor_1_rotation_mode_swings = 0;
			break;

		case MOTOR_MODE_RIGHT_LEFT:
			set_interval_1(0, RESET_TIMER);

			set_eeprom_motor_1_mode();

			stop_motor(MOTOR_1);

			FP_LED_MOTOR_1_RIGHT_LEFT_MODE_ENABLE();
			set_motor_direction(MOTOR_1, MOTOR_DIRECTION_RIGHT);

			start_motor(MOTOR_1);
			set_interval_1(2, RESET_TIMER);
			motor1_started = 1;
			motor_1_rotation_mode_swings = 1;
			motor_1_rotation_mode_swings_state = MOTOR_MODE_LEFT;
			break;
	}
}

void motor_1_execute_delay()
{
	FP_LED_MOTOR_1_DELAY_DISABLE_ALL();
	motor_1_current_delay_timeout = MOTOR_DELAY_TIMEOUTS[motor_1_current_delay_pos];

	set_eeprom_motor_1_delay_pos_timeout();

	if (!motor1_started) {
		set_interval_1(motor_1_current_delay_timeout, NO_RESET_TIMER);
	}

	switch (motor_1_current_delay_pos) {
		case 0:
			FP_LED_MOTOR_1_DELAY_0_ENABLE();
			break;

		case 1:
			FP_LED_MOTOR_1_DELAY_1_ENABLE();
			break;

		case 2:
			FP_LED_MOTOR_1_DELAY_2_ENABLE();
			break;

		case 3:
			FP_LED_MOTOR_1_DELAY_3_ENABLE();
			break;
	}
}

int main()
{
	motor_0_current_mode = eeprom_read_byte(&EERPOM_MOTOR_0_MODE);
	motor_0_current_delay_pos = eeprom_read_byte(&EERPOM_MOTOR_0_DELAY_POS);
	motor_0_current_delay_timeout = eeprom_read_byte(&EERPOM_MOTOR_0_DELAY_TIMEOUT);

	motor_1_current_mode = eeprom_read_byte(&EERPOM_MOTOR_1_MODE);
	motor_1_current_delay_pos = eeprom_read_byte(&EERPOM_MOTOR_1_DELAY_POS);
	motor_1_current_delay_timeout = eeprom_read_byte(&EERPOM_MOTOR_1_DELAY_TIMEOUT);

	init_motors();
	init_fp_led_controller();
	init_fp_buttons();

	init_initerval_timer();

	motor_0_execute_delay();
	motor_1_execute_delay();

	motor_0_execute_mode();
	motor_1_execute_mode();

	while (1) {
		if (is_motor_0_mode_button_pressed()) {
			delay_ms(250);

			if (is_motor_0_mode_button_pressed()) {
				motor_0_current_mode++;

				if (motor_0_current_mode == MOTOR_MODE_UNDEFINED) {
					motor_0_current_mode = MOTOR_MODE_STOP;
				}

				motor_0_execute_mode();
			}
		}

		if (is_motor_0_delay_button_pressed()) {
			delay_ms(250);

			if (is_motor_0_delay_button_pressed()) {
				motor_0_current_delay_pos++;

				if (MOTOR_DELAY_TIMEOUTS[motor_0_current_delay_pos] == 0) {
					motor_0_current_delay_pos = 0;
				}

				motor_0_execute_delay();
			}
		}

		if (is_motor_1_mode_button_pressed()) {
			delay_ms(250);

			if (is_motor_1_mode_button_pressed()) {
				motor_1_current_mode++;

				if (motor_1_current_mode == MOTOR_MODE_UNDEFINED) {
					motor_1_current_mode = MOTOR_MODE_STOP;
				}

				motor_1_execute_mode();
			}
		}

		if (is_motor_1_delay_button_pressed()) {
			delay_ms(250);

			if (is_motor_1_delay_button_pressed()) {
				motor_1_current_delay_pos++;

				if (MOTOR_DELAY_TIMEOUTS[motor_1_current_delay_pos] == 0) {
					motor_1_current_delay_pos = 0;
				}

				motor_1_execute_delay();
			}
		}


		delay_ms(50);
	}

	return 0;
}

