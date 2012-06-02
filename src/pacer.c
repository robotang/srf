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

#include "pacer.h"

#include <avr/interrupt.h>

#define OCR2_MAX            (256-1)
#define OCR2_INCREMENT      78 //approx 78.125

static void pacer_service(void);

static pacer_rate_t val;
static bool update_flag;

void pacer_init(pacer_rate_t rate)
{
    sbi(TIMSK, OCIE2); //Enable Output compare A interrupt

    sbi(TCCR2, CS22); //Prescale 8MHz clock by 1024 -> 7812.5Hz
    sbi(TCCR2, CS21);
    sbi(TCCR2, CS20);
    
    OCR2 = OCR2_INCREMENT;
    
    if(rate == PACER_20HZ)
    {
        val = 5;
    }
    else //default PACER_10HZ
    {
        val = 10;
    }    
    update_flag = false;
}

bool pacer_update(void)
{
    if(update_flag)
    {
        update_flag = false;
        return true;
    }
    return false;
}

static void pacer_service(void)
{
    static uint8_t i = 0;
    i++;
    if(i % val == 0)
    {
        update_flag = true;
        i = 0;
    }
}

ISR(TIMER2_COMP_vect)
{
    pacer_service();
    
    //update compare register
    uint16_t tmp = OCR2;
    tmp += OCR2_INCREMENT;
    if(tmp > OCR2_MAX) tmp -= OCR2_MAX;
    OCR2 = tmp;
}
