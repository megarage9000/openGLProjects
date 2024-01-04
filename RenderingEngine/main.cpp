#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <stdarg.h>
#include "EngineObject.h"
#include "GarageMathLibrary.h"
#include "GarageShaders.h"
#include "GarageLog.h"
#include "../OpenGLCommon/.h/stb_image.h"
#include "../OpenGLCommon/.h/MeshLoader.h"

#define DIFFUSE_MAP "..\\textures\\Woodenbox.png"
#define SPECULAR_MAP "..\\textures\\BoxSpecular.png"
#define SPOT_LIGHT_TEXTURE "..\\textures\\mario_face.jpg"


using namespace GarageLinearAlgebra;

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
int num_meshes = 6;

// Light variables
int num_dir_lights = 1;
int num_point_lights = 4;
int num_spot_lights = 2;

std::vector<Vec3> dir_lights_dirs;
std::vector<EngineObject> point_lights;
std::vector<EngineObject> spot_lights;


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
	for (int x = -num_meshes / 2; x < num_meshes / 2; x++) {
		for (int y = -num_meshes / 2; y < num_meshes / 2; y++) {
			for (int z = -num_meshes / 2; z < num_meshes / 2; z++) {
				Vec3 position{ (float)x, (float)y, (float)z };
				Meshes.push_back(EngineObject(position, Versor{ 0.0f, 1.0f, 0.0f, 0.0f }));
			}
		}
	}
	MainMesh = EngineObject(Vec3{ 0.0f, 2.0f, 0.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });

	// Setup Directional Light
	for (int i = 0; i < num_dir_lights; i++) {
		Vec3 dir = Vec3(-0.2f, -1.0f, -0.3f);
		dir_lights_dirs.push_back(dir);
	}

	// Setup Point Light transnforms
	for (int i = 0; i < num_point_lights; i++) {
		EngineObject point_light = EngineObject(Vec3{ 1.2f, 1.0f, 2.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });
		point_lights.push_back(point_light);
	}

	// Setup Spot Light transforms
	for (int i = 1; i < num_spot_lights; i++) {
		EngineObject spot_light = EngineObject(Vec3{ 0.0f, 0.0f, 0.0f }, Versor{ 0.0f, 1.0f, 0.0f, 0.0f });
		spot_lights.push_back(spot_light);
	}

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
		MeshShader = Shader("..\\OpenGLCommon\\Shaders\\vertexShader.vert", "..\\OpenGLCommon\\Shaders\\multiLightShader.frag");

	}
	catch (std::exception e) {
		printf(e.what());
		return -1;
	}

	// Light Shader creation
	try {
		LightShader = Shader("..\\OpenGLCommon\\Shaders\\lightVertexShader.vert", "..\\OpenGLCommon\\Shaders\\lightFragmentShader.frag");
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
	GLuint diffuse_id, specular_id, spot_light_texture_id, texture_vbo;
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

	load_texture(SPOT_LIGHT_TEXTURE, &spot_light_texture_id, [] {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

	MeshShader.SetInt("material.diffuse", 0);
	MeshShader.SetInt("material.specular", 1);
	MeshShader.SetInt("spot_light_texture", 2);


	// Setup number of lights per type
	MeshShader.SetInt("num_dir_lights", num_dir_lights);
	MeshShader.SetInt("num_point_lights", num_point_lights);
	MeshShader.SetInt("num_spot_lights", num_spot_lights);

	// - Setup values (Directional Light)
	for (int i = 0; i < num_dir_lights; i++) {
		std::string dir_light_str = "dir_lights[" + std::to_string(i) + "]";
		MeshShader.SetVector4((dir_light_str + ".ambient_colour").c_str(), Vec3{ 0.1f, 0.1f, 0.1f });
		MeshShader.SetVector4((dir_light_str + ".diffuse_colour").c_str(), Vec3{ 0.1f, 0.1f, 0.1f });
		MeshShader.SetVector4((dir_light_str + ".specular_colour").c_str(), Vec3{ 1.0f, 1.0f, 1.0f });

	}

	// - Setup values (Point Light)
	for (int i = 0; i < num_point_lights; i++) {
		std::string point_light_str = "point_lights[" + std::to_string(i) + "]";
		MeshShader.SetFloat((point_light_str +".constant").c_str(), 1.0f);
		MeshShader.SetFloat((point_light_str +".linear").c_str(), 0.09f);
		MeshShader.SetFloat((point_light_str +".quadratic").c_str(), 0.032f);

		MeshShader.SetVector4((point_light_str + ".diffuse_colour").c_str(), Vec3{ 0.2f, 0.2f, 0.2f });
		MeshShader.SetVector4((point_light_str + ".ambient_colour").c_str(), Vec3{ 0.2f, 0.2f, 0.2f });
		MeshShader.SetVector4((point_light_str + ".specular_colour").c_str(), Vec3{ 1.0f, 1.0f, 1.0f });
	}

	// - Setup values (Spot Light)
	for (int i = 0; i < num_spot_lights; i++) {
		std::string spot_light_str = "spot_lights[" + std::to_string(i) + "]";
		MeshShader.SetFloat((spot_light_str + ".constant").c_str(), 0.5f);
		MeshShader.SetFloat((spot_light_str + ".linear").c_str(), 0.09f);
		MeshShader.SetFloat((spot_light_str + ".quadratic").c_str(), 0.032f);

		MeshShader.SetVector4((spot_light_str + ".ambient_colour").c_str(), Vec3{ 0.5f, 0.5f, 1.0f });
		MeshShader.SetVector4((spot_light_str + ".diffuse_colour").c_str(), Vec3{ 0.5f, 0.5f, 1.0f });
		MeshShader.SetVector4((spot_light_str + ".specular_colour").c_str(), Vec3{ 1.0f, 1.0f, 1.0f });
	}

	// Setup Light Mesh
	LightMesh = CubeRenderer(LightShader);

	while (!glfwWindowShouldClose(window)) {

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

		MeshShader.SetFloat("material.shininess", 32.0f);

		// Setting colour properties
		LightShader.SetVector4("light_colour", Vec4{1.0f, 1.0f, 1.0f, 1.0f});
		
		// Set camera view matrix
		MeshShader.SetMatrix4("view", view, GL_TRUE);
		LightShader.SetMatrix4("view", view, GL_TRUE);
		
		// Set camera position
		MeshShader.SetVector3("camera_pos", Camera.GetCameraPos());

		// - Set Spot Light Data (Spot Lighting) (FOR CAMERA)
		MeshShader.SetVector3("spot_lights[0].position", Camera.GetCameraPos());
		MeshShader.SetVector3("spot_lights[0].direction", Camera.GetFront());
		MeshShader.SetFloat("spot_lights[0].cut_off", cos(20.5f * DEG_TO_RAD));
		MeshShader.SetFloat("spot_lights[0].outer_cut_off", cos(26.5f * DEG_TO_RAD));

		// - Set Spot Light Data (Spot Lighting) (NON-CAMMERA)
		for (int i = 1; i < num_spot_lights; i++) {
			int radius = 4.0f * (i + 1);
			float time = glfwGetTime();
			float y_pos = cos(time) * radius;
			float z_pos = sin(time) * radius;
			spot_lights[i - 1].SetPosition(Vec3{ spot_lights[i - 1].Position()[0], y_pos, z_pos });

			Vec3 forward_vec = spot_lights[i - 1].GetForward();
			Vec3 right_vec = spot_lights[i - 1].GetRight();
			Vec3 up_vec = spot_lights[i - 1].GetUp();

			Mat4 look_matrix = view_matrix(
				Vec4{0.0f, 0.0f, 0.0f},
				Vec4(spot_lights[i - 1].Position()),
				up_vec,
				forward_vec,
				right_vec
			);


			LightShader.SetMatrix4("matrix", look_matrix, GL_TRUE);
			LightMesh.Draw();

			spot_lights[i - 1].SetForward(forward_vec);
			spot_lights[i - 1].SetRight(right_vec);
			spot_lights[i - 1].SetUp(up_vec);

			std::string spot_light_str = "spot_lights[" + std::to_string(i) + "]";
			MeshShader.SetVector3((spot_light_str + ".position").c_str(), spot_lights[i - 1].Position());
			MeshShader.SetVector3((spot_light_str + ".direction").c_str(), spot_lights[i - 1].GetForward());
			MeshShader.SetFloat((spot_light_str + ".cut_off").c_str(), cos(20.5f * DEG_TO_RAD));
			MeshShader.SetFloat((spot_light_str + ".outer_cut_off").c_str(), cos(26.5f * DEG_TO_RAD));
		}

		// - Set Point Light Data (Point Lighting)
		for (int i = 0; i < num_point_lights; i++) {
			// Have point lights "orbit around center"
			// On the x, z plane
			int radius = 1.5f * i;
			float time = glfwGetTime();
			float x_pos = cos(time) * radius;
			float z_pos = sin(time) * radius;
			point_lights[i].SetPosition(Vec3{x_pos, point_lights[i].Position()[1], z_pos});
			point_lights[i].ApplyScale(Vec3{ 0.25, 0.25, 0.25 });

			LightShader.SetMatrix4("matrix",point_lights[i].GetTransformationMatrix(), GL_TRUE);
			LightMesh.Draw();

			Vec4 position = point_lights[i].Position();
			std::string point_light_str = "point_lights[" + std::to_string(i) + "]";
			MeshShader.SetVector4((point_light_str + ".position").c_str(), position);
		}
		
		// - Set Directional Light Data (Directional Lighting)
		for (int i = 0; i < num_dir_lights; i++) {
			std::string directional_light_str = "dir_lights[" + std::to_string(i) + "]";
			MeshShader.SetVector3((directional_light_str + ".direction").c_str(), dir_lights_dirs[i]);
		}
		// - Rendering Objects

		// Setup texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_id);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, spot_light_texture_id);

		Mat4 transform_matrix;

		// Doing random transformations
		float y_rotation = elapsed_seconds * mesh_rotation_speed;
		float x_rotation = elapsed_seconds * mesh_rotation_speed * 0.5f;
		// ALWAYS DO A REFERENCE when iterating a vector of objects
		for (EngineObject& Mesh : Meshes) {
			Versor y_versor{ Mesh.GetUp(), y_rotation };
			Versor x_versor{ Mesh.GetRight(), x_rotation };
			Mesh.ApplyRotations(std::vector<Versor> {y_versor, x_versor});
			Mesh.ApplyScale(Vec3{ 0.5, 0.5, 0.5 });
			transform_matrix = Mesh.GetTransformationMatrix();
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
	float x_offset = (x_pos - last_mouse_x) * (rotation_sensitivity / 100.0f) * -1.0f;
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
Mat4 set_up_projection_matrix() {
	float near = 0.1f;
	float far = 100.0f;
	float fov = 67.0f * DEG_TO_RAD;
	float aspect = (float)g_win_width / (float)g_win_height;
	Mat4 perspective = projection_matrix(near, far, fov, aspect);
	return perspective;
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