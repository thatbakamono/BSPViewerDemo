#pragma once

#include "BSPViewer/BSP/Decoder.hpp"

template<typename T>
struct Rgba {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

template<>
Rgba<uint8_t> BSPDecoder::read(MemoryStream& stream) {
    return Rgba<uint8_t> {
    	.r = read<uint8_t>(stream),
    	.g = read<uint8_t>(stream),
    	.b = read<uint8_t>(stream),
    	.a = read<uint8_t>(stream)
    };
}
