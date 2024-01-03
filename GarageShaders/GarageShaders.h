#pragma once
#pragma warning(disable : 4996)

#ifndef GARAGE_SHADERS
#define GARAGE_SHADERS


#include <stdio.h>
#include <malloc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GarageLog.h"
#include "GarageMathLibrary.h"

using namespace GarageLinearAlgebra;

char* loadShaderString(const char* shaderFileLocation);
bool checkShaderCompilation(GLuint shaderIndex);
void printShaderInfo(GLuint shaderIndex);
bool checkLinking(GLuint programIndex);
void printProgramInfo(GLuint programIndex);
bool is_valid(GLuint programIndex);
const char* GL_type_to_string(GLenum type);

#pragma region Shader

class Shader {
	GLuint shader_program;

	GLuint CreateShader(const char* shader_source, GLenum shader_type);
	GLuint GetShaderUniform(const char* id);

public:
	Shader();
	Shader(const char* vertex_shader, const char* fragment_shader);
	void printAll();
	void UseShader();
	GLuint GetShaderProgramID();

	// Setting Uniform Methods
	void SetMatrix4(const char* id, Mat4 matrix, GLboolean transpose = GL_FALSE, GLsizei count = 1);
	void SetVector4(const char* id, Vec4 vector, GLsizei count = 1);
	void SetVector3(const char* id, Vec3 vector, GLsizei count = 1);
	void SetFloat(const char* id, float value, GLsizei count = 1);
	void SetInt(const char* id, int value, GLsizei count = 1);
};

#pragma endregion Shader

#endif // !GARAGE_SHADERS

