/*
 * This file is part of liblcf. Copyright (c) liblcf authors.
 * https://github.com/EasyRPG/liblcf - https://easyrpg.org
 *
 * liblcf is Free/Libre Open Source Software, released under the MIT License.
 * For the full copyright and license information, please view the COPYING
 * file that was distributed with this source code.
 */

#include <algorithm>
#include "lcf/unknown_chunk.h"
#include "log.h"
#include "reader_struct.h"

namespace lcf {

/*
Chunks of a struct that liblcf does not know are kept as raw data.

In XML they are written after the fields of the struct, the data as bytes:

<unknown_chunks>
  <UnknownChunk id="0171">1 0 3</UnknownChunk>
</unknown_chunks>
*/

const char* const UnknownChunks::name = "unknown_chunks";

void UnknownChunks::ReadLcf(std::vector<UnknownChunk>& ref, LcfReader& stream, const LcfReader::Chunk& chunk_info, const char* where) {
	Log::Debug("Unknown Chunk %02X (%" PRIu32 " byte) in lcf at %" PRIX32 " (%s) kept",
			chunk_info.ID, chunk_info.length, stream.Tell(), where);

	UnknownChunk chunk;
	chunk.ID = chunk_info.ID;
	// In blocks: the length of a corrupted chunk can exceed the file
	constexpr uint32_t block_size = 4096;
	uint32_t remaining = chunk_info.length;
	while (remaining > 0) {
		const size_t offset = chunk.data.size();
		const uint32_t block = std::min(remaining, block_size);
		chunk.data.resize(offset + block);
		const size_t read = stream.Read0(chunk.data.data() + offset, 1, block);
		if (read != block) {
			chunk.data.resize(offset + read);
			Log::Warning("%s: Corrupted Chunk 0x%02" PRIx32 " (size: %" PRIu32 "): Read %zu bytes!",
					where, chunk_info.ID, chunk_info.length, chunk.data.size());
			break;
		}
		remaining -= block;
	}
	ref.push_back(std::move(chunk));
}

void UnknownChunks::WriteLcf(const UnknownChunk& ref, LcfWriter& stream) {
	stream.WriteInt(ref.ID);
	stream.WriteInt(ref.data.size());
	if (!ref.data.empty()) {
		stream.Write(ref.data);
	}
}

int UnknownChunks::LcfSize(const std::vector<UnknownChunk>& ref) {
	int result = 0;
	for (const auto& chunk: ref) {
		result += LcfReader::IntSize(chunk.ID);
		result += LcfReader::IntSize(chunk.data.size());
		result += chunk.data.size();
	}
	return result;
}

void UnknownChunks::WriteXml(const std::vector<UnknownChunk>& ref, XmlWriter& stream) {
	if (ref.empty()) {
		return;
	}
	stream.BeginElement(name);
	for (const auto& chunk: ref) {
		stream.BeginElement("UnknownChunk", chunk.ID);
		stream.Write(chunk.data);
		stream.EndElement("UnknownChunk");
	}
	stream.EndElement(name);
}

class UnknownChunksXmlHandler : public XmlHandler {
public:
	UnknownChunksXmlHandler(std::vector<UnknownChunk>& ref) : ref(ref), chunk(NULL) {}

	void StartElement(XmlReader& /* stream */, const char* name, const char** atts) {
		if (strcmp(name, "UnknownChunk") != 0)
			Log::Error("XML: Expecting %s but got %s", "UnknownChunk", name);
		ref.resize(ref.size() + 1);
		chunk = &ref.back();
		for (int i = 0; atts[i] != NULL && atts[i + 1] != NULL; i += 2) {
			if (strcmp(atts[i], "id") == 0)
				chunk->ID = atoi(atts[i + 1]);
		}
	}

	void EndElement(XmlReader& /* stream */, const char* /* name */) {
		chunk = NULL;
	}

	void CharacterData(XmlReader& /* stream */, const std::string& data) {
		if (chunk != NULL)
			XmlReader::Read(chunk->data, data);
	}
private:
	std::vector<UnknownChunk>& ref;
	UnknownChunk* chunk;
};

void UnknownChunks::BeginXml(std::vector<UnknownChunk>& ref, XmlReader& stream) {
	stream.SetHandler(new UnknownChunksXmlHandler(ref));
}

} //namespace lcf
