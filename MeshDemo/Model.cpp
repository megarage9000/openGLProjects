#include "Model.h"


void Model::Draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		shader.UseShader();
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(string path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Flip the textures
	stbi_set_flip_vertically_on_load(true);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR\n";
		return;
	}

	directory = path.substr(0, path.find_last_of('\\'));
	std::cout << "directory: " << directory << std::endl;
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	
	// Process node meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Repeat process for children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// Process vertex positions, normals, and texture coordinates

		// -- Position
		vertex.position[0] = mesh->mVertices[i].x;
		vertex.position[1] = mesh->mVertices[i].y;
		vertex.position[2] = mesh->mVertices[i].z;

		// -- Normal
		vertex.normal[0] = mesh->mNormals[i].x;
		vertex.normal[1] = mesh->mNormals[i].y;
		vertex.normal[2] = mesh->mNormals[i].z;

		// -- Vertex Colour
		if (mesh->mColors[1]) {              
			vertex.colour[0] = mesh->mColors[1][i].r;
			vertex.colour[1] = mesh->mColors[1][i].g;
			vertex.colour[2] = mesh->mColors[1][i].b;
			vertex.colour[3] = mesh->mColors[1][i].a;
		}
		else {
			vertex.colour[0] = 0.5f;
			vertex.colour[1] = 0.5f;
			vertex.colour[2] = 0.5f;
			vertex.colour[3] = 1.0f;
		}

		// Assimp allows up to 8 texture coordinates, we will only use the first set, if there's any
		if (mesh->mTextureCoords[0]) {
			vertex.textureCoordinates[0] = mesh->mTextureCoords[0][i].x;
			vertex.textureCoordinates[1] = mesh->mTextureCoords[0][i].y;
			// std::cout << "texture coords\n" << vertex.textureCoordinates[0] << ", " << vertex.textureCoordinates[1] << '\n';
		}
		else {
			// std::cout << "no texture coords\n";
			vertex.textureCoordinates[0] = 0.0f;
			vertex.textureCoordinates[1] = 0.0f;
		}

		vertices.push_back(vertex);
	}
	// Process Indices
	// - Each face contains 
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process Material
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		// Diffuse Maps
		vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// Specular Maps
		vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	
	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName) {

	vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {

		aiString str;
		material->GetTexture(type, i, &str);

		/*
		* Allows us to save textures in a "global setting" of a model,
		* so submeshes can reuse loaded textures
		*/
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				std::cout << "Skipping texture: " << str.C_Str() << std::endl;
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			std::cout << "Loading texture: " << str.C_Str() << std::endl;
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma) {

	string filename = string(path);
	filename = directory + '\\' + filename;

	unsigned int textureID;

	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		// Need to review what these do LOL
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}