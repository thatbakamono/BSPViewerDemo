#pragma once

#include <cstdint>
#include <optional>
#include <utility>

#include <glad/glad.h>

#include "BSPViewer/Graphics/IndexBuffer.hpp"
#include "BSPViewer/Graphics/VertexBuffer.hpp"

class VertexArray {
private:
	uint32_t _vertexArrayId;

	std::optional<VertexBuffer> _attachedVertexBuffer;
	std::optional<IndexBuffer> _attachedIndexBuffer;

	bool _isMoved = false;
public:
	VertexArray() {
		glGenVertexArrays(1, &_vertexArrayId);
	}

	VertexArray(const VertexArray& other) = delete;

	VertexArray(VertexArray&& other) noexcept {
		_vertexArrayId = other._vertexArrayId;
    	_attachedVertexBuffer = std::exchange(other._attachedVertexBuffer, std::nullopt);
    	_attachedIndexBuffer = std::exchange(other._attachedIndexBuffer, std::nullopt);
    	other._isMoved = true;
	}

	~VertexArray() {
		if (!_isMoved) {
			glDeleteVertexArrays(1, &_vertexArrayId);
		}
	}

    VertexArray& operator =(const VertexArray& other) = delete;

    VertexArray& operator =(VertexArray&& other) noexcept {
    	_vertexArrayId = other._vertexArrayId;
    	_attachedVertexBuffer = std::exchange(other._attachedVertexBuffer, std::nullopt);
    	_attachedIndexBuffer = std::exchange(other._attachedIndexBuffer, std::nullopt);
    	other._isMoved = true;

    	return *this;
    }

    void attachVertexBuffer(VertexBuffer vertexBuffer) {
    	_attachedVertexBuffer = std::make_optional<VertexBuffer>(std::move(vertexBuffer));
    }

    [[nodiscard]] std::optional<VertexBuffer>& getAttachedVertexBuffer() {
    	return _attachedVertexBuffer;
    }

    void attachIndexBuffer(IndexBuffer indexBuffer) {
    	_attachedIndexBuffer = std::make_optional<IndexBuffer>(std::move(indexBuffer));
    }

    [[nodiscard]] std::optional<IndexBuffer>& getAttachedIndexBuffer() {
    	return _attachedIndexBuffer;
    }

	void bind() {
		glBindVertexArray(_vertexArrayId);
	}
};
