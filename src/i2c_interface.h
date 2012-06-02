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

#ifndef I2C_INTERFACE
#define I2C_INTERFACE

#include "global.h"

#define SRF_MAKE_MEASUREMENT        1
#define SRF_GET_READING_CM          2
#define SRF_PING                    3
#define SRF_LISTEN                  4
#define SRF_I2C_TEST                5
#define SRF_AUTOMATIC_RANGING       6

#define SRF_CALIBRATE               10
#define SRF_UPLOAD_CALIBRATION      11
#define SRF_UPLOAD_DATA             12            

#define DEFAULT_ADDRESS             10
#define AD_CHANGE_1                 20
#define AD_CHANGE_2                 21
#define AD_CHANGE_3                 22

#define SRF_I2C_SPEED               100 //in kHz

#define SRF_UPLOAD_PACKET_SIZE      10
#define SRF_UPLOAD_NUM_PACKETS      (NUM_LEVELS/SRF_UPLOAD_PACKET_SIZE)

typedef enum {STATUS_NONE, STATUS_MEASURE, STATUS_PING, STATUS_LISTEN, STATUS_CALIBRATE} status_t; 

void i2c_interface_init(void);
status_t i2c_interface_update(void);

#endif
