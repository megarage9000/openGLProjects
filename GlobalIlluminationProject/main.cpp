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
#include "../OpenGLCommon/.h/MeshLoader.h"

using namespace LinearAlgebra;

#pragma region BasicObject Class and CameraObject Class
class BasicObject {
private:
	Vec3 forward;
	Vec3 up;
	Vec3 right;
	Vec3 position;
	Versor orientation;


	void ApplyDirections() {
		Mat4 orientation_matrix = orientation.to_matrix();
		up = orientation_matrix * Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		forward = orientation_matrix * Vec4(0.0f, 0.0f, 1.0f, 0.0f);
		right = orientation_matrix * Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}

public:
	BasicObject() {
		forward = Vec3{ 0.0, 0.0, 1.0f };
		right = Vec3{ 1.0f, 0.0f, 0.0f };
		up = Vec3{ 0.0f, 1.0f, 0.0f };
		position = Vec3{ 0.0f, 0.0f, 0.0f };
		orientation = Versor{ 0.0f, 1.0f, 0.0f, 0.0f };
	}

	BasicObject(Vec3 position, Versor orientation) {
		forward = Vec3{ 0.0, 0.0, 1.0f };
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
		return orientation.to_matrix();
	}

	Mat4 TranslationMatrix() {
		return translate(position);
	}

	Mat4 ApplyRotations(std::vector<Versor> versors) {
		for (auto versor : versors) {
			orientation = versor * orientation; 
		}
		ApplyDirections();
		return orientation.to_matrix();
	}

	Mat4 ApplyTranslation(Vec3 translation_changes) {
		Vec3 right_move = right * translation_changes[0];
		Vec3 up_move = up * translation_changes[1];
		Vec3 forward_move = forward * -translation_changes[2];

		position = position + right_move + up_move + forward_move;
		return translate(position);
	}

	Vec3 GetForward() {
		return forward;
	}

	Vec3 GetUp() {
		return up;
	}

	Vec3 GetRight() {
		return right;
	}
};

// Based off of LearnOpenGL's camera implementation
class CameraObject {
	Vec3 direction;
	Vec3 position;
	Vec3 camera_front;
	Vec3 camera_up;
	Vec3 camera_right;
	Vec3 world_up;

	
	void GetNewDirections() {
		camera_right = (camera_front.cross(world_up)).normalize();
		camera_up = (camera_front.cross(camera_right)).normalize();
	/*	std::cout << "------RIGHT, UP, FRONT------\n";
		camera_right.print();
		camera_up.print();
		camera_front.print();
	*/
	}

public:
	// pitch and yaw
	float pitch;
	float yaw;

	CameraObject() {
		position = Vec3(0.0f, 0.0f, 5.0f);
		world_up = Vec3(0.0f, 1.0f, 0.0f);
		pitch = 0.0f;
		yaw = -90.0f;
		RealignGaze();
	}
	CameraObject(Vec3 position, Vec3 target, Vec3 world_up = Vec3{0.0f, 1.0f, 0.0f}) {
		this->position = position;
		this->world_up = world_up;
		pitch = 0.0f;
		yaw = -90.0f;
		RealignGaze();
	}

	void RealignGaze() {
		float yaw_rad = yaw * DEG_TO_RAD;
		float pitch_rad = pitch * DEG_TO_RAD;
		direction[0] = cos(yaw_rad) * cos(pitch_rad);
		direction[1] = sin(pitch_rad);
		direction[2] = sin(yaw_rad) * cos(pitch_rad);
		camera_front = direction.normalize();
		GetNewDirections();
	}

	void ApplyTranslation(Vec3 translation_changes) {
		position = position +
			camera_right * translation_changes[0] +
			camera_up * translation_changes[1] +
			camera_front * translation_changes[2];
	}

	Mat4 GetLookAt() {
		return view_matrix(camera_up, Vec4(position + camera_front), Vec4(position));
	}
};
#pragma endregion BasicObject Class

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
BasicObject Mesh;

float last_mouse_x = g_win_width / 2.0f;
float last_mouse_y = g_win_height / 2.0f;

float elapsed_seconds = 0.0f;
float camera_speed = 10.0f;
float rotation_sensitivity = 1.5f;

float pitch = 0.0f;
float yaw = -90.0f;

#pragma region Function Headers
// GLFW Callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_window_framebuffer_callback(GLFWwindow* window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// OpenGL helpers
GLFWwindow* create_window(int version_major, int version_minor);
GLuint create_shader_program(std::map<const char *, GLenum> shader_infos);
void printAll(GLuint programIndex);
const char* GL_type_to_string(GLenum type);
void process_keyboard(GLFWwindow * window);

// Transformations
void set_up_projection_matrix(GLuint shader_program);
#pragma endregion Function Headers



int main() {
	float last_position = 0.0f;
	float speed = 5.0f;

	Camera = CameraObject();
	Mesh = BasicObject(Vec3{ 0.0, 0.0, 0.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });

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

	printAll(shader_program);
	glUseProgram(shader_program);
	set_up_projection_matrix(shader_program);
	int matrix_location = glGetUniformLocation(shader_program, "matrix");

	// Mesh Loading
	GLuint vao;
	int point_count;
	assert(load_mesh(MESH_FILE, &vao, &point_count));
	Mat4 transform_matrix = Mat4(IDENTITY_4, 16);

	Mat4 view = Camera.GetLookAt().inverse();
	int view_mat_loc = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(view_mat_loc, 1, GL_TRUE, view);

	while (!glfwWindowShouldClose(window)) {

		// Clear drawing surface color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, g_fb_width, g_fb_height);

		// Draw triangle
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

		Mat4 view = Camera.GetLookAt().inverse();
		int view_mat_loc = glGetUniformLocation(shader_program, "view");
		glUniformMatrix4fv(view_mat_loc, 1, GL_TRUE, view);

		Mat4 translation_shape = Mesh.ApplyTranslation(Vec3{ 0.0f, 0.0f, 0.0f });
		transform_matrix = transform_matrix * translation_shape;
		glUniformMatrix4fv(matrix_location, 1, GL_TRUE, transform_matrix);
		last_position = elapsed_seconds * speed + last_position;

		// Enable back face culling
		// More info here: https://www.khronos.org/opengl/wiki/Face_Culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		glDrawArrays(GL_TRIANGLES, 0, point_count);

		process_keyboard(window);

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
void glfw_cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos) {
	float x_offset = (x_pos - last_mouse_x) * (rotation_sensitivity / 100.0f);
	float y_offset = (last_mouse_y - y_pos) * (rotation_sensitivity / 100.0f);

	last_mouse_x = x_pos;
	last_mouse_y = y_pos;

	Camera.yaw += x_offset;
	Camera.pitch += y_offset;

	if (Camera.pitch > 90.0f) {
		Camera.pitch = 90.0f;
	}
	if (Camera.pitch < -90.0f) {
		Camera.pitch = -90.0f;
	}
	Camera.RealignGaze();
	//Vec3 up_vector = Camera.GetUp();
	//Vec3 right_vector = Camera.GetRight();

	//Versor horizontal_rotation{ up_vector, x_offset };
	//Versor vertical_rotation{ right_vector, y_offset };

	//Camera.ApplyRotations(std::vector<Versor> {horizontal_rotation, vertical_rotation});
}

bool key_hold = false;
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

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

void process_keyboard(GLFWwindow * window) {

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
	Camera.ApplyTranslation(translation_change.normalize() * camera_speed * elapsed_seconds);
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
