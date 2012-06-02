/*
 *  A DIY ultrasonic rangefinder module
 *
 *  Copyright (C) 2008, Robert Tang <opensource@robotang.co.nz>
 *  COpyright (C) 2008, krw42
 *
 *  To read fuse bits:
 *      avrdude -p m8 -c stk500v2 -P com8 -U lfuse:r:con:h -U hfuse:r:con:h   
 *  To change fuse bits to 8MHz internal oscillator:
 *      avrdude -p m8 -c stk500v2 -P com8 -U lfuse:w:0xE4:m
 *  To change fuse bits to save EEPROM:
 *      avrdude -p m8 -c stk500v2 -P com8 -U hfuse:w:0xD1:m 
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

#include <stdlib.h>
#include <avr/io.h>
#include "global.h"
#include "delay.h"
#include "pacer.h"
#include "sonar.h"
#include "i2c_interface.h"

extern bool automatic_ranging;

int main(void)
{
    cli();
    sonar_init();
    i2c_interface_init();
    pacer_init(PACER_20HZ);
    sei();
    
    while(1)
    {
        if(automatic_ranging)
        {
            if(pacer_update())
            {
                sonar_measure();
            }
        }
        else
        {        
            switch(i2c_interface_update())
            {        
                case STATUS_MEASURE:
                {
                    sonar_measure();            
                }
                break;
                
                case STATUS_PING:
                {
                    sonar_ping();
                }
                break;
                
                case STATUS_LISTEN:
                {
                    sonar_listen();
                }
                break;
                
                case STATUS_CALIBRATE:
                {
                    TCNT1 = 0;
                    while(TCNT1 < (65000));
                    sonar_calibrate();
                }
                break;    
                
                default:
                {
                    //do nothing
                }
                break;
            }
        }
    }

    return 0;
}
