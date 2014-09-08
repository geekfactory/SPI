/*	SPI Interface Driver
	Copyright (C) 2014 Jesus Ruben Santa Anna Zamudio.

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

	Author website: http://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
#ifndef SPI_H
#define SPI_H
/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include "SPIPort.h"

/*-------------------------------------------------------------*/
/*		Macros and definitions				*/
/*-------------------------------------------------------------*/
// SPI operation mode
#define SPI_MASTER	0x00000001	//!< Configures the SPI module as Master
#define SPI_SLAVE	0x00000002	//!< Configures the SPI module as Slave

// Clock polarity and phase configuration
#define SPI_CPOL0_CPHA0 0x00000010
#define SPI_CPOL0_CPHA1	0x00000020
#define SPI_CPOL1_CPHA0	0x00000030
#define SPI_CPOL1_CPHA1 0x00000040
#define SPI_MODE_0	SPI_CPOL0_CPHA0
#define SPI_MODE_1	SPI_CPOL0_CPHA1
#define SPI_MODE_2	SPI_CPOL1_CPHA0
#define SPI_MODE_3	SPI_CPOL1_CPHA1

// Order of bit transmission
#define SPI_MSB_LSB	0x00000100	//!< Transmit MSB first, LSB last (default)
#define SPI_LSB_MSB	0x00000200	//!< Transmit LSB first, MSB last

// Number of bits per word default is 8
#define SPI_DATA_BITS(n) ((n & 0x0000001F) << 12)	//!< Macro to set data bits per SPI word

// Generic Divider Settings
#define SPI_DIV_1_1	1
#define SPI_DIV_1_2	2
#define SPI_DIV_1_4	3
#define SPI_DIV_1_8	4
#define SPI_DIV_1_16	5
#define SPI_DIV_1_32	6
#define SPI_DIV_1_64	7
/*-------------------------------------------------------------*/
/*		Typedefs enums & structs			*/
/*-------------------------------------------------------------*/

/**
Defines the available SPI hardware modules
 */
enum enSPIModules {
	E_SPI_1 = 1,
	E_SPI_2 = 2,
	E_SPI_3 = 3,
	E_SPI_4 = 4,
	E_SPI_5 = 5,
	E_SPI_6 = 6,
	E_SPI_7 = 7,
	E_SPI_8 = 8,
};

// Define "boolean" values
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/

/**
 * @brief Prepares the SPI hardware for use
 *
 * Obtains the requested SPI module descriptor used to access the hardware. 
 *
 * @param eModule The SPI module
 *
 * @return The SPI descriptor asociated with the selected module or NULL if 
 * something fails
 */
xSPIHandle spi_init(enum enSPIModules eModule);

/**
 * @brief Configures the SPI module
 *
 * This function configures the operation parameters for the SPI module
 *
 * @param spid The SPI descriptor of the module to control
 * @param control The control flags for the SPI module
 * @param arg The argument for the passed control option
 *
 * @return Returns 0 if success, negative value if fails
 */
uint8_t spi_control(xSPIHandle spid, uint32_t ctrl, uint32_t arg);

/**
 * @brief Enables SPI module operation
 *
 * Enables the SPI module and the SPI hardware takes control over the IO pins.
 * If implemented, this function claims exclusive access over the SPI.
 *
 * @param spid The SPI descriptor of the module to open
 *
 * @return Non-zero value if the SPI module was opened
 */
uint8_t spi_open(xSPIHandle spid);

/**
 * @brief Disables SPI module operation
 *
 * Disables the SPI module and in most platforms the IO pins are free to use.
 * If implemented, this function returns the exclusive access mutex.
 *
 * @param spid The SPI descriptor of the module to close
 *
 * @return Non-zero value if the SPI module was closed
 */
uint8_t spi_close(xSPIHandle spid);

/**
 * @brief Writes a single byte to the SPI
 *
 * @param spid The SPI descriptor of the module to write
 *
 * @param data The data byte to be transfered
 */
void spi_write(xSPIHandle spid, uint8_t data);

/**
 * @brief Reads a single byte from the SPI
 *
 * @param spid The SPI descriptor of the module to read
 *
 * @return The received data byte
 */
uint8_t spi_read(xSPIHandle spid);

/**
 * @brief Writes an array of bytes to the SPI
 *
 * @param spid The SPI descriptor of the module to write
 * @param txbuf Pointer to buffer containing the data to transmit
 * @param len The lenght of the data to transfer
 */
void spi_write_array(xSPIHandle spid, const uint8_t * txbuf, uint16_t len);

/**
 * @brief Reads an array of bytes from the SPI
 * 
 * @param spid The SPI descriptor of the module to read
 * @param rxbuf Pointer to buffer to hold the received data
 * @param len The length of the data to be received
 */
void spi_read_array(xSPIHandle spid, uint8_t * rxbuf, uint16_t len);

/**
 * @brief Transfers a byte to and from the SPI
 *
 * @param spid The SPI descriptor of the module to read/write
 * @param data The data byte to be transfered
 *
 * @return The received data byte
 */
uint8_t spi_trans(xSPIHandle spid, uint8_t data);

/**
 * @brief Transfers an array of bytes to and from the SPI
 *
 * @param spid The SPI hardware descriptor of the module to read/write
 * @param txbuf Pointer to buffer containing the data to transmit
 * @param rxbuf Pointer to buffer to hold the received data
 * @param len The length of the Read and Write buffers
 */
void spi_trans_array(xSPIHandle spid, uint8_t * txbuf, uint8_t * rxbuf, uint16_t len);

#endif
// End of Header file
