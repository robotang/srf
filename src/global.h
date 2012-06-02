 /*! \file global.h \brief AVRlib project global include. */
 //*****************************************************************************
 //
 // File Name    : 'global.h'
 // Title        : AVRlib project global include 
 // Author       : Pascal Stang - Copyright (C) 2001-2002
 // Created      : 7/12/2001
 // Revised      : 9/30/2002
 // Version      : 1.1
 // Target MCU   : Atmel AVR series
 // Editor Tabs  : 4
 //
 //  Description : This include file is designed to contain items useful to all
 //                  code files and projects.
 //
 // This code is distributed under the GNU Public License
 //      which can be found at http://www.gnu.org/licenses/gpl.txt
 //
 //*****************************************************************************
 
 #ifndef GLOBAL_H
 #define GLOBAL_H
 
 // global AVRLIB defines
 #include "avrlibdefs.h"
 // global AVRLIB types definitions
 #include "avrlibtypes.h"
 
 #include "board.h"
 
 #include <stdint.h>
 #include <stdbool.h>
 
 // CPU clock speed
 #define F_CPU           8000000                      // 8MHz processor
 #define CYCLES_PER_US ((F_CPU+500000)/1000000)  // cpu cycles per microsecond
 
 #define BIND(LOWER,val,UPPER)		MIN((MAX(val,LOWER)),UPPER)
 
 #endif
