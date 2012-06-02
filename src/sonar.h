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

#ifndef SONAR_H
#define SONAR_H

#include "global.h"

#define NUM_LEVELS             450 //do not modify!

void sonar_init(void);
void sonar_measure(void);
void sonar_ping(void);
void sonar_listen(void);
void sonar_calibrate(void);

#endif