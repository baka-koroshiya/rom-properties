/***************************************************************************
 * ROM Properties Page shell extension. (librpbase)                        *
 * IRpFile.cpp: File wrapper interface.                                    *
 *                                                                         *
 * Copyright (c) 2016-2019 by David Korth.                                 *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ***************************************************************************/

#include "IRpFile.hpp"
#include "threads/Atomics.h"

// C includes. (C++ namespace)
#include <cassert>
#include <cstdio>

namespace LibRpBase {

// Total reference count for all files.
volatile int IRpFile::ms_refCntTotal = 0;

IRpFile::IRpFile()
	: m_lastError(0)
	, m_refCnt(1)
{
	// Increment the total reference count.
	ATOMIC_INC_FETCH(&ms_refCntTotal);
}

/**
 * Take a reference to this IRpFile* object.
 * @return this
 */
IRpFile *IRpFile::ref(void)
{
	ATOMIC_INC_FETCH(&m_refCnt);
	ATOMIC_INC_FETCH(&ms_refCntTotal);
	return this;
}

/**
 * Unreference this IRpFile* object.
 * If ref_cnt reaches 0, the IRpFile* object is deleted.
 */
void IRpFile::unref(void)
{
	assert(m_refCnt > 0);
	assert(ms_refCntTotal > 0);
	if (ATOMIC_DEC_FETCH(&m_refCnt) <= 0) {
		// All references removed.
		delete this;
	}
	ATOMIC_DEC_FETCH(&ms_refCntTotal);
}

/**
 * Get the last error.
 * @return Last POSIX error, or 0 if no error.
 */
int IRpFile::lastError(void) const
{
	return m_lastError;
}

/**
 * Clear the last error.
 */
void IRpFile::clearError(void)
{
	m_lastError = 0;
}

/**
 * Get a single character (byte) from the file
 * @return Character from file, or EOF on end of file or error.
 */
int IRpFile::getc(void)
{
	uint8_t buf;
	size_t sz = this->read(&buf, 1);
	return (sz == 1 ? buf : EOF);
}

/**
 * Un-get a single character (byte) from the file.
 *
 * Note that this implementation doesn't actually
 * use a character buffer; it merely decrements the
 * seek pointer by 1.
 *
 * @param c Character. (ignored!)
 * @return 0 on success; non-zero on error.
 */
int IRpFile::ungetc(int c)
{
	RP_UNUSED(c);	// TODO: Don't ignore this?

	// TODO: seek() overload that supports SEEK_CUR?
	int64_t pos = tell();
	if (pos <= 0) {
		// Cannot ungetc().
		return -1;
	}

	return this->seek(pos-1);
}

/**
 * Seek to the specified address, then read data.
 * @param pos	[in] Requested seek address.
 * @param ptr	[out] Output data buffer.
 * @param size	[in] Amount of data to read, in bytes.
 * @return Number of bytes read on success; 0 on seek or read error.
 */
size_t IRpFile::seekAndRead(int64_t pos, void *ptr, size_t size)
{
	int ret = this->seek(pos);
	if (ret != 0) {
		// Seek error.
		return 0;
	}
	return this->read(ptr, size);
}

}
