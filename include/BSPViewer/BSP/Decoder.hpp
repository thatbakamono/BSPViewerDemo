#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>
#include "BSPViewer/AccessOutOfBoundsException.hpp"

class MemoryStream {
private:
    std::vector<uint8_t> bytes;
    size_t currentPosition = 0;
public:
    MemoryStream(std::vector<uint8_t> bytes) : bytes(std::move(bytes)) {}

    static MemoryStream fromFile(const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        std::vector<uint8_t> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return MemoryStream(fileContents);
    }

    [[nodiscard]] inline bool isNotEmpty() {
        return currentPosition < bytes.size();
    }

    inline void advance(size_t bytes) {
        currentPosition += bytes;
    }

    [[nodiscard]] uint8_t readByte() {
        if (currentPosition >= bytes.size()) {
            throw AccessOutOfBoundsException();
        }

        return readByteUnsafe();
    }

    template<size_t COUNT>
    [[nodiscard]] std::array<uint8_t, COUNT> readBytes() {
        if (currentPosition + (COUNT - 1) >= bytes.size()) {
            throw AccessOutOfBoundsException();
        }

        std::array<uint8_t, COUNT> readBytes;

        for (size_t i = 0; i < COUNT; i++) {
            readBytes[i] = readByteUnsafe();
        }

        return readBytes;
    }
private:
    [[nodiscard]] inline uint8_t readByteUnsafe() {
        return bytes[currentPosition++];
    }
};

class BSPDecoder {
public:
    template<typename T>
    [[nodiscard]] T read(MemoryStream& stream);

    template<typename T, typename... PARAMETERS>
    [[nodiscard]] T read(MemoryStream& stream, PARAMETERS... parameters);

    template<typename T>
    [[nodiscard]] std::vector<T> readMany(MemoryStream& stream) {
        auto size = read<int32_t>(stream);

        std::vector<T> elements;

        elements.reserve(size);

        for (int32_t i = 0; i < size; i++) {
            elements.push_back(read<T>(stream));
        }

        return elements;
    }

    template<typename T>
    [[nodiscard]] std::optional<T> readOptional(MemoryStream& stream) {
        auto isPresent = read<int32_t>(stream);

        if (isPresent) {
            return std::make_optional<T>(read<T>(stream));
        } else {
            return std::nullopt;
        };
    }

    template<typename... Ts>
    [[nodiscard]] std::tuple<Ts...> readTuple(MemoryStream& stream) {
        return std::tuple<Ts...>((read<Ts>(stream))...);
    }
};

template<>
uint8_t BSPDecoder::read(MemoryStream& stream) {
    return stream.readByte();
}

template<>
uint16_t BSPDecoder::read(MemoryStream& stream) {
    uint8_t secondByte = stream.readByte();
    uint8_t firstByte = stream.readByte();

    return (static_cast<uint16_t>(firstByte) << 8) | static_cast<uint16_t>(secondByte);
}

template<>
int32_t BSPDecoder::read(MemoryStream& stream) {
    uint8_t fourthByte = stream.readByte();
    uint8_t thirdByte = stream.readByte();
    uint8_t secondByte = stream.readByte();
    uint8_t firstByte = stream.readByte();

    return std::bit_cast<int32_t>(
        (static_cast<uint32_t>(firstByte) << 24) 
        | (static_cast<uint32_t>(secondByte) << 16) 
        | (static_cast<uint32_t>(thirdByte) << 8) 
        | (static_cast<uint32_t>(fourthByte) << 0)
    );
}

template<>
uint32_t BSPDecoder::read(MemoryStream& stream) {
    uint8_t fourthByte = stream.readByte();
    uint8_t thirdByte = stream.readByte();
    uint8_t secondByte = stream.readByte();
    uint8_t firstByte = stream.readByte();

    return (static_cast<uint32_t>(firstByte) << 24) 
        | (static_cast<uint32_t>(secondByte) << 16) 
        | (static_cast<uint32_t>(thirdByte) << 8) 
        | (static_cast<uint32_t>(fourthByte) << 0);
}

template<>
float BSPDecoder::read(MemoryStream& stream) {
    uint8_t fourthByte = stream.readByte();
    uint8_t thirdByte = stream.readByte();
    uint8_t secondByte = stream.readByte();
    uint8_t firstByte = stream.readByte();

    return std::bit_cast<float>(
        (static_cast<uint32_t>(firstByte) << 24) 
        | (static_cast<uint32_t>(secondByte) << 16) 
        | (static_cast<uint32_t>(thirdByte) << 8) 
        | (static_cast<uint32_t>(fourthByte) << 0)
    );
}
