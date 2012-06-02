/*
 *  Copyright (C) 2009, Robert Tang <opensource@robotang.co.nz>
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public Licence
 *  along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOARD_H
#define BOARD_H

/* LEDS */

#define LED_DEBUG_DDR       DDRB
#define LED_DEBUG_PORT      PORTB
#define LED_DEBUG_PORTIN    PINB
#define LED_DEBUG_PIN       PB5

/* Serial */

#define RXD_DDR             DDRD
#define RXD_PORT            PORTD
#define RXD_PORTIN          PIND
#define RXD_PIN             PD0

#define TXD_DDR             DDRD
#define TXD_PORT            PORTD
#define TXD_PORTIN          PIND
#define TXD_PIN             PD1

/* I2C bus */

#define SDA_DDR             DDRC
#define SDA_PORT            PORTC
#define SDA_PORTIN          PINC
#define SDA_PIN             PC4

#define SCL_DDR             DDRC
#define SCL_PORT            PORTC
#define SCL_PORTIN          PINC
#define SCL_PIN             PC5

#endif
