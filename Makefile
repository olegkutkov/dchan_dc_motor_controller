#
# Makefile
#
#   Copyright 2018  Oleg Kutkov <elenbert@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.  
#

PROGRAM = motor_controller

CC := avr-gcc
OBJCOPY := avr-objcopy

CFLAGS := -mmcu=atmega32a -Os -DF_CPU=8000000 -DRTS_ENABLE -std=c99 -Wall -gstabs -Wall

SRC := main.c motors.c utils.c interval_timer.c fp_led_controller.c fp_buttons.c

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(PROGRAM)
	$(OBJCOPY) -j .text -j .data -R .eeprom -O ihex $(PROGRAM) $(PROGRAM).hex
	$(OBJCOPY) -O ihex -j .eeprom --change-section-lma .eeprom=0 $(PROGRAM) $(PROGRAM).eep

readfuse:
	avrdude -p m32 -c usbasp -U lfuse:r:-:i -U hfuse:r:-:i -U efuse:r:-:i -U lock:r:-:i

writefuse:
	avrdude -p m32 -c usbasp -U lfuse:w:0xff:m -U hfuse:w:0xd9:m 

upload:
	avrdude -p m32 -c usbasp -U flash:w:$(PROGRAM).hex
	avr-size -C --mcu=atmega32a $(PROGRAM)

upload-eeprom:
	avrdude -p m32 -c usbasp -U eeprom:w:$(PROGRAM).eep

clean:
	rm -fr $(PROGRAM) $(PROGRAM).o $(PROGRAM).hex

