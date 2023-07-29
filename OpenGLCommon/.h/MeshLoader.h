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
#include "../.h/ShaderLoading.h"
#include "../.h/LinearTransformations.h"


#define MESH_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\box.obj"
#define TANK_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\tank.obj"

using namespace LinearAlgebra;

/*
	WE NEED VCPKG
	https://www.youtube.com/watch?v=0h1lC3QHLHU
*/



#pragma region Mesh Functions
bool load_mesh(const char* file_name, GLuint* vao, int* point_count);
//bool load_multiple_meshes(const char* file_name, )

#pragma endregion Mesh Functions

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
	void SetMatrix4(const char* id, Mat4 matrix, GLboolean transpose=GL_FALSE, GLsizei count=1);
	void SetVector4(const char* id, Vec4 vector, GLsizei count = 1);
	void SetVector3(const char* id, Vec3 vector, GLsizei count = 1);
	void SetFloat(const char* id, float value, GLsizei count = 1);
	void SetInt(const char* id, int value, GLsizei count = 1);

};


#pragma endregion Shader

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