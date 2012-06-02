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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timerx8.h"
#include "rtc.h"

#define OCR2A_INCREMENT        250
#define OCR2A_MAX            MAX_U08

static volatile uint32_t rtc_time_ms;
static volatile bool update_ms_flag;

void rtc_1ms_service(void);

void rtc_init(void)
{
    sbi(TIMSK2, OCIE2A); //Enable Output compare A interrupt
    timer2SetPrescaler(TIMERRTC_CLK_DIV64); //Prescale 16MHz clock by 64.
    timerAttach(TIMER2OUTCOMPARE_INT, rtc_1ms_service);
    rtc_reset();
}

void rtc_reset(void)
{
    rtc_time_ms = 0;
    update_ms_flag = false;
    TCNT2 = 0;
    OCR2A = OCR2A_INCREMENT;
}

uint32_t rtc_uptime_ms(void)
{
    return rtc_time_ms;
}

bool rtc_update_ms(void)
{
    if(update_ms_flag)
    {
        update_ms_flag = false;
        return true;    
    }
    return false;
}

void rtc_1ms_service(void) //This should be called every 1ms
{
    rtc_time_ms++;
    update_ms_flag = true;
    //update compare register
    uint16_t tmp = OCR2A;
    tmp += OCR2A_INCREMENT;
    if(tmp >= OCR2A_MAX) tmp -= OCR2A_MAX;
    OCR2A = tmp;
}
