/***************************************************************************
 * ROM Properties Page shell extension. (Win32)                            *
 * RpFile_IStream.hpp: IRpFile using an IStream*.                          *
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

#ifndef __ROMPROPERTIES_WIN32_RPFILE_ISTREAM_HPP__
#define __ROMPROPERTIES_WIN32_RPFILE_ISTREAM_HPP__

#include "librpbase/file/IRpFile.hpp"
#include <objidl.h>

// zlib
struct z_stream_s;

class RpFile_IStream : public LibRpBase::IRpFile
{
	public:
		/**
		 * Create an IRpFile using IStream* as the underlying storage mechanism.
		 * @param pStream	[in] IStream*.
		 * @param gzip		[in] If true, handle gzipped files automatically.
		 */
		explicit RpFile_IStream(IStream *pStream, bool gzip = false);
	protected:
		virtual ~RpFile_IStream();	// call unref() instead

	private:
		typedef LibRpBase::IRpFile super;
		RP_DISABLE_COPY(RpFile_IStream)

	public:
		/**
		 * Is the file open?
		 * This usually only returns false if an error occurred.
		 * @return True if the file is open; false if it isn't.
		 */
		bool isOpen(void) const final;

		/**
		 * Close the file.
		 */
		void close(void) final;

		/**
		 * Read data from the file.
		 * @param ptr Output data buffer.
		 * @param size Amount of data to read, in bytes.
		 * @return Number of bytes read.
		 */
		size_t read(void *ptr, size_t size) final;

		/**
		 * Write data to the file.
		 * @param ptr Input data buffer.
		 * @param size Amount of data to read, in bytes.
		 * @return Number of bytes written.
		 */
		size_t write(const void *ptr, size_t size) final;

		/**
		 * Set the file position.
		 * @param pos File position.
		 * @return 0 on success; -1 on error.
		 */
		int seek(int64_t pos) final;

		/**
		 * Get the file position.
		 * @return File position, or -1 on error.
		 */
		int64_t tell(void) final;

		/**
		 * Truncate the file.
		 * @param size New size. (default is 0)
		 * @return 0 on success; negative POSIX error code on error.
		 */
		int truncate(int64_t size = 0) final;

	public:
		/** File properties. **/

		/**
		 * Get the file size.
		 * @return File size, or negative on error.
		 */
		int64_t size(void) final;

		/**
		 * Get the filename.
		 * @return Filename. (May be empty if the filename is not available.)
		 */
		std::string filename(void) const final;

	protected:
		IStream *m_pStream;
		std::string m_filename;

		// zlib
		unsigned int m_z_uncomp_sz;
		unsigned int m_z_filepos;	// position in compressed file
		int64_t m_z_realpos;		// position in real file
		struct z_stream_s *m_pZstm;
		// zlib buffer
		uint8_t *m_pZbuf;
		ULONG m_zbufLen;
		ULONG m_zcurPos;

		/**
		 * Copy the zlib stream from another RpFile_IStream.
		 * @param other
		 * @return 0 on success; non-zero on error.
		 */
		int copyZlibStream(const RpFile_IStream &other);
};

#endif /* __ROMPROPERTIES_WIN32_RPFILE_ISTREAM_HPP__ */
