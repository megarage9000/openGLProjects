
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include <stdio.h>

int main() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello triangle", NULL, NULL);

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

	//--- Triangle drawing ---

	// Shaders: Vertex Shader
	const char* vertexShader =
		"#version 410\n"
		"in vec3 vp;"
		"void main(){"
		"	gl_Position = vec4(vp, 1.0);"
		"}";

	// Shaders: Fragment Shader
	const char* fragmentShader =
		"#version 410\n"
		"out vec4 frag_color;"
		"void main(){"
		"	frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	// Shaders: Loading and Compilation
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);
	
	// Shaders: Creating a Shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Our triangle points, going clockwise with xyz float coordinates
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f, // top coordinates
		0.5f, -0.5f, 0.0f, // bottom right coordinates
		-0.5f, -0.5f, 0.0f // bottom left coordinates
	};

	// Generating a Vertex Buffer Object for our Triangle, to 
	// store our points into memory
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Generating a Vertex Array Object for our Triangle, to prevent having to
	// bind and defining memory per vertex buffer object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while (!glfwWindowShouldClose(window)) {

		// Clear drawing surface color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// track events 
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	
	glfwTerminate();
	return 0;
}