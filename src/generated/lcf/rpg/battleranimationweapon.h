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

#ifndef LCF_RPG_BATTLERANIMATIONWEAPON_H
#define LCF_RPG_BATTLERANIMATIONWEAPON_H

// Headers
#include <stdint.h>
#include "lcf/dbstring.h"
#include "lcf/context.h"
#include "lcf/unknown_chunk.h"
#include <ostream>
#include <type_traits>

/**
 * rpg::BattlerAnimationWeapon class.
 */
namespace lcf {
namespace rpg {
	class BattlerAnimationWeapon {
	public:
		int ID = 0;
		DBString name;
		DBString weapon_name;
		int32_t weapon_index = 0;
		/** Chunks liblcf does not know: written back on save. */
		std::vector<UnknownChunk> unknown_chunks;
	};

	inline bool operator==(const BattlerAnimationWeapon& l, const BattlerAnimationWeapon& r) {
		return l.name == r.name
		&& l.weapon_name == r.weapon_name
		&& l.weapon_index == r.weapon_index
		&& l.unknown_chunks == r.unknown_chunks;
	}

	inline bool operator!=(const BattlerAnimationWeapon& l, const BattlerAnimationWeapon& r) {
		return !(l == r);
	}

	std::ostream& operator<<(std::ostream& os, const BattlerAnimationWeapon& obj);

	template <typename F, typename ParentCtx = Context<void,void>>
	void ForEachString(BattlerAnimationWeapon& obj, const F& f, const ParentCtx* parent_ctx = nullptr) {
		const auto ctx1 = Context<BattlerAnimationWeapon, ParentCtx>{ "name", -1, &obj, parent_ctx };
		f(obj.name, ctx1);
		const auto ctx2 = Context<BattlerAnimationWeapon, ParentCtx>{ "weapon_name", -1, &obj, parent_ctx };
		f(obj.weapon_name, ctx2);
		(void)obj;
		(void)f;
		(void)parent_ctx;
	}

} // namespace rpg
} // namespace lcf

#endif
