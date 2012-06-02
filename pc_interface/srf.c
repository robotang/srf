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

#include "i2c.h"
#include "delay.h"
#include "srf.h"
#include "rprintf.h"

#define TARGET_ADDR                 10
#define READ_CM                     27
#define READ_LOW                    1
#define READ_HIGH                   2

#define SRF_MAKE_MEASUREMENT        1
#define SRF_GET_READING_CM          2
#define SRF_PING                    3
#define SRF_LISTEN                  4
#define SRF_I2C_TEST                5
#define SRF_AUTOMATIC_RANGING       6

#define SRF_CALIBRATE               10
#define SRF_GET_CALIBRATION         11
#define SRF_GET_DATA                12    

static void srf_get_data(uint8_t field);

uint8_t txdata[2];
uint8_t rxdata[32];

void srf_init(void)
{
    i2cInit();
    i2cSetBitrate(100);
}

void srf_auto_range(bool auto_range)
{
    txdata[0] = SRF_AUTOMATIC_RANGING;
    txdata[1] = (auto_range) ? 1 : 0;
    i2cMasterSend(TARGET_ADDR, 2, txdata);
}

void srf_measure(void)
{
    txdata[0] = SRF_MAKE_MEASUREMENT;
    i2cMasterSend(TARGET_ADDR, 1, txdata);
    for(uint8_t i = 0; i < 10; i++)
        _delay_ms(10);
}

uint16_t srf_read(void)
{
    txdata[0] = SRF_GET_READING_CM;
    i2cMasterSend(TARGET_ADDR, 1, txdata);
    i2cMasterReceive(TARGET_ADDR, 2, rxdata);
    return (rxdata[1] << 8) + rxdata[0];
}

void srf_read_raw(void)
{        
    srf_measure();    
    srf_get_data(SRF_GET_DATA);
}

void srf_calibrate(void)
{
    txdata[0] = SRF_CALIBRATE;
    i2cMasterSend(TARGET_ADDR, 1, txdata);
    for(uint8_t i = 0; i < 150; i++)
        _delay_ms(20);    

    srf_get_data(SRF_GET_CALIBRATION);
}

static void srf_get_data(uint8_t field)
{    
    txdata[0] = field;
    for(uint8_t i = 0; i < 45; i++)
    {
        for(uint8_t j = 0; j < 10; j++)
            rxdata[j] = 0;
        
        txdata[1] = i;
        i2cMasterSend(TARGET_ADDR, 2, txdata);        
        i2cMasterReceive(TARGET_ADDR, 10, rxdata);
        for(uint8_t j = 0; j < 10; j++)
            rprintf("%d,", rxdata[j]);
        _delay_ms(10);
    }
    rprintf("\r\n\n");
}
