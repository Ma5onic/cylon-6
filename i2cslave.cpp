//
// i2cslave.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2016  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "i2cslave.h"
#include <circle/memio.h>
#include <circle/bcm2835.h>
#include <circle/synchronize.h>
#include <assert.h>
#include <circle/logger.h>

#define DR_DATA__MASK		0xFF

#define RSR_UE			(1 << 1)
#define RSR_OE			(1 << 0)

#define CR_RXE			(1 << 9)
#define CR_TXE			(1 << 8)
#define CR_BRK			(1 << 7)
#define CR_I2C			(1 << 2)
#define CR_EN			(1 << 0)

#define FR_RXFLEVEL(reg)	(((reg) >> 11) & 0x1F)
#define FR_TXFLEVEL(reg)	(((reg) >> 6)  & 0x1F)
#define FR_RXBUSY		(1 << 5)
#define FR_TXFE			(1 << 4)
#define FR_RXFF			(1 << 3)
#define FR_TXFF			(1 << 2)
#define FR_RXFE			(1 << 1)
#define FR_TXBUSY		(1 << 0)

#define IFLS_RXIFLSEL__SHIFT	3
#define IFLS_TXIFLSEL__SHIFT	0

#define IMSC_OEIM		(1 << 3)
#define IMSC_BEIM		(1 << 2)
#define IMSC_TXIM		(1 << 1)
#define IMSC_RXIM		(1 << 0)

#define RIS_OERIS		(1 << 3)
#define RIS_BERIS		(1 << 2)
#define RIS_TXRIS		(1 << 1)
#define RIS_RXRIS		(1 << 0)

#define MIS_OEMIS		(1 << 3)
#define MIS_BEMIS		(1 << 2)
#define MIS_TXMIS		(1 << 1)
#define MIS_RXMIS		(1 << 0)

#define ICR_OEIC		(1 << 3)
#define ICR_BEIC		(1 << 2)
#define ICR_TXIC		(1 << 1)
#define ICR_RXIC		(1 << 0)

CI2CSlave::CI2CSlave (u8 ucAddress, CTimer *_m_Timer)
:	m_ucAddress (ucAddress),
	m_SDA (18, GPIOModeAlternateFunction3),
	m_SCL (19, GPIOModeAlternateFunction3)
{
	m_Timer = _m_Timer;
}

CI2CSlave::~CI2CSlave (void)
{
}

boolean CI2CSlave::Initialize (void)
{
	PeripheralEntry ();

	write32 (ARM_BSC_SPI_SLAVE_SLV, m_ucAddress);

	write32 (ARM_BSC_SPI_SLAVE_IMSC, 0);

	write32 (ARM_BSC_SPI_SLAVE_CR, CR_I2C | CR_EN);

	PeripheralExit ();

	return TRUE;
}

int CI2CSlave::Read (void *pBuffer, unsigned nCount, u8 timeout)
{
	if (nCount == 0)
	{
		return -1;
	}

	u8 *pData = (u8 *) pBuffer;
	assert (pData != 0);

	PeripheralEntry ();

	write32 (ARM_BSC_SPI_SLAVE_RSR, 0);
	write32 (ARM_BSC_SPI_SLAVE_CR, read32 (ARM_BSC_SPI_SLAVE_CR) | CR_RXE);

	int nResult = 0;
	int time_start = m_Timer->GetUptime();

	while (nCount-- > 0)
	{
		while (read32 (ARM_BSC_SPI_SLAVE_FR) & FR_RXFE)
		{
			// do nothing
			if ((m_Timer->GetUptime() - time_start) > timeout) 
			{
				nResult = -2;
				goto READ_EXIT;
			}
		}

		if (read32 (ARM_BSC_SPI_SLAVE_RSR) & RSR_OE)
		{
			nResult = -1;

			break;
		}

		*pData++ = read32 (ARM_BSC_SPI_SLAVE_DR) & DR_DATA__MASK;

		nResult++;
	}

	if (nResult > 0)
	{
		// wait for transfer to stop
		while (read32 (ARM_BSC_SPI_SLAVE_FR) & FR_RXBUSY)
		{
			if (read32 (ARM_BSC_SPI_SLAVE_RSR) & RSR_OE)
			{
				nResult = -1;

				break;
			}
		}
	}

READ_EXIT:
	write32 (ARM_BSC_SPI_SLAVE_CR, read32 (ARM_BSC_SPI_SLAVE_CR) & ~CR_RXE);

	PeripheralExit ();

	return nResult;
}

int CI2CSlave::Write (const void *pBuffer, unsigned nCount, u8 timeout)
{
	const u8 *pData = (const u8 *) pBuffer;

	PeripheralEntry ();

	write32 (ARM_BSC_SPI_SLAVE_RSR, 0);
	write32 (ARM_BSC_SPI_SLAVE_CR, read32 (ARM_BSC_SPI_SLAVE_CR) | CR_TXE);

	boolean bTxActive = FALSE;

	int count_left = 16;
	if (nCount<=16)
		count_left = nCount;

	for (int i=0;i<count_left;i++)
		write32 (ARM_BSC_SPI_SLAVE_DR, *pData++);
	count_left = nCount - count_left;

	int FR=0;
	int nResult=0;
	int FIFO_FREE = 0;
	int count_to_write = 0;
	int time_start = m_Timer->GetUptime();

	while(count_left>0)
	{
		FR = read32 (ARM_BSC_SPI_SLAVE_FR);
		FIFO_FREE = 16 - FR_TXFLEVEL(FR);
		
		count_to_write = count_left > FIFO_FREE ? FIFO_FREE:count_left;
		count_left -= count_to_write;

		while(count_to_write-- > 0)
			write32 (ARM_BSC_SPI_SLAVE_DR, *pData++);
	
		if (FIFO_FREE == 0 && (m_Timer->GetUptime() - time_start) > timeout)
		{
			nResult = -2;
			goto WRITE_EXIT;
		}
		if (read32 (ARM_BSC_SPI_SLAVE_RSR) & RSR_UE)
		{
			CLogger::Get() -> Write("i2cslave.cpp", LogDebug, "UnderRun! Too slow to fill FIFO");
			nResult = -1;
			goto WRITE_EXIT;
		}
	}
	nResult = nCount - count_left;

	if (nResult > 0)
	{
		if (!bTxActive)
		{
			// wait for transfer to start
			while (!(read32 (ARM_BSC_SPI_SLAVE_FR) & FR_TXBUSY))
			{
				// do nothing
			}
		}

		// wait for transfer to stop
		while (read32 (ARM_BSC_SPI_SLAVE_FR) & FR_TXBUSY)
		{
			if (read32 (ARM_BSC_SPI_SLAVE_RSR) & RSR_UE)
			{
				nResult = -1;

				break;
			}
		}
	}

WRITE_EXIT:
	write32 (ARM_BSC_SPI_SLAVE_CR, read32 (ARM_BSC_SPI_SLAVE_CR) & ~CR_TXE);
	PeripheralExit ();
	return nResult;
}
