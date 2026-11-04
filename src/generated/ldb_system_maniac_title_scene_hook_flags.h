/* !!!! GENERATED FILE - DO NOT EDIT !!!!
 * --------------------------------------
 *
 * This file is part of liblcf. Copyright (c) liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

/*
 * Headers
 */
#include "lcf/ldb/reader.h"
#include "lcf/ldb/chunks.h"
#include "reader_struct.h"

namespace lcf {

// Read System.

template <>
char const* const Flags<rpg::System::ManiacTitleSceneHook_Flags>::name = "System_ManiacTitleSceneHook_Flags";

template <>
decltype(Flags<rpg::System::ManiacTitleSceneHook_Flags>::flag_names) Flags<rpg::System::ManiacTitleSceneHook_Flags>::flag_names = {
	"initialize",
	"create_picture",
	"begin_scene",
	"end_scene",
	"every_frame",
	"selection_changed",
	"new_game"
};

template <>
decltype(Flags<rpg::System::ManiacTitleSceneHook_Flags>::flags_is2k3) Flags<rpg::System::ManiacTitleSceneHook_Flags>::flags_is2k3 = {
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

} //namespace lcf
