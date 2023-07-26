#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <stdarg.h>
#include "EngineObject.h"
#include "../OpenGLCommon/.h/LinearAlgebra.h";
#include "../OpenGLCommon/.h/LinearTransformations.h";
#include "../OpenGLCommon/.h/Log.h";
#include "../OpenGLCommon/.h/ShaderLoading.h"
#include "../OpenGLCommon/.h/MeshLoader.h"

using namespace LinearAlgebra;

// Move on with this book!
// https://learnopengl.com/Introduction

// Window Size
int g_win_width = 1280;
int g_win_height = 720;

// Frame Buffer Size
int g_fb_width = 1080;
int g_fb_height = 720;

// Global Variables
CameraObject Camera;
EngineObject Mesh;
EngineObject LightSource;
Vec4 LightColour{ 1.0f, 1.0f, 1.0f };

float last_mouse_x = g_win_width / 2.0f;
float last_mouse_y = g_win_height / 2.0f;

float elapsed_seconds = 0.0f;
float camera_speed = 10.0f;
float rotation_sensitivity = 1.5f;
float camera_rotation_speed = 15;

#pragma region Function Headers
// GLFW Callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfw_mousebutton_callback(GLFWwindow* window, int button, int action, int mods);

// OpenGL helpers
GLFWwindow* create_window(int version_major, int version_minor);
GLuint create_shader_program(std::map<const char *, GLenum> shader_infos);
void printAll(GLuint programIndex);
const char* GL_type_to_string(GLenum type);
void process_keyboard(GLFWwindow * window);

// Transformations
void set_up_projection_matrix(GLuint shader_program, GLuint light_program);
#pragma endregion Function Headers

int main() {
	float last_position = 0.0f;
	float speed = 5.0f;

	Camera = CameraObject(Vec3 {0.0f, 0.0f, 0.0f}, Versor {0.0f, 1.0f, 0.0f, 0.0f});
	// Camera = CameraObject();
	Mesh = EngineObject(Vec3{ 0.0f, 0.0f, -5.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });
	LightSource = EngineObject(Vec3{ 0.0f, 10.0f, -5.0f }, Versor{0.0f, 1.0f, 0.0f, 0.0f});

	if (!restart_gl_log()) {
		return -1;
	}

	glfwSetErrorCallback(glfw_error_callback);

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

	// Setting callbacks
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_window_framebuffer_callback);
	glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
	glfwSetMouseButtonCallback(window, glfw_mousebutton_callback);
	glfwSetKeyCallback(window, glfw_keyboard_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Light Shader creation
	GLuint light_shader_program;
	try {
		light_shader_program = create_shader_program(std::map<const char*, GLenum> {
			{"lightVertexShader.vert", GL_VERTEX_SHADER},
			{ "lightFragmentShader.frag", GL_FRAGMENT_SHADER }
		});
	}
	catch (std::exception e) {
		printf(e.what());
		return -1;
	}

	printAll(shader_program);
	printAll(light_shader_program);
	set_up_projection_matrix(shader_program, light_shader_program);
	int matrix_location = glGetUniformLocation(shader_program, "matrix");

	// Mesh Loading
	glUseProgram(shader_program);
	GLuint vao;
	int point_count;
	assert(load_mesh(MESH_FILE, &vao, &point_count));
	Mat4 transform_matrix = Mat4(IDENTITY_4, 16);

	Mat4 view = Camera.GetViewMatrix().inverse();
	int view_mat_loc = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(view_mat_loc, 1, GL_TRUE, view);

	// Light loading (use same mesh)
	glUseProgram(light_shader_program);
	GLuint vao_light;
	int light_point_count;
	assert(load_mesh(MESH_FILE, &vao_light, &light_point_count));




	while (!glfwWindowShouldClose(window)) {

		// Clear drawing surface color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, g_fb_width, g_fb_height);

		// Draw Object
		glUseProgram(shader_program);
		glBindVertexArray(vao);

		// Informing the program to use a specific texture slot
		// glUniform1i(tex_loc, 0);
		// glUniform3fv(light_location, 3, camera_pos);

		// Applying matrix transformation
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		if (last_position > 1.0f || last_position < -1.0f) {
			speed = -speed;
		}
		process_keyboard(window);

		Mat4 view = Camera.GetViewMatrix();
		int view_mat_loc = glGetUniformLocation(shader_program, "view");
		glUniformMatrix4fv(view_mat_loc, 1, GL_TRUE, view);

		// Get camera position
		int camera_pos_loc = glGetUniformLocation(shader_program, "camera_pos");
		glUniform3fv(camera_pos_loc, 1, Camera.GetCameraPos());

		// Get light position
		int light_position = glGetUniformLocation(shader_program, "light_position");
		glUniform3fv(light_position, 1, LightSource.Position());

		Mat4 translation_shape = Mesh.ApplyTranslation(Vec3{ 0.0f, 0.0f, 0.0f });
		transform_matrix = transform_matrix * translation_shape;
		glUniformMatrix4fv(matrix_location, 1, GL_TRUE, translation_shape);
		last_position = elapsed_seconds * speed + last_position;


		// Enable back face culling
		// More info here: https://www.khronos.org/opengl/wiki/Face_Culling
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//glFrontFace(GL_CCW);
		glDrawArrays(GL_TRIANGLES, 0, point_count);

		// Render light source
		glUseProgram(light_shader_program);
		glBindVertexArray(vao_light);

		int light_view_mat_loc = glGetUniformLocation(light_shader_program, "view");
		glUniformMatrix4fv(light_view_mat_loc, 1, GL_TRUE, view);

		int light_colour_loc = glGetUniformLocation(light_shader_program, "light_colour");
		glUniform4fv(light_colour_loc, 1, LightColour);
		

		Mat4 light_translation_shape = LightSource.ApplyTranslation(Vec3{ 0.0f, 0.0f, 0.0f });
		// Find out why lamp not ligthing up: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/2.2.basic_lighting_specular/basic_lighting_specular.cpp
		int light_matrix_location = glGetUniformLocation(light_shader_program, "matrix");
		glUniformMatrix4fv(light_matrix_location, 1, GL_TRUE, light_translation_shape);

		glDrawArrays(GL_TRIANGLES, 0, light_point_count);
		
		// track events 
		glfwPollEvents();
		glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
	}

	glfwTerminate();
	return 0;
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

bool can_rotate = false;
void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos) {
	if (!can_rotate) return;
	float x_offset = (x_pos - last_mouse_x) * (rotation_sensitivity / 100.0f);
	float y_offset = (last_mouse_y - y_pos) * (rotation_sensitivity / 100.0f);

	last_mouse_x = x_pos;
	last_mouse_y = y_pos;

	Camera.RealignGaze(x_offset, y_offset);
}

bool cursor_lock = true;
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_0) {
			cursor_lock != cursor_lock;
			if (cursor_lock) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}
}

void glfw_mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		can_rotate = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		can_rotate = false;
	}
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

	return shader_program;
}

bool lock_cursor = true;
void process_keyboard(GLFWwindow * window) {

	float pitch = 0.0f;
	float yaw = 0.0f;

	Vec3 translation_change{ 0.0f, 0.0f, 0.0f };
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		translation_change[2] += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		translation_change[0] += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		translation_change[2] -= 1;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		translation_change[0] -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		translation_change[1] += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		translation_change[1] -= 1;
	}

	// Rotation with arrow keys
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pitch += rotation_sensitivity * camera_rotation_speed * elapsed_seconds;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pitch -= rotation_sensitivity * camera_rotation_speed * elapsed_seconds;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		yaw -= rotation_sensitivity * camera_rotation_speed * elapsed_seconds;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		yaw += rotation_sensitivity * camera_rotation_speed * elapsed_seconds;
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		lock_cursor = !lock_cursor;
		if (lock_cursor) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	Camera.ApplyTranslation(translation_change * camera_speed * elapsed_seconds);
	Camera.RealignGaze(yaw, pitch);
}

#pragma endregion OpenGL Helpers

#pragma region Transformations
void set_up_projection_matrix(GLuint shader_program, GLuint light_program) {
	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * DEG_TO_RAD;
	float aspect = (float)g_win_width / (float)g_win_height;
	Mat4 perspective = projection_matrix(near, far, fov, aspect);

	glUseProgram(shader_program);
	int proj_mat_loc = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, perspective);

	glUseProgram(light_program);
	int proj_light_mat_loc = glGetUniformLocation(light_program, "projection");
	glUniformMatrix4fv(proj_light_mat_loc, 1, GL_TRUE, perspective);
}

void printAll(GLuint programIndex)
{
	printf("-------------\n shader programme %i info: \n", programIndex);
	int params = -1;

	glGetProgramiv(programIndex, GL_LINK_STATUS, &params);
	printf("GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(programIndex, GL_ATTACHED_SHADERS, &params);
	printf("GL_ATTACHED_SHADERS = %i\n", params);


	// Print attribute information
	glGetProgramiv(programIndex, GL_ACTIVE_ATTRIBUTES, &params);
	printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);

	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int maxLength = 64;
		int actualLength = 0;
		int size = 0;
		GLenum type;

		glGetActiveAttrib(programIndex, i, maxLength, &actualLength, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char longName[64];
				sprintf(longName, "%s[%i]", name, j); // sprintf copies strings unto a buffer
				int location = glGetAttribLocation(programIndex, longName);
				printf("%i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), longName, location);
			}
		}
	}

	// Printing all active unifroms
	glGetProgramiv(programIndex, GL_ACTIVE_UNIFORMS, &params);
	printf("GL_ACTIVE_UNIFORMS = %i\n", params);
	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int maxLength = 64;
		int actualLength = 0;
		int size = 0;
		GLenum type;

		glGetActiveUniform(programIndex, i, maxLength, &actualLength, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char longName[64];
				sprintf(longName, "%s[%i]", name, j);
				int location = glGetUniformLocation(programIndex, longName);
				printf(" %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
			}
		}
	}

	printProgramInfo(programIndex);
}

// We can always add other types here 
const char* GL_type_to_string(GLenum type)
{
	switch (type) {

	case GL_BOOL: return "bool";
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_3D: return "sampler3D";
	case GL_SAMPLER_CUBE: return "samplerCube";
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
	default: return "other";

	}
}
#pragma endregion Transformations
