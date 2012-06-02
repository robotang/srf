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

#include "filter.h"

void filter_init(filter_t *filter, uint8_t depth)
{    
    if(depth > MAX_FILTER_DEPTH)
        filter->depth = MAX_FILTER_DEPTH;
    else if(depth == 0)
        filter->depth = 1;
    else
        filter->depth = depth;
    filter->position = 0;
    filter_flush(filter, 0);
}

void filter_flush(filter_t *filter, uint16_t value)
{
    uint8_t i;
    for(i = 0; i < MAX_FILTER_DEPTH; i++)
        filter->data[i] = value;
}

uint16_t filter_avg(filter_t *filter, uint16_t value)
{
    //Push data onto storage
    filter->data[filter->position] = value;
    filter->position++;
    if(filter->position >= (filter->depth))
        filter->position = 0;
    
    //Find average
    uint32_t sum = 0;
    uint8_t i;
    for(i = 0; i < filter->depth; i++)
        sum += filter->data[i];
    sum /= filter->depth;
    return (uint16_t) sum;
}
