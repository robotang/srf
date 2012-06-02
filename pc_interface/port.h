/** 	@file   port.h
	@author M. P. Hayes, UCECE
	@date   11 Jan 2006
	@brief  I/O port hardware abstraction for AVR microcontroller.
	@note   Macros are used to avoid function call overhead and to allow
		compile-time constant folding. 
*/
#ifndef PORT_H
#define PORT_H

#include "global.h"

#define PORT_AVR

/* With the AVR, the DDR bits are set 1 for an output, 0 for input. 
   By default all ports are inputs.  

   With the PIC, the TRIS (tristate) register bits are set 1 for an
   input, 0 for output.  */

/** Define port names, note not all the ports are available on some AVRs.  */

#ifdef PORTA
#define PORT_A &PORTA
#endif

#ifdef PORTB
#define PORT_B &PORTB
#endif

#ifdef PORTC
#define PORT_C &PORTC
#endif

#ifdef PORTD
#define PORT_D &PORTD
#endif

#ifdef PORTE
#define PORT_E &PORTE
#endif

#ifdef PORTF
#define PORT_F &PORTF
#endif

#ifdef PORTG
#define PORT_G &PORTG
#endif


#define PORT_BITS_MASK(first, last) ((1 << ((last) + 1)) - (1 << (first)))

typedef volatile uint8_t *port_t;
typedef uint8_t port_bit_t;
typedef uint8_t port_data_t;
typedef uint8_t port_mask_t;


/** DDR  PORT  Comment
	 0	 0  High impedance input
	 0	 1  Weak pullup input
	 1	 0  Output low
	 1	 1  Output high
*/


/** Private macro to map a port register to its corresponding data
   direction register (DDR).  NB, the DDR and PORT registers must be
   separated by the same number of bytes in all cases.  PORTB is used
   for the pattern since PORTA is not always defined for some
   AVRs.  */
#define PORT_DDR_(PORT) (*((PORT) + (&DDRB - &PORTB)))

/** Private macro to map a port register to its input register (PIN).
   NB, the PIN and PORT registers must be separated by the same number
   of bytes in all cases.  PORTB is used for the pattern since PORTA
   is not always defined for some AVRs.  */
#define PORT_PIN_(PORT) (*((PORT) + (&PINB - &PORTB)))

/** Private macro to access a port data register.  */
#define PORT_DATA_(PORT) (*(PORT))


/** Read input state from port.
	@param port I/O port
	@return port pin value  */
#define port_read(port) *(PORT_PIN_ (port))

/** Get output state for port.  
	@param port I/O port
	@return port value  */
#define port_get(port) *(port)

/** Write value to port.  
	@param port I/O port
	@param value value to write  */
#define port_write(port, value) *(port) = (value)


/** Configure selected pins of port as outputs.
	@param port I/O port
	@param pins bit mask for pins to configure as outputs  */
#define port_pins_config_output(port, pins) PORT_DDR_ (port) |= (pins)

/** Configure selected pins of port as inputs without pullup.
	@param port I/O port
	@param pins bit mask for pins to configure as inputs  */  
#define port_pins_config_input(port, pins) \
	do {PORT_DATA_ (port) &= ~(pins); PORT_DDR_ (port) &= ~(pins);} while (0)

/** Configure selected pins of port as inputs with pullup.
	@param port I/O port
	@param pins bit mask for pins to configure as inputs with pullup  */
#define port_pins_config_pullup(port, pins) \
	do {PORT_DATA_ (port) |= (pins); PORT_DDR_ (port) &= ~(pins);} while (0)

/** Set selected pins of port high.
	@param port I/O port
	@param pins bit mask for pins to set high  */
#define port_pins_set_high(port, pins) PORT_DATA_ (port) |= (pins)

/** Set selected pins of port low. 
	@param port I/O port
	@param pins bit mask for pins to set low  */
#define port_pins_set_low(port, pins) PORT_DATA_ (port) &= ~(pins)

/** Toggle selected pins of port.
	@param port I/O port
	@param pins bit mask for pins to toggle  */
#define port_pins_toggle(port, pins) PORT_DATA_ (port) ^= (pins)

/** Read input state from selected pins of port. 
	@param port I/O port
	@param pins bit mask for pins to read input state
	@return input state of selected pins  */
#define port_pins_read(port, pins) (PORT_PIN_ (port) & (pins))

/** Get output state for selected pins of port. 
	@param port I/O port
	@param pins bit mask for pins to get output state 
	@return output state of selected pins  */
#define port_pins_get(port, pins) (PORT_DATA_ (port) & (pins))

/** Write to selected pins of port. 
	@param port I/O port
	@param pins bit mask for pins to write to
	@param value value to write
	@note It might be more efficient to and with mask then xor with value.  */
#define port_pins_write(port, pins, value) \
	(PORT_DATA_ (port) = (PORT_DATA_ (port) & ~(pins)) | (value))

/** Configure selected pin of port as output. 
	@param port I/O port
	@param pin pin number to set as output  */
#define port_pin_config_output(port, pin) \
	port_pins_config_output (port, BIT (pin))

/** Configure selected pin of port as input without pullup.
	@param port I/O port
	@param pin pin number to set as input  */
#define port_pin_config_input(port, pin) \
	port_pins_config_input (port, BIT (pin))

/** Configure selected pin of port as input with pullup.
	@param port I/O port
	@param pin pin number to set as input with pullup  */
#define port_pin_config_pullup(port, pin) \
	port_pins_config_pullup (port, BIT (pin))

/** Set selected pin of port high.
	@param port I/O port
	@param pin pin number to set high  */
#define port_pin_set_high(port, pin) port_pins_set_high (port, BIT (pin))

/** Set selected pin of port low.
	@param port I/O port
	@param pin pin number to set low  */
#define port_pin_set_low(port, pin) port_pins_set_low (port, BIT (pin))

/** Set selected pins of port to desired state
	@param port I/O port
	@param pins bit mask for pins to write to
	@param state value to write to each selected pin  */
#define port_pins_set(port, pins, state)  \
	do {port_data_t _tmp = PORT_DATA_ (port) | pins; \
		_tmp ^= (state ? 0 : pins); \
	PORT_DATA_ (port) = _tmp;} while (0)

/** Toggle selected pin of port.
	@param port I/O port
	@param pin pin number to toggle  */
#define port_pin_toggle(port, pin) port_pins_toggle (port, BIT (pin))

/** Get output state for selected pin of port.
	@param port I/O port
	@param pin pin number to get output state
	@return output state of selected pin  */
#define port_pin_get(port, pin) (port_pins_get (port, BIT (pin)) != 0)

/** Read input state from selected pin of port. 
	@param port I/O port
	@param pin pin number to get input state
	@return input state of selected pin  */
#define port_pin_read(port, pin) (port_pins_read (port, BIT (pin)) != 0)

/** Write state of selected pin of port.
	@param port I/O port
	@param pin pin number to set output state
	@param state value to write  */
#define port_pin_write(port, pin, state)			 \
	((state) ? port_pin_set_high ((port), BIT (pin)) \
		   : port_pin_set_low ((port), BIT (pin))



/** Configure selected bus of port as output. 
	@param port I/O port
	@param pin1 first pin defining output bus
	@param pin2 last pin defining output bus  */
#define port_bus_config_output(port, pin1, pin2) \
	port_pins_config_output (port, PORT_BITS_MASK (pin1, pin2))

/** Configure selected bus of port as input without pullup. 
	@param port I/O port
	@param pin1 first pin defining input bus
	@param pin2 last pin defining input bus  */
#define port_bus_config_input(port, pin1, pin2) \
	port_pins_config_input (port, PORT_BITS_MASK (pin1, pin2))

/** Configure selected bus of port as input with pullup.
	@param port I/O port
	@param pin1 first pin defining input bus with pullup
	@param pin2 last pin defining input bus with pullup  */
#define port_bus_config_pullup(port, pin1, pin2) \
	port_pins_config_pullup (port, PORT_BITS_MASK (pin1, pin2))

/** Write value to selected bus of port. 
	@param port I/O port
	@param pin1 first pin defining output bus
	@param pin2 last pin defining output bus
	@param value value to write to bus  */
#define port_bus_write(port, pin1, pin2, value)				 \
	port_write ((port), (port_get (port) & ~PORT_BITS_MASK (pin1, pin2)) \
		  | (((value) & PORT_BITS_MASK (0, pin2 - pin1)) << (pin1)))

/** Read value from selected bus of port.
	@param port I/O port
	@param pin1 first pin defining input bus
	@param pin2 last pin defining input bus
	@return value read from bus  */
#define port_bus_read(port, pin1, pin2) \
	((port_read (port) & PORT_BITS_MASK (pin1, pin2) >> (pin1))

#endif
