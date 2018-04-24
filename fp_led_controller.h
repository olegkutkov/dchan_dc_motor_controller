/*
   fp_led_controller.h
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

#ifndef FP_LED_CONTROLLER_H
#define FP_LED_CONTROLLER_H

#define FP_LED_CHANNEL_0 0x0
#define FP_LED_CHANNEL_1 0x1

#define FP_LED_0 0x1
#define FP_LED_1 0x2
#define FP_LED_2 0x4
#define FP_LED_3 0x8
#define FP_LED_4 0x10
#define FP_LED_5 0x20
#define FP_LED_6 0x40
#define FP_LED_7 0x80

#define FP_LED_DISABLE 0x0
#define FP_LED_ENABLE 0x1

void init_fp_led_controller();

void fp_switch_led(uint8_t chnum, uint8_t led, uint8_t act);

void fp_renew_led_states();

void fp_switch_led_mode_disable_all(uint8_t chnum);
void fp_switch_led_delay_disable_all(uint8_t chnum);

#endif

