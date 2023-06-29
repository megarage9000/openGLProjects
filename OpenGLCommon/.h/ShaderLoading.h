#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <malloc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Log.h"

char* loadShaderString(const char* shaderFileLocation);
bool checkShaderCompilation(GLuint shaderIndex);
void printShaderInfo(GLuint shaderIndex);
bool checkLinking(GLuint programIndex);
void printProgramInfo(GLuint programIndex);
bool is_valid(GLuint programIndex);