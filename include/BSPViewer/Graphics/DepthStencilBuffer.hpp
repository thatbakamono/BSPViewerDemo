#pragma once

#include <cstdint>

#include "BSPViewer/Graphics/RenderBuffer.hpp"

class DepthStencilBuffer : public RenderBuffer {
public:
	DepthStencilBuffer(uint32_t width, uint32_t height) : RenderBuffer() {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}
};
