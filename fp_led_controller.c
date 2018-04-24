/*
   fp_led_controller.c
	- control front panel leds using two HC595 shift registers

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
#include <util/delay.h>
#include "utils.h"
#include "fp_led_controller.h"

#define HC595_0_SERIAL_DATA PA7
#define HC595_0_REGISTER_LATCH PA6
#define HC595_0_SERIAL_CLOCK PA5

#define RUN_LED_0_PIN PA4

#define HC595_1_SERIAL_DATA PA3
#define HC595_1_REGISTER_LATCH PA2
#define HC595_1_SERIAL_CLOCK PA1

#define RUN_LED_1_PIN PA0

static uint8_t HC595_0_current_reg_state = 0b00000000;
static uint8_t HC595_1_current_reg_state = 0b00000000;

void hc595_clock_pulse(uint8_t pin)
{
	PORTA |= (1 << pin);
	PORTA &= ~(1 << pin);
}

void hc595_latch(uint8_t pin)
{
	PORTA |= (1 << pin);

	_delay_loop_1(1);

	PORTA &= ~(1 << pin);
}

void hc595_write_byte(uint8_t data, uint8_t dt_pin, uint8_t clk_pin, uint8_t lat_pin)
{
	for (uint8_t i = 0; i < 8; i++) {
		if (data & 0b10000000) {
			PORTA |= (1 << dt_pin);
		} else {
			PORTA &= ~(1 << dt_pin);
		}

		hc595_clock_pulse(clk_pin);

		data = data << 1;
	}

	hc595_latch(lat_pin);
}

void fp_switch_led(uint8_t chnum, uint8_t led, uint8_t act)
{
	uint8_t selected_reg = ((chnum == FP_LED_CHANNEL_0)
				? HC595_0_current_reg_state : HC595_1_current_reg_state);

	if (act == FP_LED_ENABLE) {
		selected_reg |= led;
	} else {
		selected_reg &= ~led;
	}

	if (chnum == FP_LED_CHANNEL_0) {
		hc595_write_byte(selected_reg, HC595_0_SERIAL_DATA, HC595_0_SERIAL_CLOCK, HC595_0_REGISTER_LATCH);
		HC595_0_current_reg_state = selected_reg;
	} else {
		hc595_write_byte(selected_reg, HC595_1_SERIAL_DATA, HC595_1_SERIAL_CLOCK, HC595_1_REGISTER_LATCH);
		HC595_1_current_reg_state = selected_reg;
	}
}

void fp_renew_led_states()
{
	hc595_write_byte(HC595_0_current_reg_state, HC595_0_SERIAL_DATA, HC595_0_SERIAL_CLOCK, HC595_0_REGISTER_LATCH);
	hc595_write_byte(HC595_1_current_reg_state, HC595_1_SERIAL_DATA, HC595_1_SERIAL_CLOCK, HC595_1_REGISTER_LATCH);
}

static void fp_disable_leds_by_mask(uint8_t chnum, uint8_t mask)
{
	uint8_t selected_reg = ((chnum == FP_LED_CHANNEL_0)
				? HC595_0_current_reg_state : HC595_1_current_reg_state);

	selected_reg &= ~mask;

	if (chnum == FP_LED_CHANNEL_0) {
		hc595_write_byte(selected_reg, HC595_0_SERIAL_DATA, HC595_0_SERIAL_CLOCK, HC595_0_REGISTER_LATCH);
		HC595_0_current_reg_state = selected_reg;
	} else {
		hc595_write_byte(selected_reg, HC595_1_SERIAL_DATA, HC595_1_SERIAL_CLOCK, HC595_1_REGISTER_LATCH);
		HC595_1_current_reg_state = selected_reg;
	}

}

void fp_switch_led_mode_disable_all(uint8_t chnum)
{
	fp_disable_leds_by_mask(chnum, 0b00001111);
}

void fp_switch_led_delay_disable_all(uint8_t chnum)
{
	fp_disable_leds_by_mask(chnum, 0b11110000);
}

void hello_led_pattern()
{
	uint8_t initial_pattern_ch1 = 0b11111111;
	uint8_t initial_pattern_ch2 = 0b11111111;

	hc595_write_byte(initial_pattern_ch1, HC595_0_SERIAL_DATA, HC595_0_SERIAL_CLOCK, HC595_0_REGISTER_LATCH);
	hc595_write_byte(initial_pattern_ch2, HC595_1_SERIAL_DATA, HC595_1_SERIAL_CLOCK, HC595_1_REGISTER_LATCH);

	for (uint8_t i = 0; i<8; i++) {
		delay_ms(100);

		initial_pattern_ch1 = initial_pattern_ch1 >> 1;
		initial_pattern_ch2 = initial_pattern_ch2 << 1;

		hc595_write_byte(initial_pattern_ch1, HC595_0_SERIAL_DATA, HC595_0_SERIAL_CLOCK, HC595_0_REGISTER_LATCH);
		hc595_write_byte(initial_pattern_ch2, HC595_1_SERIAL_DATA, HC595_1_SERIAL_CLOCK, HC595_1_REGISTER_LATCH);

	}
}

void init_fp_led_controller()
{
	DDRA |= (1 << HC595_0_SERIAL_DATA);
	DDRA |= (1 << HC595_0_REGISTER_LATCH);
	DDRA |= (1 << HC595_0_SERIAL_CLOCK);
	DDRA |= (1 << RUN_LED_0_PIN);
	DDRA |= (1 << HC595_1_SERIAL_DATA);
	DDRA |= (1 << HC595_1_REGISTER_LATCH);
	DDRA |= (1 << HC595_1_SERIAL_CLOCK);
	DDRA |= (1 << RUN_LED_1_PIN);

	HC595_0_current_reg_state = 0b00000000;
	HC595_1_current_reg_state = 0b00000000;

	hello_led_pattern();
}

