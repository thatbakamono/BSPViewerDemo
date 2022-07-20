#include <GLFW/glfw3.h>

class GLFW {
public:
	inline GLFW() {
		glfwInit();
	}

	inline ~GLFW() {
		glfwTerminate();
	}

	inline void setContextVersion(int major, int minor) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	}

	inline void setOpenGLProfile(int openGLProfile) {
		glfwWindowHint(GLFW_OPENGL_PROFILE, openGLProfile);
	}

	inline void enableOpenGLForwardCompatibility() {
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	}

	inline void setSwapInterval(int swapInterval) {
		glfwSwapInterval(swapInterval);
	}

	inline void pollEvents() {
		glfwPollEvents();
	}
};
