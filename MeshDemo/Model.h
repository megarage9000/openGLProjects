#pragma once

#ifndef MODEL
#define MODEL

#include "Mesh.h"
#include "../OpenGLCommon/.h/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{

public:
	Model(const char* path) {
		LoadModel(path);
	}
	void Draw(Shader& shader);
private:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string type_name);
};

unsigned int TextureFromFile(const char * path, const string &directory, bool gamma=false);
#endif

