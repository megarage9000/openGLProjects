#pragma once
#ifndef MESH_LOADER
#define MESH_LOADER

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <unordered_map>
#include "GarageShaders.h"
#include "GarageMathLibrary.h"


#define MESH_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\box.obj"
#define TANK_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\tank.obj"

using namespace GarageLinearAlgebra;

/*
	WE NEED VCPKG
	https://www.youtube.com/watch?v=0h1lC3QHLHU
*/


#pragma region Mesh Functions
bool load_mesh(const char* file_name, GLuint* vao, int* point_count);
//bool load_multiple_meshes(const char* file_name, )

#pragma endregion Mesh Functions


// TODO: Create Mesh Visual that can load 1 or more Meshes!
#pragma region MeshVisual

class Renderable {
protected:
	Shader shader;
	std::unordered_map<GLuint, int> vao_data;
	std::unordered_map<const char*, GLuint> vao_maps;
public:
	Renderable() {}
	~Renderable() {}
	virtual void Draw() = 0;
	GLuint GetVaoByName(const char* name);
	void AttachVBO(GLuint vbo, GLuint vao, GLuint index, GLuint size, GLenum type, GLsizei stride, GLboolean normalized = false, const void * pointer = NULL);
};

class CubeRenderer : public virtual Renderable{
	void InitializeMesh();
public:
	CubeRenderer();
	CubeRenderer(Shader shader);
	void Draw();
};

#pragma endregion MeshVisual


#endif // !MESH_LOADER