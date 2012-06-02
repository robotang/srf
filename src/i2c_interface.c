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

#include "i2c_interface.h"
#include "i2c.h"
#include "delay.h"
#include "sonar.h"
#include <avr/eeprom.h>

#define HIGH_BYTE(x)     ((x) >> 8)
#define LOW_BYTE(x)        ((x) & 0xFF)

static void i2c_receive(uint8_t receiveDataLength, uint8_t *rxdata);
static uint8_t i2c_transmit(uint8_t transmitDataLengthMax, uint8_t *txdata);

volatile bool received_i2c, automatic_ranging;
static volatile uint8_t reg, packet_index;
static volatile status_t status;
extern volatile uint16_t distance, distance_filtered;
extern uint8_t calibration[NUM_LEVELS];
extern uint8_t measure[NUM_LEVELS];

void i2c_interface_init(void)
{
    uint8_t address = DEFAULT_ADDRESS;
    /*uint8_t address = eeprom_read_byte(0);
    if(address == 0xFF) {
        address = DEFAULT_ADDRESS;
        eeprom_write_byte(0, address);
    }*/    
    
    i2cInit();
    i2cSetBitrate(SRF_I2C_SPEED);
    i2cSetLocalDeviceAddr(address, 0);
    i2cSetSlaveReceiveHandler(i2c_receive);
    i2cSetSlaveTransmitHandler(i2c_transmit);
    
    status = STATUS_NONE;
    reg = 0;
    packet_index = 0;
    received_i2c = false;
    automatic_ranging = false;
}

status_t i2c_interface_update(void)
{
    if(received_i2c)
    {
        status_t tmp = status;
        status = STATUS_NONE;
        received_i2c = false;
        return tmp;
    }
    return STATUS_NONE;
}

static void i2c_receive(uint8_t receiveDataLength, uint8_t *rxdata)
{
    received_i2c = true;
    
    switch(rxdata[0])
    {
        case SRF_MAKE_MEASUREMENT: 
        {
            status = STATUS_MEASURE;            
        } 
        break;
        
        case SRF_PING: 
        {
            status = STATUS_PING; 
        } 
        break;
        
        case SRF_LISTEN:
        {
            status = STATUS_LISTEN; 
        } 
        break;
        
        case SRF_AUTOMATIC_RANGING:
        {
            if(rxdata[1])
                automatic_ranging = true;
            else
                automatic_ranging = false;
        } 
        break;
        
        case SRF_CALIBRATE: 
        {
            status = STATUS_CALIBRATE; 
        } 
        break;
        
        case SRF_UPLOAD_CALIBRATION: 
        {
            reg = SRF_UPLOAD_CALIBRATION; 
            packet_index = BIND(0, rxdata[1], SRF_UPLOAD_NUM_PACKETS);
        } 
        break;
        
        case SRF_UPLOAD_DATA: 
        {
            reg = SRF_UPLOAD_DATA; 
            packet_index = BIND(0, rxdata[1], SRF_UPLOAD_NUM_PACKETS);
        } 
        break;
        
        default: 
        {
            reg = rxdata[0]; 
        }
        break;    
    }
    
    i2cFlushBuffers();
}

static uint8_t i2c_transmit(uint8_t transmitDataLengthMax, uint8_t *txdata)
{
    uint8_t n = 0;
    switch(reg)
    {
        case SRF_GET_READING_CM: 
        {
            if(automatic_ranging)
            {
                txdata[n++] = LOW_BYTE(distance_filtered);
                txdata[n++] = HIGH_BYTE(distance_filtered);
            }
            else
            {
                txdata[n++] = LOW_BYTE(distance);
                txdata[n++] = HIGH_BYTE(distance);
            }
        }
        break;
        
        case SRF_I2C_TEST:
        {
            txdata[n++] = 123;
            txdata[n++] = 98;
        }
        break;
        
        case SRF_UPLOAD_CALIBRATION:
        {
            for(uint8_t i = 0; i < SRF_UPLOAD_PACKET_SIZE; i++)
            {
                txdata[n++] = calibration[packet_index*SRF_UPLOAD_PACKET_SIZE+i];            
            }
        }
        break;
        
        case SRF_UPLOAD_DATA:
        {
            for(uint8_t i = 0; i < SRF_UPLOAD_PACKET_SIZE; i++)
            {
                txdata[n++] = measure[packet_index*SRF_UPLOAD_PACKET_SIZE+i];            
            }
        }
        break;
    }
    
    return n;
}
