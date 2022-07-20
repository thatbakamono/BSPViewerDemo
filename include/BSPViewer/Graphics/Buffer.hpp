#pragma once

#include <cstdint>

class Buffer {
protected:
	uint32_t _bufferId;

	bool _isMoved = false;
public:
	Buffer() {
		glGenBuffers(1, &_bufferId);
	}

	~Buffer() {
		if (!_isMoved) {
			glDeleteBuffers(1, &_bufferId);
		}
	}

	virtual void bind() = 0;
};
