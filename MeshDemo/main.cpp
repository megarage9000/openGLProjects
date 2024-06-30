#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GarageShaders.h"
#include "Model.h"

// Window Size
int g_win_width = 1280;
int g_win_height = 720;

// Frame Buffer Size
int g_fb_width = 1080;
int g_fb_height = 720;

void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);

GLFWwindow* create_window(int version_major, int version_minor);

int main() {

	
	
	if (!restart_gl_log()) {
		return -1;
	}

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3");
		return -1;
	}

	GLFWwindow* window = create_window(3, 2);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Define Shader here
	Shader MeshShader = Shader("..\\OpenGLCommon\\Shaders\\vertexShader.vert", "..\\OpenGLCommon\\Shaders\\multiLightShader.frag");

	// Define Model here
	Model model { "..\\testMeshes\\tank.obj" };

	while (!glfwWindowShouldClose(window)) {
		model.Draw(MeshShader);
	}
}


GLFWwindow* create_window(int version_major, int version_minor) {
	// Change opengl version here
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable Anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(g_win_width, g_win_height, "Global Illumination Project", NULL, NULL);

	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_window_framebuffer_callback);

	return window;
}

void glfw_window_resize_callback(GLFWwindow* window, int width, int height) {
	g_win_width = width;
	g_win_height = height;
}

void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height) {
	g_fb_width = width;
	g_fb_height = height;
}