/*
 * This file is part of liblcf. Copyright (c) liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#include <sstream>
#include "doctest.h"
#include "lcf/config.h"
#include "lcf/ldb/reader.h"
#include "lcf/lmu/reader.h"
#include "lcf/rpg/database.h"
#include "lcf/rpg/map.h"

using namespace lcf;

// Chunk IDs that neither liblcf nor the Maniac Patch use
static rpg::Database MakeDatabase() {
	rpg::Database db;
	db.actors.resize(1);
	db.actors[0].ID = 1;
	db.actors[0].name = DBString("Alex");
	db.actors[0].unknown_chunks = { { 0x60, { 1, 2, 3 } }, { 0x70, {} } };
	db.terms.unknown_chunks = { { 0xB0, { 0x41, 0x42 } } };
	db.system.unknown_chunks = { { 0x70, { 0x81, 0x00 } } };
	db.unknown_chunks = { { 0x0A, { 9 } }, { 0x30, { 7 } } };
	return db;
}

static rpg::Map MakeMap() {
	rpg::Map map;
	map.unknown_chunks = { { 0x40, { 0 } }, { 0x70, { 0xFF } } };
	map.events.resize(1);
	map.events[0].ID = 1;
	map.events[0].unknown_chunks = { { 0x10, { 5 } } };
	map.events[0].pages.resize(1);
	map.events[0].pages[0].ID = 1;
	map.events[0].pages[0].unknown_chunks = { { 0x30, { 1, 1 } } };
	return map;
}

TEST_SUITE_BEGIN("UnknownChunk");

TEST_CASE("LcfRoundTrip") {
	const auto db = MakeDatabase();
	std::stringstream ldb;
	REQUIRE(LDB_Reader::Save(ldb, db, "1252"));
	auto db2 = LDB_Reader::Load(ldb, "1252");
	REQUIRE(db2 != nullptr);
	CHECK_EQ(db2->actors.size(), 1);
	CHECK_EQ(db2->actors[0].name, db.actors[0].name);
	CHECK(db2->actors[0].unknown_chunks == db.actors[0].unknown_chunks);
	CHECK(db2->terms.unknown_chunks == db.terms.unknown_chunks);
	CHECK(db2->system.unknown_chunks == db.system.unknown_chunks);
	CHECK(db2->unknown_chunks == db.unknown_chunks);

	const auto map = MakeMap();
	std::stringstream lmu;
	REQUIRE(LMU_Reader::Save(lmu, map, EngineVersion::e2k3, "1252"));
	auto map2 = LMU_Reader::Load(lmu, "1252");
	REQUIRE(map2 != nullptr);
	CHECK(*map2 == map);
}

TEST_CASE("LcfWrittenInPlace") {
	// written back where they were: ascending order of the IDs
	const auto db = MakeDatabase();
	std::stringstream ldb;
	REQUIRE(LDB_Reader::Save(ldb, db, "1252"));
	const std::string bytes = ldb.str();
	// Database: 0x0A before the first field (0x0B), right after the header,
	// and 0x30 after the last field (0x21)
	CHECK_EQ(bytes.substr(12, 3), std::string("\x0A\x01\x09", 3));
	CHECK_EQ(bytes.substr(bytes.size() - 3), std::string("\x30\x01\x07", 3));
	// Terms: 0xB0 (BER: 0x81 0x30)
	CHECK_NE(bytes.find(std::string("\x81\x30\x02\x41\x42", 5)), std::string::npos);
}

#if LCF_SUPPORT_XML
TEST_CASE("XmlRoundTrip") {
	const auto db = MakeDatabase();
	std::stringstream ldb;
	REQUIRE(LDB_Reader::SaveXml(ldb, db));
	CHECK_NE(ldb.str().find("<UnknownChunk id=\"0096\">1 2 3</UnknownChunk>"), std::string::npos);
	auto db2 = LDB_Reader::LoadXml(ldb);
	REQUIRE(db2 != nullptr);
	CHECK(db2->actors[0].unknown_chunks == db.actors[0].unknown_chunks);
	CHECK(db2->terms.unknown_chunks == db.terms.unknown_chunks);
	CHECK(db2->system.unknown_chunks == db.system.unknown_chunks);
	CHECK(db2->unknown_chunks == db.unknown_chunks);

	const auto map = MakeMap();
	std::stringstream lmu;
	REQUIRE(LMU_Reader::SaveXml(lmu, map, EngineVersion::e2k3));
	auto map2 = LMU_Reader::LoadXml(lmu);
	REQUIRE(map2 != nullptr);
	CHECK(*map2 == map);
}
#endif

TEST_SUITE_END();
