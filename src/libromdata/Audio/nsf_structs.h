/***************************************************************************
 * ROM Properties Page shell extension. (libromdata)                       *
 * nsf_structs.h: NSF audio data structures.                               *
 *                                                                         *
 * Copyright (c) 2018 by David Korth.                                      *
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

// References:
// - http://vgmrips.net/wiki/NSF_File_Format

#ifndef __ROMPROPERTIES_LIBROMDATA_AUDIO_NSF_STRUCTS_H__
#define __ROMPROPERTIES_LIBROMDATA_AUDIO_NSF_STRUCTS_H__

#include "librpbase/common.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

/**
 * Nintendo Sound Format. (NES/Famicom)
 * All fields are little-endian.
 */
#define NSF_MAGIC "NESM\x1A\x01"
typedef struct PACKED _NSF_Header {
	char magic[6];			// [0x000] "NESM\x1A\x01"
	uint8_t track_count;		// [0x006] Number of tracks
	uint8_t default_track;		// [0x007] Default track number, plus one.
	uint16_t load_address;		// [0x008] Load address. (must be $8000-$FFFF)
	uint16_t init_address;		// [0x00A] Init address. (must be $8000-$FFFF)
	uint16_t play_address;		// [0x00C] Play address.
	char title[32];			// [0x00E] Title. (ASCII, NULL-terminated)
	char composer[32];		// [0x02E] Composer. (ASCII, NULL-terminated)
	char copyright[32];		// [0x04E] Copyright. (ASCII, NULL-terminated)
	uint16_t ntsc_framerate;	// [0x06E] NTSC framerate, in microseconds.
					//         (not always in use)
	uint8_t bankswitching[8];	// [0x070] If non-zero, initial bank setting for
					//         $8xxx, $9xxx, etc.
	uint16_t pal_framerate;		// [0x078] PAL framerate, in microseconds.
					//         (not always in use)
	uint8_t tv_system;		// [0x07A] TV system. (See NSF_TV_System_e.)
	uint8_t expansion_audio;	// [0x07B] Expansion audio. (See NSF_Expansion_e.)
	uint8_t reserved[4];		// [0x07C] Reserved. (must be 0)
} NSF_Header;
ASSERT_STRUCT(NSF_Header, 128);

/**
 * NSF: TV system.
 */
typedef enum {
	NSF_TV_NTSC	= 0,
	NSF_TV_PAL	= 1,
	NSF_TV_BOTH	= 2,

	NSF_TV_MAX
} NSF_TV_System_e;

/**
 * NSF: Expansion audio.
 * NOTE: This is a bitfield.
 */
typedef enum {
	NSF_EXP_VRC6		= (1 << 0),	// Konami VRC6
	NSF_EXP_VRC7		= (1 << 1),	// Konami VRC7
	NSF_EXP_2C33		= (1 << 2),	// 2C33 (Famicom Disk System)
	NSF_EXP_MMC5		= (1 << 3),	// MMC5
	NSF_EXP_N163		= (1 << 4),	// Namco N163
	NSF_EXP_SUNSOFT_5B	= (1 << 5),	// Sunsoft 5B
} NSF_Expansion_e;

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __ROMPROPERTIES_LIBROMDATA_AUDIO_NSF_STRUCTS_H__ */