#pragma once

#include <cstdint>

#include <glad/glad.h>

class RenderBuffer {
protected:
	uint32_t _renderBufferId;
public:
	RenderBuffer() {
		glGenRenderbuffers(1, &_renderBufferId);
	}

	~RenderBuffer() {
		glDeleteRenderbuffers(1, &_renderBufferId);
	}

	void bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferId);
	}

	[[nodiscard]] inline uint32_t getId() {
		return _renderBufferId;
	}
};
