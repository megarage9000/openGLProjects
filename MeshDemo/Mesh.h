#pragma once

#ifndef MESH
#define MESH

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GarageMathLibrary.h"
#include "GarageShaders.h"

using namespace GarageLinearAlgebra;
using namespace std;

struct Vertex {
	std::array<float, 3> position;
	std::array<float, 3> normal;

	// TODO: Make a vec2 class
	std::array<float, 2> textureCoordinates;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
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
		vertices(_vertices), indices(_indices), textures(_textures) {
		SetupMesh();
	}
	void Draw(Shader& shader);
};

#endif