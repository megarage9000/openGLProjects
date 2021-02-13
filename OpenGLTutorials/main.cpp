
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>



int main() {
	
	// Initialise glfw
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	// Create window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello triangle", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR, could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Start GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Fetch GLEW version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Render %s\n", renderer);
	printf("OpenGL version supported %s\n", version);


	glEnable(GL_DEPTH_TEST);  // enable depth-testing
	glDepthFunc(GL_LESS); // deoth-testing interprets a smaller value as "closer"

	glfwTerminate();
	return 0;
}