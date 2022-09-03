
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include "LinearTransformationCPlusPlus.h"
#include <stdio.h>
#include <malloc.h>

// Reported Window Size
int g_win_width = 640;
int g_win_height = 480;

// Frame Buffer Size
int g_fb_width = 640;
int g_fb_height = 480;

// Using transformations library
using namespace LinearTransformationCPlusPlus;
using namespace std;

// FPS tracking
double previous_seconds;
int frame_count;
void _update_fps_counter(GLFWwindow* window);

// Callbacks
void glfw_error_callback(int error, const char* description);
void glfw_window_resize_callback(GLFWwindow* window, int width, int height);
void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height);

// OpenGL parameters
void log_gl_params();

// Shader functions
char* loadShaderString(const char* shaderFileLocation);
bool checkShaderCompilation(GLuint shaderIndex);
void printShaderInfo(GLuint shaderIndex);
bool checkLinking(GLuint programIndex);
void printProgramInfo(GLuint programIndex);
void printAll(GLuint programIndex);
const char* GL_type_to_string(GLenum type);
bool is_valid(GLuint programIndex);

bool moveCamera(GLFWwindow* window, float camera_pos[], float speed, float camRotSpeed,float elapsed_seconds, float * yaw);

int main() {

	// Starting log
	if (!restart_gl_log()) {
		return -1;
	}
	
	gl_log("starting GLFW \n%s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return -1;
	}

	// Hinting for OpenGL 3.2 Forward-Compatible core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable Anti-aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	// Setting monitor dimensions according to primary monitor
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * vmode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Extended OpenGL Init", monitor, NULL);*/
	GLFWwindow* window = glfwCreateWindow(g_win_width, g_win_height, "Extended Init OpenGL", NULL, NULL);

	glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);


	if (!window) {
		fprintf(stderr, "ERROR, could not open window with GLFW3\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	
	log_gl_params();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Render %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST);  
	glDepthFunc(GL_LESS); 


	//--- Triangle drawing ---

	// Shaders: Vertex Shader
	char* vertexShader = loadShaderString("vertexShader.vert");

	// Shaders: Fragment Shader
	char* fragmentShader = loadShaderString("fragmentShader.frag");

	

	// Shaders: Loading and Compilation
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);
	if (!checkShaderCompilation(vs)) {
		return -1;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);
	if (!checkShaderCompilation(fs)) {
		return -1;
	}

	
	
	// Shaders: Creating a Shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);

	// FOR MAC: Enabling attribute locations for shaders
	//glBindAttribLocation(shaderProgram, 0, "vertex_position");
	//glBindAttribLocation(shaderProgram, 1, "vertex_colour");

	glLinkProgram(shaderProgram);
	if (!checkLinking(shaderProgram)) {
		return -1;
	}

	if (!is_valid(shaderProgram)) {
		return -1;
	}
	
	printAll(shaderProgram);

	// Getting our transformation matrix location
	int matrix_location = glGetUniformLocation(shaderProgram, "matrix");


	// Our triangle points, going clockwise with xyz float coordinates
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f, // top coordinates
		0.5f, -0.5f, 0.0f, // bottom right coordinates
		-0.5f, -0.5f, 0.0f // bottom left coordinates
	};

	// Colours!
	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f, // Red
		0.0f, 1.0f, 0.0f, // Green
		0.0f, 0.0f, 1.0f  // Blue
	};

	float matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // First Column
		0.0f, 1.0f, 0.0f, 0.0f, // Second Column
		0.0f, 0.0f, 0.0f, 1.0f, // Third Column
		0.5f, 0.0f, 0.0f, 1.0f  // Fourth Column (We move object to right by .5!)
	};


	// Camera positions
	float camera_pos[] = { 0.0f, 0.0f, 2.0f };
	float cam_yaw = 0.0f;
	

	// Generating a Vertex Buffer Object for our Triangle, to 
	// store our points into memory
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Generating colour points vbo
	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	
	
	// --- (VAO) --- Generating a Vertex Array Object for our Triangle, to prevent having to
	// bind and defining memory per vertex buffer object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 3. Enabling the arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	
	// 1. Adding the points vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// 2. Adding the colors vbo
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Setting up transformation speed
	float speed = 1.0f;
	float last_position = 0.0f;
	std::vector<float> transform_matrix = LinearTransformationCPlusPlus::identity(16);
	
	
	while (!glfwWindowShouldClose(window)) {

		_update_fps_counter(window);

		// Clear drawing surface color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, g_fb_width, g_fb_height);

		// Draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		// Applying matrix transformation
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		if (last_position > 1.0f || last_position < -1.0f) {
			speed = -speed;
		}

		// Camera move
		bool can_move = moveCamera(window, camera_pos, 1, 10, elapsed_seconds, &cam_yaw);
		vector<float> translation = translate(-camera_pos[0], -camera_pos[1], -camera_pos[2]);
		vector<float> rotation = rotate_euler(-cam_yaw, false, true, false);
		vector<float> view = matrix_multiplication(rotation, translation);

		float near = 0.1f;
		float far = 100.0f;
		float fov = 67.0f * DEG_TO_RAD;
		float range = tan(fov * 0.5) * near;
		float aspect = (float)g_win_width / (float)g_win_height;
		vector<float> perspective = projection_matrix(near, far, fov, range, aspect);

		int view_mat_loc = glGetUniformLocation(shaderProgram, "view");
		int proj_mat_loc = glGetUniformLocation(shaderProgram, "projection");

		glUniformMatrix4fv(view_mat_loc, 1, GL_TRUE, view.data());
		glUniformMatrix4fv(proj_mat_loc, 1, GL_TRUE, perspective.data());

		std::vector<float> tranlsation_vector = LinearTransformationCPlusPlus::translate(0.0f, elapsed_seconds * speed, 0.0f);
		transform_matrix = LinearTransformationCPlusPlus::matrix_multiplication(transform_matrix, tranlsation_vector);
		glUniformMatrix4fv(matrix_location, 1, GL_TRUE, transform_matrix.data());
		last_position = elapsed_seconds * speed + last_position;

		// Enable back face culling
		// More info here: https://www.khronos.org/opengl/wiki/Face_Culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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

bool moveCamera(GLFWwindow* window, float camera_pos[], float speed, float camRotSpeed, float elapsed_seconds, float * yaw) {
	bool cam_moved = false;

	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera_pos[0] -= speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera_pos[0] += speed * elapsed_seconds;
		cam_moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP)) {
		camera_pos[1] += speed * elapsed_seconds;
		cam_moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN)) {
		camera_pos[1] -= speed * elapsed_seconds;
		cam_moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera_pos[2] -= speed * elapsed_seconds;
		cam_moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera_pos[2] += speed * elapsed_seconds;
		cam_moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		*yaw -= camRotSpeed * elapsed_seconds;
		cam_moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		*yaw += camRotSpeed * elapsed_seconds;
		cam_moved = true;
	}

	return cam_moved;
}

void _update_fps_counter(GLFWwindow* window)
{
	double current_seconds;
	double elapsed_seconds;

	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - previous_seconds;

	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		char tmp[128];
		double fps = (double)frame_count / elapsed_seconds;
		sprintf(tmp, "openGL @fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

void glfw_error_callback(int error, const char* description) {
	gl_log_err("GLFW ERROR:code %i msg: %s\n", error, description);
}

void glfw_window_resize_callback(GLFWwindow* window, int width, int height)
{
	g_win_width = width;
	g_win_height = height;
}

void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	g_fb_width = width;
	g_fb_height = height;
}

void log_gl_params()
{
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};

	const char* names[]{
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE", 
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};

	gl_log("GL Context Params:\n");

	// Integer parameters
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s: %i\n", names[i], v);
	}

	// Other parameters
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	gl_log("%s: %i, %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s: %u\n", names[11], (unsigned int)s);
	gl_log("------------------------\n");
}

char * loadShaderString(const char * shaderFileLocation){
	
	// From https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer, cool ey?
	FILE* f = fopen(shaderFileLocation, "rb");

	if (f) {
		// Seek to the end of the file, and use its position to determine the length of the buffer
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);

		// Reset to the beginning
		fseek(f, 0, SEEK_SET);

		char* content = (char*)malloc(fsize + 1);
		if (content) {
			fread(content, 1, fsize, f);
		}
		fclose(f);
		content[fsize] = 0;
		return content;
	}
	return nullptr;
}
// Try using https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/ for this!

bool checkShaderCompilation(GLuint shaderIndex)
{
	int params = -1;
	glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		gl_log_err("ERROR: shader index %i didn't compile correctly!\n", shaderIndex);
		printShaderInfo(shaderIndex);
		return false;
	}
	return true;
}

void printShaderInfo(GLuint shaderIndex)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];

	glGetShaderInfoLog(shaderIndex, max_length, &actual_length, log);
	printf("Shader info for index %i: \n %s\n", shaderIndex, log);
	gl_log("Shader info for index %i: \n %s\n", shaderIndex, log);
}

bool checkLinking(GLuint programIndex)
{
	int params = -1;
	glGetProgramiv(programIndex, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		gl_log_err("ERROR: linking program for index %i did not work!\n", programIndex);
		printProgramInfo(programIndex);
		return false;
	}
	return true;
}

void printProgramInfo(GLuint programIndex)
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];

	glGetProgramInfoLog(programIndex, max_length, &actual_length, log);
	printf("program info log for index %i:\n%s\n", programIndex, log);
	gl_log("program info log for index %i:\n%s\n", programIndex, log);

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

bool is_valid(GLuint programIndex)
{
	glValidateProgram(programIndex);
	int params = -1;
	glGetProgramiv(programIndex, GL_VALIDATE_STATUS, &params);
	printf("program %i GL_VALIDATE_STATUS = %i\n", programIndex, params);
	if (GL_TRUE != params) {
		printProgramInfo(programIndex);
		return false;
	}
	return true;
}



