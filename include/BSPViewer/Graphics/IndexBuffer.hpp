#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include <glad/glad.h>

#include "BSPViewer/Graphics/Buffer.hpp"

class IndexBuffer : public Buffer {
private:
	std::vector<uint32_t> _indices;
	size_t _indicesCount = 0;
public:
	IndexBuffer(std::vector<uint32_t> indices) {
		_indicesCount = indices.size();
		_indices = std::move(indices);

		bind();

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
	}

	~IndexBuffer() = default;

	IndexBuffer(const IndexBuffer& other) = delete;

	IndexBuffer(IndexBuffer&& other) {
		_bufferId = std::exchange(other._bufferId, 0);
		_indicesCount = std::exchange(other._indicesCount, 0);
		other._isMoved = true;
	}

	IndexBuffer& operator =(const IndexBuffer& other) = delete;

	IndexBuffer& operator =(IndexBuffer&& other) {
		_bufferId = std::exchange(other._bufferId, 0);
		_indicesCount = std::exchange(other._indicesCount, 0);
		other._isMoved = true;
		
		return *this;
	}

	[[nodiscard]] inline size_t getIndicesCount() const {
		return _indicesCount;
	}

	void bind() override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
	}
};
