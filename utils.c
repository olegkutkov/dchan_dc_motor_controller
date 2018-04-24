/*
   utils.c
	- misc stuff

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

#include <util/delay.h> 

void delay_ms(uint16_t count)
{
	while (count--) {
		_delay_ms(1);
	}
}

void delay_us(uint16_t count)
{
	while(count--) {
		_delay_us(1);
	}
}

