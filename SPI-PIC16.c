/**
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SPI.h"

static BOOL inuse = FALSE;

static BOOL spi_available( xSPIHandle spid );

xSPIHandle spi_init( enum enSPIModules eModule )
{
	if( eModule != 1 )
		return 0;

	SSPSTAT = 0x00;
	SSPCON = 0x00;

	// Apply default SPI configuration
	spi_control( eModule, SPI_MASTER | SPI_MODE_0, SPI_DIV_1_4 );

	// Retrun module reference
	return eModule;
}

PORTBASE spi_control( xSPIHandle spid, DWORD ctrl, DWORD arg )
{
	// Check handle
	if( spid != 1 )
		return -1;

	BYTE speed = (BYTE) arg & 0x0000000F;

	// Process the SPI mode fields
	switch( ctrl & 0x0000000F ) {
		// If SPI is master, use arg as prescaler config or baurdate information
	case SPI_MASTER:
		if( speed == SPI_DIV_1_4 )
			SSPCONbits.SSPM = 0;
		else if( speed == SPI_DIV_1_16 )
			SSPCONbits.SSPM = 1;
		else if( speed == SPI_DIV_1_64 )
			SSPCONbits.SSPM = 2;
		else
			// Default to low speed
			SSPCONbits.SSPM = 2;
		break;
	case SPI_SLAVE:
		// Configure SPI module as slave
		SSPCONbits.SSPM = 4;
		break;
		// Any other operating mode is invalid
	default:
		return -2;
	}

	// Process the SPI clock phase and polarity fields
	switch( ctrl & 0x000000F0 ) {
	case SPI_CPOL0_CPHA0:
		SSPCONbits.CKP = 0;
		SSPSTATbits.CKE = 0;
		break;
	case SPI_CPOL0_CPHA1:
		SSPCONbits.CKP = 0;
		SSPSTATbits.CKE = 1;
		break;
	case SPI_CPOL1_CPHA0:
		SSPCONbits.CKP = 1;
		SSPSTATbits.CKE = 0;
		break;
	case SPI_CPOL1_CPHA1:
		SSPCONbits.CKP = 1;
		SSPSTATbits.CKE = 1;
		break;
	default:
		return -2;
	}
	return TRUE;
}

BOOL spi_open( xSPIHandle spid )
{
	inuse = TRUE;
	SSPCONbits.SSPEN = 1;
	return TRUE;
}

BOOL spi_close( xSPIHandle spid )
{
	inuse = FALSE;
	SSPCONbits.SSPEN = 0;
	return TRUE;
}

void spi_write( xSPIHandle spid, BYTE data )
{
	BYTE rxtmp;
	SSPBUF = data;		// Write to SPI
	while( !spi_available( spid ) ); // Wait for transfer to complete
	rxtmp = SSPBUF;		// Dummy read
}

BYTE spi_read( xSPIHandle spid )
{
	SSPBUF = 0xAA;		// Writes dummy byte
	while( !spi_available( spid ) ); // Wait for transfer to complete
	return SSPBUF;		// Returns received byte
}

void spi_write_array( xSPIHandle spid, const BYTE * txbuf, unsigned short len )
{
	BYTE rxtmp;
	while( len ) {
		SSPBUF = *txbuf++;
		while( !spi_available( spid ) );
		rxtmp = SSPBUF;
		len--;
	}
}

void spi_read_array( xSPIHandle spid, BYTE * rxbuf, unsigned short len )
{
	while( len ) {
		SSPBUF = 0xAA;
		while( !spi_available( spid ) ); // Wait for data to be received
		*rxbuf++ = SSPBUF; // Read from SPI buffer to array
		len--;
	}
}

BYTE spi_trans( xSPIHandle spid, BYTE data )
{
	BYTE rxtmp;
	SSPBUF = data;
	while( !spi_available( spid ) );
	rxtmp = SSPBUF;
	return rxtmp;
}

void spi_trans_array( xSPIHandle spid, BYTE * txbuf, BYTE * rxbuf, unsigned short len )
{
	while( len ) {
		SSPBUF = *txbuf++;
		while( !spi_available( spid ) );
		*rxbuf++ = SSPBUF;
		len--;
	}
}

static BOOL spi_available( xSPIHandle spid )
{
	return ( SSPSTATbits.BF ) ? TRUE : FALSE;
}
