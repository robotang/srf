/*
 *  PC interface to srf
 *
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

#include <stdio.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#include "global.h"
#include "rtc.h"
#include "serial.h"
#include "rprintf.h"
#include "uart.h"
#include "a2d.h"

#include "srf.h"

static void main_init(void);
static void main_periodic_1Hz(void);
static void main_periodic_10Hz(void);
static void main_periodic_100Hz(void);
static void main_periodic_1000Hz(void);
static void main_event(void);  

bool auto_range = false;

int main(void)
{
    cli();
    main_init();
    sei();
    
    uint16_t counter = 0;    
    
    while(true)
    {        
        if(rtc_update_ms())
        {
            main_periodic_1000Hz();            
            counter++;
            if(counter % 10 == 0)
            {
                main_periodic_100Hz();
            }
            if(counter % 100 == 0)
            {                
                main_periodic_10Hz();
            }
            if(counter > 1000)
            {                
                counter = 0;
                main_periodic_1Hz();
            }
        }
        
        main_event();
    }
   
    return(0);
}

static void main_init(void)
{
    LED_DEBUG_INIT();
    
    serial_init();
    a2dInit();
    rtc_init();
    srf_init();
}

static void main_periodic_1Hz(void)
{
    LED_DEBUG_TOGGLE;    

    uint8_t c;
    if(uartReceiveByte(&c))
    {
        if(c == 'c')
            srf_calibrate();
        else if(c == 'r')
            srf_read_raw();
        else if(c == 'd')
        {
            srf_measure();
            uint16_t d = srf_read();
            rprintf("distance = %d\r\n", d);
        }
        else if(c == 'a')
        {
            auto_range = (auto_range) ? false : true;
            srf_auto_range(auto_range);
        }
    }
}

static void main_periodic_10Hz(void)
{
    /*uint16_t distance_cm = a2dConvert10bit(0);
    distance_cm *= 127; //approx to 1.2655...
    distance_cm /= 100;
    rprintf("%d\r\n", distance_cm);*/
    
    if(auto_range)
    {
        uint16_t d = srf_read();
        rprintf("distance = %d\r\n", d);
    }
}

static void main_periodic_100Hz(void)
{

}

static void main_periodic_1000Hz(void)
{

}

static void main_event(void)
{

}
