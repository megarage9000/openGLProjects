#include "Mesh.h"

void Mesh::SetupMesh() {
	// Setting up VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Setting up VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	
	// Setting up EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


	// Enable position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Enable normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Enable texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

	/*
		Doing the above reads the vertices buffer as following 

		vertices = [Vertex [ position (vec3), normal (vec3), texture (array of 2), ...]

		ARRAY_BUFFER = [ (position.x ,position.y, position.z), (normal.x, normal.y, normal.z), (texture.u, texture.v), ...]
					 = [ position.x ,position.y, position.z, normal.x, normal.y, normal.z, texture.u, texture.v, ...]
	*/

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		// Textures named as 'material.texture_diffuse1' for example
		shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// Draw Mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	try {
		glBindVertexArray(0);
	}

	catch (std::exception& e) {
		std::cout << "ERROR IN DRAWING: " << e.what() << '\n';
	}
}