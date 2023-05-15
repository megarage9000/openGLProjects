#pragma once

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MESH_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\box.obj"

/*
	WE NEED VCPKG
	https://www.youtube.com/watch?v=0h1lC3QHLHU
*/

//class MeshLoader
//{
//};

bool load_mesh(const char* file_name, GLuint* vao, int* point_count);
