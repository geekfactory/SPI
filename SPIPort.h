#ifndef SPIPORT_H
#define SPIPORT_H

#if defined( PLIB_PIC16 )

	#include <xc.h>
	#include <stdint.h>

	// On PIC16 xUARTHandle is a char
	typedef char xSPIHandle;

#elif defined( PLIB_PIC18 )

	#include <xc.h>
	#include <stdint.h>

	// On PIC18 xUARTHandle is a char
	typedef char xSPIHandle;
	
#endif

#endif
// End of header file
