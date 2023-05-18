#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGLCommon/.h/LinearAlgebra.h";
#include "../OpenGLCommon/.h/LinearTransformations.h";
#include "../OpenGLCommon/.h/Log.h";

// Window Size
#define G_WIN_WIDTH 1280
#define G_WIN_HEIGHT 720

// Frame Buffer Size
#define G_FB_WIDTH 1080
#define G_FB_HEIGHT 720

using namespace LinearAlgebra;

// GLFW Callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);

int main() {
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3");
		return -1;
	}

	// Change opengl version here
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable Anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(G_WIN_WIDTH, G_WIN_HEIGHT, "Global Illumination Project", NULL, NULL);

	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_window_framebuffer_callback);

	if (!window) {
		fprintf(stderr, "ERROR, could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Render %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

}


void glfw_error_callback(int error, const char* description) {

}

void glfw_window_resize_callback(GLFWwindow* window, int width, int height) {

}
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height) {

}
