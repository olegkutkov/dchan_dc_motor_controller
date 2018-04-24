/*
   interval_timer.h
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


#ifndef INTERVAL_TIMER_H
#define INTERVAL_TIMER_H

#define NO_RESET_TIMER 0x0
#define RESET_TIMER 0x1

void init_initerval_timer();
void set_interval_0(const int interval_min, uint8_t reset);
void set_interval_1(const int inteval_min, uint8_t reset);

#endif

