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

#elif defined( PLIB_RBPI )

	#include <stdint.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/spi/spidev.h>

	typedef int xSPIHandle;
	
#endif

#endif
// End of header file
