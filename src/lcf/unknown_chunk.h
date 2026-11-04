/*
 * This file is part of liblcf. Copyright (c) liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#ifndef LCF_UNKNOWN_CHUNK_H
#define LCF_UNKNOWN_CHUNK_H

#include <cstdint>
#include <vector>

namespace lcf {

/**
 * A chunk of a struct that liblcf does not know, e.g. one written by a
 * patched RPG_RT (Maniac Patch) or by a newer editor.
 *
 * Its data is kept as read and written back when the struct is saved,
 * among the known chunks in ascending order of the IDs (as RPG Maker
 * does). Loading and saving a file does not lose it.
 */
struct UnknownChunk {
	/** ID of the chunk. */
	int32_t ID = 0;
	/** Raw data of the chunk (without its ID and length). */
	std::vector<uint8_t> data;
};

inline bool operator==(const UnknownChunk& l, const UnknownChunk& r) {
	return l.ID == r.ID && l.data == r.data;
}

inline bool operator!=(const UnknownChunk& l, const UnknownChunk& r) {
	return !(l == r);
}

} // namespace lcf

#endif
