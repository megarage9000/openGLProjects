#pragma once

#include "GarageEngineObject.h"

/*
	The OpenGL version of the Garage Engine, hoping to implement various versions of this engine
	further down the line! (i.e. Vulkan and Metal)
*/

using namespace GarageEngine;

using GarageGLEngineWindowProperties = struct {
	// Window Size
	int windowWidth;
	int windowHeight;

	// Frame Buffer Size;
	int frameBufferWidth;
	int frameBufferHeight;
};

class GarageGLEngine {

public:

	explicit GarageGLEngine(const GarageGLEngineWindowProperties& windowProps);

private:
	GarageGLEngineWindowProperties windowProperties;

};