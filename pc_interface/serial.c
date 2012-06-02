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

#include "serial.h"

#include "rprintf.h"
#include "uart.h"

// initialise the serial LCD module
void serial_init(void)
{
    uartInit();    
    uartSetBaudRate(115200);
    rprintfInit(uartSendByte);
    //serial_clear();
}

void serial_clear(void)
{    
    uint8_t i;    
    //Make sure serial port is cleared!
    for(i = 0; i < 4; i++)
    {
        rprintf("\x1B[2J");
        rprintf("\x1B[%d;%dH", 0, 0);
    }
}
