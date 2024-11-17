#include "Mesh.h"
#include <algorithm>
#include <iterator>

void Mesh::SetupMesh() {

	// Setting up VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	
	// Setting up VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	// Setting up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Enable position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Enable normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Enable colours
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glEnableVertexAttribArray(2);

	// Enable texture coordinates
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	std::vector<unsigned int> diffuseSet;
	std::fill_n(std::back_inserter(diffuseSet), 32, 0);
	std::vector<unsigned int> specularSet;
	std::fill_n(std::back_inserter(specularSet), 32, 0);

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		string number;

		string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr - 1);
			diffuseSet[diffuseNr - 1] = 1;
			shader.SetInt(std::string("diffuse_samplers[" + number + "]").c_str(), i);
			diffuseNr++;
		}
		
		else if (name == "texture_specular") {
			number = std::to_string(specularNr - 1);
			specularSet[diffuseNr - 1] = 1;
			shader.SetInt(std::string("specular_samplers[" + number + "]").c_str(), i);
			specularNr++;
		}

		// Textures named as 'material.texture_diffuse1' for example
		// shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	shader.UseShader();
	shader.SetUIntArray("specular_set", specularSet.data(), 32);
	shader.SetUIntArray("diffuse_set", diffuseSet.data(), 32);

	// Draw Mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}