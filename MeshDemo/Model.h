#pragma once

#ifndef MODEL
#define MODEL

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{

public:
	Model(char* path) {
		LoadModel(path);
	}
	void Draw(Shader& shader);
private:
	vector<Mesh> meshes;
	string directory;

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string type_name);
};

#endif

