#pragma once

#include <memory>
#include <GLFW/glfw3.h>

class Window {
// TODO
//private:
public:
	std::unique_ptr<GLFWwindow, decltype(glfwDestroyWindow)*> window;
public:
	Window(const std::string& name, int width, int height) : window(glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr), glfwDestroyWindow) {
		// TODO
		if (window.get() == nullptr) {
			
		}
	}

	inline void makeContextCurrent() {
		glfwMakeContextCurrent(window.get());
	}

	inline bool isOpen() {
		return !glfwWindowShouldClose(window.get());
	}

	inline void swapBuffers() {
		glfwSwapBuffers(window.get());
	}
};
