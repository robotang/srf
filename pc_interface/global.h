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

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "board.h"

#define F_CPU									16000000 // 16MHz processor
#define CYCLES_PER_US 							((F_CPU+500000)/1000000)  // cpu cycles per microsecond
 
#define BIND(LOWER,val,UPPER)					MIN((MAX(val,LOWER)),UPPER)

#define HIGH_BYTE(x) 							((x) >> 8)
#define LOW_BYTE(x)								((x) & 0x00FF)
#define BYTES_TO_INT16(high, low)				(((high) << 8) + (low))

#define MIN_CONTROL		-255
#define MAX_CONTROL		255

/* Depreciated macros, define if not already defined */
#ifndef cbi
	#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
	#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#ifndef inb
	#define inb(sfr) _SFR_BYTE(sfr)
#endif
#ifndef outb
	#define outb(sfr, val) (_SFR_BYTE(sfr) = (val))
#endif
#ifndef inw
	#define inw(sfr) _SFR_WORD(sfr)
#endif
#ifndef outw
	#define outw(sfr, val) (_SFR_WORD(sfr) = (val))
#endif
#ifndef outp
	#define outp(val, sfr) outb(sfr, val)
#endif
#ifndef inp
	#define inp(sfr) inb(sfr)
#endif
#ifndef BV
	#define BV(bit) _BV(bit)
#endif

#define BIT(X) _BV(X)

/* Macros to set, clear, and test bits.  */
#define BSET(PORT, PIN)  ((PORT) |= (BIT (PIN)))
#define BCLR(PORT, PIN)  ((PORT) &= ~(BIT (PIN)))
#define BTST(PORT, PIN)  ((PORT) & (BIT (PIN)))

#define ARRAY_SIZE(ARRAY) (sizeof(ARRAY) / sizeof (ARRAY[0]))

#define LED_DEBUG_ON			cbi(LED_DEBUG_PORT, LED_DEBUG_PIN)
#define LED_DEBUG_OFF			sbi(LED_DEBUG_PORT, LED_DEBUG_PIN)
#define LED_DEBUG_TOGGLE		sbi(LED_DEBUG_PORTIN, LED_DEBUG_PIN)
#define LED_DEBUG_INIT()		sbi(LED_DEBUG_DDR, LED_DEBUG_PIN); LED_DEBUG_OFF

#define DEBUG_ON				sbi(ENC0_PHASEB_PORT, ENC0_PHASEB_PIN)
#define DEBUG_OFF				cbi(ENC0_PHASEB_PORT, ENC0_PHASEB_PIN)
#define DEBUG_TOGGLE			sbi(ENC0_PHASEB_PORTIN, ENC0_PHASEB_PIN)
#define DEBUG_INIT()			sbi(ENC0_PHASEB_DDR, ENC0_PHASEB_PIN); DEBUG_OFF



#endif
