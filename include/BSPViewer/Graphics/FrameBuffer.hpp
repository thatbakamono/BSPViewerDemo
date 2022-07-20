#pragma once

#include <cstdint>

#include <glad/glad.h>

#include "BSPViewer/Graphics/DepthStencilBuffer.hpp"
#include "BSPViewer/Graphics/Texture2D.hpp"

class FrameBuffer {
private:
	uint32_t _frameBufferId;

	Texture2D _colorAttachment;
	DepthStencilBuffer _depthStencilAttachment;
public:
	FrameBuffer(Texture2D colorAttachment, DepthStencilBuffer depthStencilAttachment) : _colorAttachment(std::move(colorAttachment)), _depthStencilAttachment(std::move(depthStencilAttachment)) {
		glGenFramebuffers(1, &_frameBufferId);

		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment.getId(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilAttachment.getId());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error("Failed to create a frame buffer");
		}
	}

	FrameBuffer(const FrameBuffer& other) = delete;

	~FrameBuffer() {
		glDeleteFramebuffers(1, &_frameBufferId);
	}

    FrameBuffer& operator =(const FrameBuffer& other) = delete;

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
	}

	[[nodiscard]] Texture2D& getColorAttachment() {
		return _colorAttachment;
	}

	[[nodiscard]] DepthStencilBuffer& getDepthStencilAttachment() {
		return _depthStencilAttachment;
	}
};
