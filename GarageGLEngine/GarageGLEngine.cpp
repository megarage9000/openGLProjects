// GarageGLEngine.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GarageMathLibrary.h"
#include "GarageShaders.h"
#include "GarageGLEngine.h"

// TODO: This is an example of a library function
void fnGarageGLEngine()
{
}

GarageGLEngine::GarageGLEngine(const GarageGLEngineWindowProperties& windowProps)
	: windowProperties(windowProps)
{

}
