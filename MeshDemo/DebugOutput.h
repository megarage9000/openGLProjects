#pragma once

#ifndef  DEBUG_OUTPUT
#define DEBUG_OUTPUT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

#endif // ! DEBUG_OUTPUT



