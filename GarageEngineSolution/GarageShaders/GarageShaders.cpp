#include "GarageShaders.h"

char* loadShaderString(const char* shaderFileLocation) {

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

const char* GL_type_to_string(GLenum type) {
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

#pragma region Shader Methods

GLuint Shader::CreateShader(const char* shader_source, GLenum shader_type) {
	char* shader_contents = loadShaderString(shader_source);
	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_contents, NULL);
	glCompileShader(shader_id);
	if (!checkShaderCompilation(shader_id)) {
		throw std::runtime_error("Error in shader compilation");
	}
	return shader_id;
}

GLuint Shader::GetShaderUniform(const GLchar* id) {
	UseShader();
	GLuint uniform_id = glGetUniformLocation(shader_program, id);
	std::string error;
	switch (uniform_id) {
	case GL_INVALID_VALUE:
		error = "Invalid uniform value for id " + std::string(id);
		throw std::runtime_error(error);
	case GL_INVALID_OPERATION:
		error = "Unable to find uniform value for id " + std::string(id);
		throw std::runtime_error(error);
	default:
		return uniform_id;
	}
}

Shader::Shader() {
	shader_program = -1;
}

void Shader::printAll()
{
	printf("-------------\n shader programme %i info: \n", shader_program);
	int params = -1;

	glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
	printf("GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(shader_program, GL_ATTACHED_SHADERS, &params);
	printf("GL_ATTACHED_SHADERS = %i\n", params);

	// Print active uniforms present in program
	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &params);
	printf("GL_ACTIVE_UNIFORMS = %i\n", params);

	// Print attribute information
	glGetProgramiv(shader_program, GL_ACTIVE_ATTRIBUTES, &params);
	printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);

	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int maxLength = 64;
		int actualLength = 0;
		int size = 0;
		GLenum type;

		glGetActiveAttrib(shader_program, i, maxLength, &actualLength, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char longName[64];
				sprintf(longName, "%s[%i]", name, j); // sprintf copies strings unto a buffer
				int location = glGetAttribLocation(shader_program, longName);
				printf("%i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), longName, location);
			}
		}
	}
}

Shader::Shader(const char* vertex_shader, const char* fragment_shader) {

	GLuint vertex_shader_id = CreateShader(vertex_shader, GL_VERTEX_SHADER);
	GLuint fragment_shader_id = CreateShader(fragment_shader, GL_FRAGMENT_SHADER);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader_id);
	glAttachShader(shader_program, fragment_shader_id);

	glLinkProgram(shader_program);
	if (!checkLinking(shader_program) || !is_valid(shader_program)) {
		throw std::runtime_error("Error in linking shaders");
	}

	printAll();
	printProgramInfo(shader_program);
}

void Shader::UseShader() {
	glUseProgram(shader_program);
}

GLuint Shader::GetShaderProgramID() {
	return shader_program;
}

void Shader::SetMatrix4(const char* id, Mat4 matrix, GLboolean transpose, GLsizei count) {
	GLuint uniform = GetShaderUniform(id);
	glUniformMatrix4fv(uniform, count, transpose, matrix);
}

void Shader::SetVector4(const char* id, Vec4 vector, GLsizei count) {
	GLuint uniform = GetShaderUniform(id);
	glUniform4fv(uniform, count, vector);
}

void Shader::SetVector3(const char* id, Vec3 vector, GLsizei count) {
	GLuint uniform = GetShaderUniform(id);
	glUniform3fv(uniform, count, vector);
}

void Shader::SetFloat(const char* id, float value, GLsizei count) {
	GLuint uniform = GetShaderUniform(id);
	glUniform1f(uniform, value);
}

void Shader::SetInt(const char* id, int value, GLsizei count) {
	GLuint uniform = GetShaderUniform(id);
	glUniform1i(uniform, value);
}

void Shader::SetIntArray(const char* id, const int* values, GLsizei count) {
	const GLuint uniform = GetShaderUniform(id);
	glUniform1iv(uniform, count, values);
}

void Shader::SetUIntArray(const char* id, const unsigned int* values, GLsizei count){
	const GLuint uniform = GetShaderUniform(id);
	glUniform1uiv(uniform, count, values);
}

#pragma endregion Shader Methods