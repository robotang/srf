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

#ifndef FILTER_H
#define FILTER_H

#include "global.h"

#define MAX_FILTER_DEPTH    10

typedef struct 
{    
    uint16_t data[MAX_FILTER_DEPTH];
    uint8_t depth;
    uint8_t position;
} filter_t;

void filter_init(filter_t *filter, uint8_t depth);
void filter_flush(filter_t *filter, uint16_t value);
uint16_t filter_avg(filter_t *filter, uint16_t value);

#endif
