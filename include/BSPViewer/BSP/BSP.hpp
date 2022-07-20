#pragma once

#include <vector>
#include "BSPViewer/BSP/ChunkHeader.hpp"
#include "BSPViewer/BSP/Decoder.hpp"
#include "BSPViewer/BSP/ModelPart.hpp"

class BSP {
public:
	std::vector<ModelPart> modelParts;
};

template<>
BSP BSPDecoder::read(MemoryStream& stream) {
	std::vector<ModelPart> modelParts;

	while (stream.isNotEmpty()) {
		auto header = read<ChunkHeader>(stream);

		switch (header.type) {
			case ChunkType::SPMesh:
				modelParts.push_back(read<ModelPart>(stream));
				
				break;
			default:
				stream.advance(header.size);

				break;
		}
	}

	return BSP {
		.modelParts = modelParts,
	};
}
