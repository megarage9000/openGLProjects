#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GarageShaders.h"
#include "GarageEngineObject.h"
#include "Model.h"

// Window Size
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

// Frame Buffer Size
int FRAME_BUFFER_WIDTH = 1080;
int FRAME_BUFFER_HEIGHT = 720;

#pragma region Callbacks
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);
#pragma endregion

#pragma region Camera Object
GarageEngine::CameraObject cameraObject = GarageEngine::CameraObject{ Vec3 {0.0f, 0.0f, 0.0f}, Versor {0.0f, 0.0f, 0.0f, 0.0f} };
#pragma endregion

#pragma region Mesh Object
GarageEngine::EngineObject meshObject = GarageEngine::EngineObject{ Vec3 {0.0f, 0.0f, -5.0f}, Versor {0.0f, 0.0f, 0.0f, 0.0f} };
#pragma endregion


#pragma region Function declarations
GLFWwindow* create_window(int version_major, int version_minor);
Mat4 set_up_projection_matrix();
#pragma endregion

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
	Shader MeshShader = Shader(
		"mesh.vert", 
		"mesh.frag");

	// Define Model here
	Model model { "..\\testMeshes\\RobotHead.obj" };

	// Define projection matrix
	Mat4 projection = set_up_projection_matrix();

	// TODO: Add transforms and such:

	while (!glfwWindowShouldClose(window)) {
		
		MeshShader.UseShader();

		MeshShader.SetMatrix4("transform_matrix", meshObject.GetTransformationMatrix(), GL_TRUE);
		MeshShader.SetMatrix4("projection", projection, GL_TRUE);
		MeshShader.SetMatrix4("view", cameraObject.GetViewMatrix(), GL_TRUE);

		model.Draw(MeshShader);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwPollEvents();
		glfwSwapBuffers(window);
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

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Global Illumination Project", NULL, NULL);

	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_window_framebuffer_callback);

	return window;
}

void glfw_window_resize_callback(GLFWwindow* window, int width, int height) {
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height) {
	FRAME_BUFFER_WIDTH = width;
	FRAME_BUFFER_HEIGHT = height;
}

Mat4 set_up_projection_matrix() {
	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * DEG_TO_RAD;
	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	Mat4 perspective = projection_matrix(near, far, fov, aspect);
	return perspective;
}