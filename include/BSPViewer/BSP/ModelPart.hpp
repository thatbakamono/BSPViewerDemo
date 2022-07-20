#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>
#include "BSPViewer/BSP/Decoder.hpp"
#include "BSPViewer/BSP/Rgba.hpp"
#include "BSPViewer/BSP/Vector3.hpp"

constexpr uint32_t HAS_VERTEX = 1 << 8;
constexpr uint32_t HAS_RECIPROCAL_HOMOGENEOUS_W = 1 << 9;
constexpr uint32_t HAS_NORMAL = 1 << 10;
constexpr uint32_t HAS_DIFFUSE = 1 << 11;
constexpr uint32_t HAS_WEIGHT = 1 << 12;
constexpr uint32_t HAS_INDICES = 1 << 13;
constexpr uint32_t UV_COUNT_MASK = 0xFF;

struct Vertex {
    std::optional<Vector3> vertex;
    std::optional<Vector3> normal;
    std::optional<float> reciprocalHomogeneousW;
    std::optional<Rgba<uint8_t>> diffuse;
    std::optional<float> weight;
    std::optional<std::tuple<uint16_t, uint16_t>> indices;
    std::vector<std::tuple<float, float>> uvs;
};

template<>
Vertex BSPDecoder::read(MemoryStream& stream, uint32_t flags) {
    std::optional<Vector3> vertex;

    if (flags & HAS_VERTEX) {
        vertex = std::make_optional<Vector3>(read<Vector3>(stream));
    }

    std::optional<Vector3> normal;

    if (flags & HAS_NORMAL) {
        normal = std::make_optional<Vector3>(read<Vector3>(stream));
    }

    std::optional<float> reciprocalHomogeneousW;
    
    if (flags & HAS_RECIPROCAL_HOMOGENEOUS_W) {
        reciprocalHomogeneousW = std::make_optional<float>(read<float>(stream));
    }
    
    std::optional<Rgba<uint8_t>> diffuse;
    
    if (flags & HAS_DIFFUSE) {
        diffuse = std::make_optional<Rgba<uint8_t>>(read<Rgba<uint8_t>>(stream));
    }

    std::optional<float> weight;
    
    if (flags & HAS_WEIGHT) {
        weight = std::make_optional<float>(read<float>(stream));
    }

    std::optional<std::tuple<uint16_t, uint16_t>> indices;
    
    if (flags & HAS_INDICES) {
        indices = std::make_optional<std::tuple<uint16_t, uint16_t>>(readTuple<uint16_t, uint16_t>(stream));
    }

    std::vector<std::tuple<float, float>> uvs;

    uvs.reserve(flags & UV_COUNT_MASK);

    for (uint32_t i = 0; i < (flags & UV_COUNT_MASK); i++) {
        uvs.emplace_back(read<float>(stream), read<float>(stream));
    }

    return Vertex {
        .vertex = vertex,
        .normal = normal,
        .reciprocalHomogeneousW = reciprocalHomogeneousW,
        .diffuse = diffuse,
        .weight = weight,
        .indices = indices,
        .uvs = uvs,
    };
}

struct Index {
	uint32_t index0;
	uint32_t index1;
	uint32_t index2;
};

template<>
Index BSPDecoder::read(MemoryStream& stream) {
    return Index {
        .index0 = read<uint32_t>(stream),
        .index1 = read<uint32_t>(stream),
        .index2 = read<uint32_t>(stream)
    };
}

struct ModelPart {
    uint32_t readAccessFlags;
    uint32_t vertexReadFlags;
    uint32_t writeAccessFlags;
    uint32_t vertexWriteFlags;
    uint32_t hintFlags;
    uint32_t constantFlags;
    uint32_t vertexFlags;
    uint32_t renderFlags;
    uint16_t trianglesCount;
    uint16_t stripsCount;
    uint16_t stripTrianglesCount;
    uint32_t materialHash;
    int32_t triangleIndex0;
    int32_t triangleIndex1;
    int32_t vertexIndex0;
    int32_t vertexIndex1;
    uint32_t layerZ;
    uint32_t floorFlags;
    uint32_t flags;
    uint32_t lightingSid;
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

template<>
ModelPart BSPDecoder::read(MemoryStream& stream) {
    auto readAccessFlags = read<uint32_t>(stream);
    auto vertexReadFlags = read<uint32_t>(stream);
    auto writeAccessFlags = read<uint32_t>(stream);
    auto vertexWriteFlags = read<uint32_t>(stream);
    auto hintFlags = read<uint32_t>(stream);
    auto constantFlags = read<uint32_t>(stream);
    auto vertexFlags = read<uint32_t>(stream);
    auto renderFlags = read<uint32_t>(stream);
    auto vertexCount = read<uint32_t>(stream);
    auto trianglesCount = read<uint16_t>(stream);
    auto stripsCount = read<uint16_t>(stream);
    auto stripTrianglesCount = read<uint16_t>(stream);

    auto materialHash = read<uint32_t>(stream);
    auto triangleIndex0 = read<int32_t>(stream);
    auto triangleIndex1 = read<int32_t>(stream);
    auto vertexIndex0 = read<int32_t>(stream);
    auto vertexIndex1 = read<int32_t>(stream);
    auto layerZ = read<uint32_t>(stream);

    auto floorFlags = read<uint32_t>(stream);
    auto flags = read<uint32_t>(stream);
    auto lightingSid = read<uint32_t>(stream);

    std::vector<Vertex> vertices;

    vertices.reserve(vertexCount);

    for (uint32_t i = 0; i < vertexCount; i++) {
        vertices.push_back(read<Vertex>(stream, vertexFlags));
    }

    std::vector<Index> indices;

    indices.reserve(trianglesCount);

    for (int32_t i = 0; i < trianglesCount; i++) {
        indices.push_back(read<Index>(stream));
    }

    return ModelPart {
        .readAccessFlags = readAccessFlags,
        .vertexReadFlags = vertexReadFlags,
        .writeAccessFlags = writeAccessFlags,
        .vertexWriteFlags = vertexWriteFlags,
        .hintFlags = hintFlags,
        .constantFlags = constantFlags,
        .vertexFlags = vertexFlags,
        .renderFlags = renderFlags,
        .trianglesCount = trianglesCount,
        .stripsCount = stripsCount,
        .stripTrianglesCount = stripTrianglesCount,
        .materialHash = materialHash,
        .triangleIndex0 = triangleIndex0,
        .triangleIndex1 = triangleIndex1,
        .vertexIndex0 = vertexIndex0,
        .vertexIndex1 = vertexIndex1,
        .layerZ = layerZ,
        .floorFlags = floorFlags,
        .flags = flags,
        .lightingSid = lightingSid,
        .vertices = vertices,
        .indices = indices,
    };
}
