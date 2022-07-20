#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

#include "BSPViewer/Graphics/VertexBufferElement.hpp"

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> _elements;
public:
	VertexBufferLayout(std::initializer_list<VertexBufferElement> initializer_list) : _elements(initializer_list) {}

	[[nodiscard]] inline const std::vector<VertexBufferElement>& getElements() const {
		return _elements;
	}

	[[nodiscard]] inline size_t getStride() const {
		size_t stride = 0;

		for (const auto& element : _elements) {
			stride += element.getSize();
		}

		return stride;
	}
};
