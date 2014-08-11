/************************* http://geekfactory.mx *******************************
 *
 * This program demonstrates the transmission and reception of data through the
 * USART peripheral. You can review the code on UART.h and UART-PIC16.c to see
 * how each function is implemented.
 *
 * Este programa demuestra la transmisión y recepción de datos a través de la
 * USART del PIC. Puede revisarse el código fuente de las funciones en los
 * archivos UART.h y UART-PIC16.c
 *
 * AUTHOR/AUTOR: Jesus Ruben Santa Anna Zamudio
 * COMPILER/COMPILADOR: Microchip XC8 http://www.microchip.com/compilers
 *
 ********************************************************************************/
#include <xc.h>			// Encabezado para el compilador XC8
#include <stdio.h>
#include "../SPI.h"
#include "../../UART/UART.h"

/*
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)
 */

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

xUARTHandle uart1;
xSPIHandle spi1;

/* Needed for prinf to work*/
void putch( BYTE data )
{
	uart_write( uart1, data );
}

void main( )
{
	ANSEL = 0x00;
	TRISCbits.TRISC3 = 0;
	TRISCbits.TRISC4 = 1;
	TRISCbits.TRISC5 = 0;

	// Variable declaration
	BYTE rx, i;

	spi1 = spi_init( E_SPI_1 );
	uart1 = uart_init( E_UART_1 );

	uart_control( uart1, UART_DATABITS_8 | UART_PARITY_NONE | UART_STOPBITS_1, 9600 );
	spi_control( spi1, SPI_MASTER | SPI_MODE_0, SPI_DIV_1_64 );

	// Open UART with default configuration
	if( !uart_open( uart1 ) )
		for(;; ); // Error opening UART

	uart_puts( uart1, "PIC SPI Demo\r\n" );

	// Start using the SPI module
	if( spi_open( spi1 ) ) {
		// Send 10 bytes of data with numers 0 to 9
		for( i = 0; i < 255; i++ ) {
			printf( "Send to SPI: %d\r\n", i );

			// Read and write to the SPI device on a single call
			rx = spi_trans( spi1, i );

			printf( "Receive from SPI: %d\r\n", rx );
		}
		// Release SPI module
		spi_close( spi1 );
		uart_puts( uart1, "End of SPI demo\r\n" );
	}
	for(;; );
}
