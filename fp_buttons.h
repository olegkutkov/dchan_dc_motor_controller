/*
   fp_buttons.h
	- front pannel buttons functionality

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

#ifndef FP_BUTTONS_H
#define FP_BUTTONS_H

void init_fp_buttons();

uint8_t is_motor_0_mode_button_unpressed();
uint8_t is_motor_0_mode_button_pressed();
uint8_t is_motor_0_delay_button_unpressed();
uint8_t is_motor_0_delay_button_pressed();
uint8_t is_motor_1_mode_button_unpressed();
uint8_t is_motor_1_mode_button_pressed();
uint8_t is_motor_1_delay_button_unpressed();
uint8_t is_motor_1_delay_button_pressed();

#endif

