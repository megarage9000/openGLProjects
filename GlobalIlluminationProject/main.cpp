#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <stdarg.h>
#include "stb_image.h"
#include "EngineObject.h"
#include "../OpenGLCommon/.h/LinearAlgebra.h";
#include "../OpenGLCommon/.h/LinearTransformations.h";
#include "../OpenGLCommon/.h/Log.h";
#include "../OpenGLCommon/.h/ShaderLoading.h"
#include "../OpenGLCommon/.h/MeshLoader.h"

#define DIFFUSE_MAP "..\\textures\\Woodenbox.png"
#define SPECULAR_MAP "..\\textures\\BoxSpecular.png"


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
std::vector<EngineObject> Meshes;
EngineObject MainMesh;
EngineObject LightSource;
Shader MeshShader;
Shader LightShader;
CubeRenderer CubeMesh;
CubeRenderer LightMesh;
Vec4 LightColour{ 1.0f, 1.0f, 1.0f };

float last_mouse_x = g_win_width / 2.0f;
float last_mouse_y = g_win_height / 2.0f;

float elapsed_seconds = 0.0f;
float camera_speed = 10.0f;
float rotation_sensitivity = 1.5f;
float camera_rotation_speed = 15;

float mesh_rotation_speed = 50.0f;
int num_meshes = 10;

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
void process_keyboard(GLFWwindow * window);
void update_colours();

// Transformations
Mat4 set_up_projection_matrix();
#pragma endregion Function Headers

#pragma region Texture Functions
// Texture
// - https://learnopengl.com/Getting-started/Textures
template<typename TextureInitializer>
void load_texture(const char* texture_file, GLuint* diffuse_id, TextureInitializer parameter_setup);
void apply_tex_coord_vbo(GLuint* vbo, float * tex_coords, int num_tex_coords);
#pragma endregion Texture Functions

int main() {
	float last_position = 0.0f;
	float speed = 1.0f;

	Camera = CameraObject(Vec3 {0.0f, 0.0f, 0.0f}, Versor {0.0f, 1.0f, 0.0f, 0.0f});
	for (int i = -num_meshes / 2; i < num_meshes / 2; i++) {
		float x = i;
		float y = i;
		float z = i;
		Meshes.push_back(EngineObject(Vec3{ x, y, z }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f }));
	}
	MainMesh = EngineObject(Vec3{ 0.0f, 2.0f, 0.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });
	LightSource = EngineObject(Vec3{ 1.2f, 1.0f, 2.0f }, Versor{0.0f, 1.0f, 0.0f, 0.0f});

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
	try {
		// - AMBIENT SHADER
		// MeshShader = Shader("vertexShader.vert", "fragmentShader.frag");
		// - DIRECTIONAL LIGHT SHADER
		// MeshShader = Shader("vertexShader.vert", "directionalLightShader.frag");
		// - POINT LIGHT SHADER
		// MeshShader = Shader("vertexShader.vert", "pointLightShader.frag");
		// - SPOT LIGHT SHADER
		MeshShader = Shader("vertexShader.vert", "spotLightShader.frag");
	}
	catch (std::exception e) {
		printf(e.what());
		return -1;
	}

	// Light Shader creation
	try {
		LightShader = Shader("lightVertexShader.vert", "lightFragmentShader.frag");
	}
	catch (std::exception e) {
		printf(e.what());
		return -1;
	}

	Mat4 projection_matrix = set_up_projection_matrix();
	MeshShader.SetMatrix4("projection", projection_matrix, GL_TRUE);
	LightShader.SetMatrix4("projection", projection_matrix, GL_TRUE);

	// Setup Cube Mesh
	CubeMesh = CubeRenderer(MeshShader);

	// Attach Texture to Cube Renderer
	GLuint diffuse_id, specular_id, texture_vbo;
	float tex_coords[] = {
		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,

		0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f,

		0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f
	};
	apply_tex_coord_vbo(&texture_vbo, tex_coords, 36);
	CubeMesh.AttachVBO(texture_vbo, CubeMesh.GetVaoByName("Cube"), 2, 2, GL_FLOAT, 0);
	
	load_texture(DIFFUSE_MAP, &diffuse_id, [] {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	});

	load_texture(SPECULAR_MAP, &specular_id, [] {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

	MeshShader.SetInt("material.diffuse", 0);
	MeshShader.SetInt("material.specular", 1);

	// - Setup attenuation values (Point Light)
	//MeshShader.SetFloat("light.constant", 1.0f);
	//MeshShader.SetFloat("light.linear", 0.09f);
	//MeshShader.SetFloat("light.quadratic", 0.032f);

	// Setup Light Mesh
	LightMesh = CubeRenderer(LightShader);

	while (!glfwWindowShouldClose(window)) {
		update_colours();
		// Clear drawing surface color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_fb_width, g_fb_height);
		
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		if (last_position > 1.0f || last_position < -1.0f) {
			speed = -speed;
		}
		process_keyboard(window);
	
		Mat4 view = Camera.GetViewMatrix();

		// - Drawing Light Source
		LightShader.SetMatrix4("view", view, GL_TRUE);
		LightSource.ApplyScale(Vec3{ 0.5, 0.5, 0.5 });
		LightSource.SetPosition(Vec3{ (float)sin(glfwGetTime()) * 2.0f, (float)cos(glfwGetTime()) * 2.0f, 0.0f });
		LightShader.SetMatrix4("matrix", LightSource.GetTransformationMatrix(), GL_TRUE);
		// - Set light position(Ambient Lighting & Point Light & Spot Light)
		MeshShader.SetVector3("light.position", LightSource.Position());
		// - Set light direction(Directional Lighting)
		// MeshShader.SetVector3("light.direction", Vec3{ -0.2f, -1.0f, -0.3f });

		// - Set Spot Light Data (Spot Lighting)
		MeshShader.SetVector3("light.direction", Camera.GetFront());
		MeshShader.SetFloat("light.cut_off", 12.5f * DEG_TO_RAD);

		// Render Light source
		LightMesh.Draw();
		
		// - Rendering Objects
		// Set camera position
		MeshShader.SetVector3("camera_pos", Camera.GetCameraPos());

		// Set camera view matrix
		MeshShader.SetMatrix4("view", view, GL_TRUE);

		// Setup texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_id);

		Mat4 transform_matrix;

		// Doing random transformations
		float y_rotation = elapsed_seconds * mesh_rotation_speed;
		float x_rotation = elapsed_seconds * mesh_rotation_speed * 0.5f;
		// ALWAYS DO A REFERENCE when iterating a vector of objects
		for (EngineObject &Mesh : Meshes) {
			/*Versor y_versor{ Mesh.GetUp(), y_rotation };
			Versor x_versor{ Mesh.GetRight(), x_rotation };
			Mesh.ApplyRotations(std::vector<Versor> {y_versor, x_versor});
			*/transform_matrix = Mesh.GetTransformationMatrix();
			MeshShader.SetMatrix4("matrix", transform_matrix, GL_TRUE);
			CubeMesh.Draw();
		}
		

		// track events 
		glfwPollEvents();
		glfwSwapBuffers(window);
		last_position = (elapsed_seconds * 0.01 * speed) + last_position;

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

void update_colours() {
	float time = glfwGetTime();
	Vec4 mat_amb_colour{
		sin(time) * 0.9f,
		sin(time) * 0.8f,
		sin(time) * 0.7f,
	};
	Vec4 mat_diff_colour{
		cos(time) * 0.75f,
		cos(time) * 0.5f,
		cos(time) * 0.76f,
	};

	Vec4 light_colour{
		1.0f, 1.0f, 1.0f
	};

	// Setting material properties 
	//MeshShader.SetVector4("material.ambient_colour", mat_amb_colour * 0.5f);
	//MeshShader.SetVector4("material.diffuse_colour", mat_diff_colour * 0.2f);
	MeshShader.SetVector4("material.specular", light_colour);
	MeshShader.SetFloat("material.shininess", 64.0f);

	// Setting colour properties
	MeshShader.SetVector4("light.ambient_colour", Vec3{ 0.2f, 0.2f, 0.2f });
	MeshShader.SetVector4("light.diffuse_colour", Vec3{ 0.5f, 0.5f, 0.5f });
	MeshShader.SetVector4("light.specular_colour", Vec3{ 1.0f, 1.0f, 1.0f });
	LightShader.SetVector4("light_colour", light_colour);
}

#pragma endregion OpenGL Helpers

#pragma region Transformations
Mat4 set_up_projection_matrix() {
	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * DEG_TO_RAD;
	float aspect = (float)g_win_width / (float)g_win_height;
	Mat4 perspective = projection_matrix(near, far, fov, aspect);
	return perspective;
/*	glUseProgram(shader_program);
	int proj_mat_loc = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, perspective);

	glUseProgram(light_program);
	int proj_light_mat_loc = glGetUniformLocation(light_program, "projection");
	glUniformMatrix4fv(proj_light_mat_loc, 1, GL_TRUE, perspective)*/;
}
#pragma endregion Transformations

#pragma region Texture Functions
template<typename TextureInitializer>
void load_texture(const char* texture_file, GLuint* texture_id, TextureInitializer parameter_setup) {

	//Texture setup follows https://learnopengl.com/Getting-started/Textures

	glGenTextures(1, texture_id);


	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_file, &width, &height, &nrChannels, 0);
	if (data) {

		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
			format = GL_RGB;
		glBindTexture(GL_TEXTURE_2D, *texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		parameter_setup();

	}
	else {
		std::string error_str = "Unable to generate texture file " + std::string(texture_file);
		throw std::runtime_error(error_str);
	}
	stbi_image_free(data);
}

void apply_tex_coord_vbo(GLuint* vbo, float* tex_coords, int num_tex_coords) {
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * num_tex_coords * sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
#pragma endregion Texture Functions