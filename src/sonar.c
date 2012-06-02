/*
 *  Copyright (C) 2008, Robert Tang <opensource@robotang.co.nz>
 *  COpyright (C) 2008, krw42
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

#include "sonar.h"
#include "a2d.h"
#include "filter.h"
#include "sonar_threshold.h"

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define EXTRA_DELAY             asm volatile("nop")

#define NUM_PULSES              13
#define HALF_PERIOD             11 //40ish kHz
#define US_PER_LEVEL            100
#define LEVEL_OFFSET            350
#define BLANKING_TIME_US        220
#define SOUND_MS                340

#define START_INDEX             7
#define MIN_PULSE_WIDTH         2
#define MAX_NUM_PULSES          5

typedef struct
{
    uint16_t peak_index;
    uint16_t width;
} pulse_t;

uint8_t calibration[NUM_LEVELS];
uint8_t measure[NUM_LEVELS];
uint16_t distance = 0, distance_filtered = 0;
static filter_t filter;
static pulse_t pulse[MAX_NUM_PULSES];
static uint8_t num_pulses;

static void load_calibration(void);
static void find_pulses(void);
static pulse_t find_pulse(uint16_t *index);

void sonar_init(void)
{
    sbi(STX_A_DDR, STX_A_PIN);
    sbi(STX_B_DDR, STX_B_PIN);
    cbi(SRX_PORT, SRX_PIN);

    sbi(TCCR1B, CS11); //Prescale 8MHz clock by 8 to provide 1us resolution
    a2dInit();    
    a2dSetChannel(SRX_ADC_CH);
    a2dSetPrescaler(ADC_PRESCALE_DIV8); 
    
    load_calibration();
    filter_init(&filter, 10);
}

void sonar_measure(void)
{    
    sonar_ping();
    sonar_listen();
}

void sonar_ping(void)
{
    sbi(STX_B_DDR, STX_B_PIN);
    for(uint8_t i = 0; i < NUM_PULSES; i++) 
    {
        TCNT1 = 0;
        sbi(STX_A_PORT, STX_A_PIN);
        cbi(STX_B_PORT, STX_B_PIN);
        while(TCNT1 < HALF_PERIOD);
        EXTRA_DELAY;
        EXTRA_DELAY;
        
        TCNT1 = 0;
        cbi(STX_A_PORT, STX_A_PIN);
        sbi(STX_B_PORT, STX_B_PIN);
        while(TCNT1 < HALF_PERIOD);
        EXTRA_DELAY;
    }
    cbi(STX_A_PORT, STX_A_PIN);
    cbi(STX_B_PORT, STX_B_PIN);
    
    sbi(PORTD, PD1);
    TCNT1 = 0;    
    while(TCNT1 < BLANKING_TIME_US); //short +Sens and -Sens
    
    //Make B input
    cbi(STX_B_DDR, STX_B_PIN);
    cbi(PORTD, PD1);
}

void sonar_listen(void)
{
    /* Capture data */
    TCNT1 = 0;
    for(uint16_t i = 0; i < NUM_LEVELS; i++) 
    {
        a2dStartConvert();
        while(TCNT1 < US_PER_LEVEL*(i+1));
        while(bit_is_set(ADCSR, ADSC)); //wait until conversion complete
        uint16_t tmp = BIND(LEVEL_OFFSET, (inp(ADCL) | (inp(ADCH)<<8)), MAX_U08 + LEVEL_OFFSET); //prevent overflow
        tmp -= LEVEL_OFFSET;
        measure[i] = (uint8_t) tmp;
    }
    
    /* Process data */
    find_pulses();
    
    /* Estimate distance */
    distance = 0;
    if(num_pulses > 0)
    {
        uint16_t index = pulse[0].peak_index; //for now, just use the first valid pulse
        uint32_t tmp = 17 * (BLANKING_TIME_US + 100*index); 
        tmp /= 1000;
        distance = (uint16_t) tmp;
    }
    distance_filtered = filter_avg(&filter, distance);
}

void sonar_calibrate(void)
{    
    sonar_ping();

    /* Capture data */
    TCNT1 = 0;
    for(uint16_t i = 0; i < NUM_LEVELS; i++) 
    {        
        a2dStartConvert();
        while(TCNT1 < US_PER_LEVEL*(i+1));
        while(bit_is_set(ADCSR, ADSC)); //wait until conversion complete        
        uint16_t tmp = BIND(LEVEL_OFFSET, (inp(ADCL) | (inp(ADCH)<<8)), MAX_U08 + LEVEL_OFFSET); //prevent overflow
        tmp -= LEVEL_OFFSET;
        calibration[i] = (uint8_t) tmp;
    }
    
    for(uint16_t i = 0; i < NUM_LEVELS; i++) 
    {
        eeprom_write_byte((uint8_t *) i, calibration[i]);
    }
}

/* 
 * Private functions
 */

static void load_calibration(void)
{
    for(uint16_t i = 0; i < NUM_LEVELS; i++)
    {
        calibration[i] = eeprom_read_byte((const uint8_t *) i);
    }
}

static void find_pulses(void)
{
    uint16_t index = START_INDEX;
    num_pulses = 0;
    
    for(uint16_t i = 0; i < MAX_NUM_PULSES; i++)
    {
        pulse_t p = find_pulse(&index);
        
        if(p.width >= MIN_PULSE_WIDTH)
        {
            pulse[num_pulses] = p;
            num_pulses++;
        }
        
        index += 5; //slight hysteresis
        
        if(index >= NUM_LEVELS)
            break;
    }
}

static pulse_t find_pulse(uint16_t *index)
{
    pulse_t p;
    
    /* Find start */
    uint16_t start = *index;
    do
    {
        int16_t tmp = measure[start] - calibration[start] - pgm_read_byte(&sonar_threshold[start]);
        if(tmp < 0)
            start++;
        else
            break;
    } while(start < NUM_LEVELS);
    
    /* Find peak index and end of pulse */
    uint16_t end = start;
    uint8_t peak = 0;
    uint16_t peak_index = 0;
    do
    {
        int16_t tmp = measure[start] - calibration[start] - pgm_read_byte(&sonar_threshold[start]);
        if(tmp > peak)
        {
            peak_index = end;
            peak = tmp;
        }
        if(tmp > 0)
            end++;
        else
            break;
    } while(end < NUM_LEVELS);

    p.peak_index = peak_index;
    p.width = end - start;
    *index = end;

    return p;
}
