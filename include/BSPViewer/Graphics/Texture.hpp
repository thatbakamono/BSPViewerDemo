#pragma once

#include <cstdint>

#include <glad/glad.h>

class Texture {
protected:
	uint32_t _textureId;
	bool _moved = false;
public:
	Texture() {
		glGenTextures(1, &_textureId);
	}

	~Texture() {
		if (!_moved) {
			glDeleteTextures(1, &_textureId);
		}
	}

	[[nodiscard]] inline getId() const {
		return _textureId;
	}
};
