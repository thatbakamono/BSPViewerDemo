#pragma once

#include <cstdint>

#include <glad/glad.h>

#include "BSPViewer/Graphics/OpenGLType.hpp"

class VertexBufferElement {
private:
	OpenGLType _type;
	size_t _count;
	bool _isNormalized;
public:
	VertexBufferElement(OpenGLType type, size_t count, bool isNormalized = false) : _type(type), _count(count), _isNormalized(isNormalized) {}

	[[nodiscard]] inline OpenGLType getType() const {
		return _type;
	}

	[[nodiscard]] inline GLenum getRawType() const {
		switch (_type) {
			case OpenGLType::Float: return GL_FLOAT;
			default: return 0;
		}
	}

	[[nodiscard]] inline size_t getCount() const {
		return _count;
	}

	[[nodiscard]] inline bool isNormalized() const {
		return _isNormalized;
	}

	[[nodiscard]] inline size_t getSize() const {
		switch (_type) {
			case OpenGLType::Float: return sizeof(float) * _count;
			default: return 0;
		}
	}
};
