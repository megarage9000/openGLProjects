#pragma once

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "../.h/ShaderLoading.h"
#include "../.h/LinearTransformations.h"

#define MESH_FILE "C:\\Users\\gtom_\\source\\repos\\C++Practice\\openGLProjects\\testMeshes\\box.obj"

using namespace LinearAlgebra;

/*
	WE NEED VCPKG
	https://www.youtube.com/watch?v=0h1lC3QHLHU
*/

//class MeshLoader
//{
//};

bool load_mesh(const char* file_name, GLuint* vao, int* point_count);
//bool load_multiple_meshes(const char* file_name, )

#pragma region Shader

class Shader {
	GLuint shader_program;

	GLuint CreateShader(const char* shader_source, GLenum shader_type);
	GLuint GetShaderUniform(const char* id);

public:
	Shader(const char* vertex_shader, const char* fragment_shader);
	void UseShader();

	// Setting Uniform Methods
	void SetMatrix4(const char* id, Mat4 matrix, GLboolean transpose=false, GLsizei count=1);
	void SetVector3(const char* id, Vec3 vector, GLsizei count = 1);

};

#pragma endregion Shader

#pragma region MeshVisual



#pragma endregion MeshVisual