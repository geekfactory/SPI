#include <stdio.h>
#include "SPI.h"

int main()
{
	// Variable to hold information needed to access the SPI hardware
	xSPIHandle spi1;
	BYTE rxdata, i;
	
	// Get SPI descriptor
	spi1 = spi_init( E_SPI_1 );
	
	// Check for valid SPI handle
	if( spi1 == NULL ) {
		printf( "ERROR: Invalid SPI module selection or OS failure" );
	} else {
		// Try to start using the SPI module
		if( spi_open( spi1 ) ) {
			for( i=0; i<10; i++ ) {
			
				printf( "Send to SPI: %d", i );
				
				// Read and write to the SPI device on a single call
				rxdata = spi_tans( spi1, i );
				
				printf( "Receive from SPI:  %d", rxdata );
			}
			// Release SPI module
			spi_close( spi1 );
		} else {
			printf("ERROR: Can't open SPI module");
		}
	}
	while(1);
}