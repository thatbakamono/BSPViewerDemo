#include <cstdint>
#include "BSPViewer/BSP/ChunkType.hpp"
#include "BSPViewer/BSP/Decoder.hpp"

struct ChunkHeader {
	ChunkType type;
	int32_t size;
	int32_t version;
};

template<>
ChunkHeader BSPDecoder::read(MemoryStream& stream) {
	auto type = static_cast<ChunkType>(read<int32_t>(stream));
	auto size = read<int32_t>(stream);
	auto version = read<int32_t>(stream);

	return ChunkHeader {
		.type = type,
		.size = size,
		.version = version,
	};
}
