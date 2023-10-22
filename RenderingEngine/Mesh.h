#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../OpenGLCommon/.h/MeshLoader.h"
#include "../OpenGLCommon/.h/LinearAlgebra.h"

using namespace LinearAlgebra;
using namespace std;

struct Vertex {
	Vec3 position;
	Vec3 normal;
	Vec3 tex_coords;
};

struct Texture {
	unsigned int id;
	string type;
};
class Mesh
{
private:
	// Mesh Data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	unsigned int VAO, VBO, EBO;
	void SetupMesh();

public:
	
	Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures) : 
		vertices(_vertices), indices(_indices), textures(_textures) {}
	void Draw(const Shader& shader);
};

