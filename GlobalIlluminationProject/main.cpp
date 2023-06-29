#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <stdarg.h>
#include "../OpenGLCommon/.h/LinearAlgebra.h";
#include "../OpenGLCommon/.h/LinearTransformations.h";
#include "../OpenGLCommon/.h/Log.h";
#include "../OpenGLCommon/.h/ShaderLoading.h"


// Move on with this book!
// https://learnopengl.com/Introduction

// Window Size
int g_win_width = 1280;
int g_win_height = 720;

// Frame Buffer Size
int g_fb_width = 1080;
int g_fb_height = 720;

using namespace LinearAlgebra;

// GLFW Callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);
static void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
static void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// OpenGL helpers
GLFWwindow* create_window(int version_major, int version_minor);
GLuint create_shader_program(std::map<const char *, GLenum> shader_infos);
void run_loop();

#pragma region Camera Class
class Camera {
private:
	Vec3 forward;
	Vec3 up;
	Vec3 right;
	Vec3 position;
	Versor orientation;

	void ApplyDirections() {
		Mat4 orientation_matrix = orientation.to_matrix();
		up = orientation_matrix * Vec4(up, 1.0f);
		forward = orientation_matrix * Vec4(forward, 1.0f);
		right = orientation_matrix * Vec4(right, 1.0f);
	}

public:

	Camera(Vec3 position, Versor orientation) {
		forward = Vec3{ 0.0, 0.0, -1.0f };
		right = Vec3{ 1.0f, 0.0f, 0.0f };
		up = Vec3{ 0.0f, 1.0f, 0.0f };
		this->orientation = orientation;
		this->position = position;
		ApplyDirections();
	}

	Vec3 Position() {
		return position;
	}
	Mat4 OrientationMatrix() {
		orientation.to_matrix();
	}

	Mat4 ApplyRotations(Versor versors...) {
		va_list args;
		va_start(args, versors);
		orientation = orientation * versors;
		va_end(args);
		ApplyDirections();
		return orientation.to_matrix();
	}

	Mat4 ApplyRotations(std::vector<Versor> versors) {
		for (auto versor : versors) {
			orientation = orientation * versor; 
		}
		ApplyDirections();
		return orientation.to_matrix();
	}

	Mat4 GetTranslation(Vec3 translation_changes) {
		Vec3 right_move = right * translation_changes[0];
		Vec3 up_move = up * translation_changes[1];
		Vec3 forward_move = forward * -translation_changes[2];

		position = position + right_move + up_move + forward_move;
		return translate(position);
	}
};
#pragma endregion Camera Class

// Transformations
void set_up_projection_matrix(GLuint shader_program);

int main() {

	if (!restart_gl_log()) {
		return -1;
	}

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3");
		return -1;
	}

	GLFWwindow* window = create_window(4, 1);

	if (!window) {
		fprintf(stderr, "ERROR, could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}

	// Setting callbacks
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_window_framebuffer_callback);
	glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
	glfwSetKeyCallback(window, glfw_keyboard_callback);


	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Render %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Shader creation
	GLuint shader_program;
	try {
		shader_program = create_shader_program(std::map<const char*, GLenum>{ 
			{ "vertexShader.vert", GL_VERTEX_SHADER }, 
			{ "fragmentShader.frag", GL_FRAGMENT_SHADER } });
	}
	catch (std::exception e) {
		printf(e.what());
		return -1;
	}
	set_up_projection_matrix(shader_program);
}



#pragma region GLFW Callbacks
void glfw_error_callback(int error, const char* description) {
	gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_resize_callback(GLFWwindow* window, int width, int height) {
	g_win_width = width;
	g_win_height = height;
}

void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height) {
	g_fb_width = width;
	g_fb_height = height;
}

static void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos) {

}

static void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
}
#pragma endregion GLFW Callbacks

#pragma region OpenGL Helpers
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

GLuint create_shader_program(std::map<const char *, GLenum> shader_infos) {
	std::vector<GLuint> shaders;
	GLuint shader_program;
	for (auto shader_info : shader_infos) {
		char* shader_contents = loadShaderString(shader_info.first);
		GLuint shader_id = glCreateShader(shader_info.second);
		glShaderSource(shader_id, 1, &shader_contents, NULL);
		glCompileShader(shader_id);
		if (!checkShaderCompilation(shader_id)) {
			throw std::runtime_error("Error in shader compilation");
		}
		shaders.push_back(shader_id);
	}

	shader_program = glCreateProgram();
	for (auto shader : shaders) {
		glAttachShader(shader_program, shader);
	}
	glLinkProgram(shader_program);
	if (!checkLinking(shader_program) || !is_valid(shader_program)) {
		throw std::runtime_error("Error in linking shaders");
	}
}


#pragma endregion OpenGL Helpers

#pragma region Transformations
void set_up_projection_matrix(GLuint shader_program) {
	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * DEG_TO_RAD;
	float aspect = (float)g_win_width / (float)g_win_height;
	Mat4 perspective = projection_matrix(near, far, fov, aspect);

	int proj_mat_loc = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, perspective);
}
#pragma endregion Transformations
