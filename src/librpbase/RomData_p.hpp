/***************************************************************************
 * ROM Properties Page shell extension. (librpbase)                        *
 * RomData_p.hpp: ROM data base class. (PRIVATE CLASS)                     *
 *                                                                         *
 * Copyright (c) 2016-2018 by David Korth.                                 *
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

#ifndef __ROMPROPERTIES_LIBRPBASE_ROMDATA_P_HPP__
#define __ROMPROPERTIES_LIBRPBASE_ROMDATA_P_HPP__

// C includes.
#include <stdint.h>

// C++ includes.
#include <vector>

#include "RomData.hpp"

// TODO: Remove from here and add to each RomData subclass?
#include "RomFields.hpp"
#include "RomMetaData.hpp"

namespace LibRpBase {

class IRpFile;
class RomData;
class RomFields;
class rp_image;

class RomData;
class RomDataPrivate
{
	public:
		/**
		 * Initialize a RomDataPrivate storage class.
		 *
		 * @param q RomData class.
		 * @param file ROM file.
		 */
		RomDataPrivate(RomData *q, IRpFile *file);

		virtual ~RomDataPrivate();

	private:
		RP_DISABLE_COPY(RomDataPrivate)
	protected:
		friend class RomData;
		RomData *const q_ptr;

	public:
		volatile int ref_cnt;		// Reference count.
		bool isValid;			// Subclass must set this to true if the ROM is valid.
		IRpFile *file;			// Open file.
		RomFields *const fields;	// ROM fields. (NOTE: allocated by the base class)
		RomMetaData *metaData;		// ROM metadata. (NOTE: nullptr initially.)

		// Class name for user configuration. (ASCII) (default is nullptr)
		const char *className;
		// File type. (default is FTYPE_ROM_IMAGE)
		RomData::FileType fileType;

	public:
		/** Convenience functions. **/

		/**
		 * Get the GameTDB URL for a given game.
		 * @param system System name.
		 * @param type Image type.
		 * @param region Region name.
		 * @param gameID Game ID.
		 * @param ext File extension, e.g. ".png" or ".jpg".
		 * TODO: PAL multi-region selection?
		 * @return GameTDB URL.
		 */
		static std::string getURL_GameTDB(
			const char *system, const char *type,
			const char *region, const char *gameID,
			const char *ext);

		/**
		 * Get the GameTDB cache key for a given game.
		 * @param system System name.
		 * @param type Image type.
		 * @param region Region name.
		 * @param gameID Game ID.
		 * @param ext File extension, e.g. ".png" or ".jpg".
		 * TODO: PAL multi-region selection?
		 * @return GameTDB cache key.
		 */
		static std::string getCacheKey_GameTDB(
			const char *system, const char *type,
			const char *region, const char *gameID,
			const char *ext);

		/**
		 * Select the best size for an image.
		 * @param sizeDefs Image size definitions.
		 * @param size Requested thumbnail dimension. (assuming a square thumbnail)
		 * @return Image size definition, or nullptr on error.
		 */
		static const RomData::ImageSizeDef *selectBestSize(const std::vector<RomData::ImageSizeDef> &sizeDefs, int size);

		/**
		 * Convert an ASCII release date in YYYYMMDD format to Unix time_t.
		 * This format is used by Sega Saturn and Dreamcast.
		 * @param ascii_date ASCII release date. (Must be 8 characters.)
		 * @return Unix time_t, or -1 on error.
		 */
		static time_t ascii_yyyymmdd_to_unix_time(const char *ascii_date);

		/**
		 * Convert a BCD timestamp to Unix time.
		 * @param bcd_tm BCD timestamp. (YY YY MM DD HH mm ss)
		 * @param size Size of BCD timestamp. (4 or 7)
		 * @return Unix time, or -1 if an error occurred.
		 *
		 * NOTE: -1 is a valid Unix timestamp (1970/01/01), but is
		 * not likely to be valid, since the first programmable
		 * video game consoles were released in the late 1970s.
		 */
		static time_t bcd_to_unix_time(const uint8_t *bcd_tm, size_t size);
};

}

#endif /* __ROMPROPERTIES_LIBRPBASE_ROMDATA_P_HPP__ */
