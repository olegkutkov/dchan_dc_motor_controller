/*
   motors.h
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

#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR_0 0x0
#define MOTOR_1 0x1

#define MOTOR_DIRECTION_RIGHT 0x0
#define MOTOR_DIRECTION_LEFT 0x1

void init_motors();

void set_motor_direction(const char motor_id, const char motor_dir);
void start_motor(const char motor_id);
void stop_motor(const char motor_id);

#endif

