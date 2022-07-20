#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include <glad/glad.h>

#include "BSPViewer/Graphics/Buffer.hpp"
#include "BSPViewer/Graphics/VertexBufferLayout.hpp"

class VertexBuffer : public Buffer {
private:
	std::vector<Vector3> _vertices;
	size_t _verticesCount = 0;
public:
	VertexBuffer(std::vector<Vector3> vertices, const VertexBufferLayout& layout) {
		_verticesCount = vertices.size();
		_vertices = std::move(vertices);

		bind();

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

		size_t offset = 0;

		for (size_t i = 0; i < layout.getElements().size(); i++) {
			const auto& element = layout.getElements()[i];

			glVertexAttribPointer(
				i, 
				element.getCount(), 
				element.getRawType(), 
				element.isNormalized(), 
				layout.getStride(), 
				reinterpret_cast<void*>(offset)
			);
			glEnableVertexAttribArray(i);

			offset += element.getSize();
		}
	}

	~VertexBuffer() = default;

	VertexBuffer(const VertexBuffer& other) = delete;

	VertexBuffer(VertexBuffer&& other) {
		_bufferId = std::exchange(other._bufferId, 0);
		_verticesCount = std::exchange(other._verticesCount, 0);
		other._isMoved = true;
	}

    VertexBuffer& operator =(const VertexBuffer& other) = delete;

    VertexBuffer& operator =(VertexBuffer&& other) {
		_bufferId = std::exchange(other._bufferId, 0);
		_verticesCount = std::exchange(other._verticesCount, 0);
		other._isMoved = true;

    	return *this;
    }

    [[nodiscard]] inline size_t getVerticesCount() const {
		return _verticesCount;
	}

	void bind() override {
		glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	}
};
