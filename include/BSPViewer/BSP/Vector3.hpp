#pragma once

#include "BSPViewer/BSP/Decoder.hpp"

struct Vector3 {
	float x;
	float y;
	float z;
};

template<>
Vector3 BSPDecoder::read(MemoryStream& stream) {
	return Vector3 {
		.x = read<float>(stream),
		.y = read<float>(stream),
		.z = read<float>(stream)
	};
}
