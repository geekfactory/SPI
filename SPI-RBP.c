/*	SPI Interface Driver for Raspberry Pi
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
#include "SPI.h"

xSPIHandle spi_init(enum enSPIModules eModule)
{
	// Raspberry Pi has 2 SPI modules available
	if (eModule < 1 || eModule > 2)
		return -1;

	// Open the device file
	switch (eModule) {
	case E_SPI_1:
		spifd = open("/dev/spidev0.0", O_RDWR);
		break;
	case E_SPI_2:
		spifd = open("/dev/spidev0.1", O_RDWR);
		break;
	default:
		// Invalid module number
		return -1;
	}

	// If failed to open file
	if (spifd < 0) {
		perror("ERROR ON SPI INIT");
		return -1;
	}

	// Retrun module reference
	return spifd;
}

int spi_control(xSPIHandle spid, uint32_t ctrl, uint32_t arg)
{
	uint8_t spiconf;
	// Check for valid file descriptor
	if (spid < 0)
		return -2;

	// Process the SPI mode fields
	switch (ctrl & 0x0000000F) {
	case SPI_MASTER:
		// Arg is used as clock speed in master mode
		if (ioctl(spid, SPI_IOC_WR_MAX_SPEED_HZ, &arg) < 0) {
			perror("ERROR ON SPI CONFIG (SPEED)");
			return -1;
		}
		break;

	case SPI_SLAVE:
		// No support for SPI slave mode, causes error
	default:
		return -2;
	}

	// Process the SPI clock phase and polarity control
	// set mode according to header file: /linux/spi/spidev.h
	switch (ctrl & 0x000000F0) {
	case SPI_CPOL0_CPHA0:
		spiconf = SPI_MODE_0;
		break;
	case SPI_CPOL0_CPHA1:
		spiconf = SPI_MODE_1;
		break;
	case SPI_CPOL1_CPHA0:
		spiconf = SPI_MODE_2;
		break;
	case SPI_CPOL1_CPHA1:
		spiconf = SPI_MODE_3;
		break;
	default:
		return -2;
	}
	if (ioctl(spid, SPI_IOC_WR_MODE, &spiconf) < 0) {
		perror("ERROR ON SPI CONFIG (CLK-MODE)");
		return -1;
	}

	// Process SPI bit order control
	switch (ctrl & 0x00000F00) {
	case SPI_LSB_MSB:
		spiconf = 0 | SPI_LSB_FIRST;
		break;
	case SPI_MSB_LSB:
	default:
		spiconf = 0;
	}
	if (ioctl(spid, SPI_IOC_WR_LSB_FIRST, &spiconf) < 0) {
		perror("ERROR ON SPI CONFIG (JUSTIFY)");
		return -1;
	}

	// Process SPI bits per word
	spiconf = ((ctrl & 0x0001F000) >> 12);
	if (ioctl(spid, SPI_IOC_WR_BITS_PER_WORD, &spiconf) < 0) {
		perror("ERROR ON SPI CONFIG (JUSTIFY)");
		return -1;
	}


	return TRUE;
}

uint8_t spi_open(xSPIHandle spid)
{
	return TRUE;
}

uint8_t spi_close(xSPIHandle spid)
{
	return TRUE;
}

void spi_write(xSPIHandle spid, uint8_t data)
{
	spi_trans(spid, data);
}

uint8_t spi_read(xSPIHandle spid)
{
	return spi_trans(spid, 0);
}

void spi_write_array(xSPIHandle spid, const uint8_t * txbuf, uint16_t len)
{
	spi_trans_array(spid, txbuf, NULL, len);
}

void spi_read_array(xSPIHandle spid, uint8_t * rxbuf, uint16_t len)
{
	spi_trans_array(spid, NULL, rxbuf, len);
}

uint8_t spi_trans(xSPIHandle spid, uint8_t data)
{
	uint32_t rxdat;
	spi_trans_array(spid, &data, &rxbuf, 1);
	return (uint8_t) (rxdat&0x000000FF);
}

void spi_trans_array(xSPIHandle spid, uint8_t * txbuf, uint8_t * rxbuf, uint16_t len)
{
	struct spi_ioc_transfer spitrans;

	spitrans.tx_buf = (unsigned long) txbuf;
	spitrans.rx_buf = (unsigned long) rxbuf;
	spitrans.len = len;
	spitrans.delay_usecs = 0;
	spitrans.speed_hz = 0;
	spitrans.bits_per_word = 0;

	if (ioctl(spid, SPI_IOC_MESSAGE(1), &spitrans) < 0)
		perror("ERROR ON SPI TRANSACTION");
}
