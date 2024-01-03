#include "../.h/MeshLoader.h"



#pragma region Mesh Functions
bool load_mesh(const char* file_name, GLuint* vao, int* point_count) {
	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}

	printf("%i animations\n", scene->mNumAnimations);
	printf("%i cameras\n", scene->mNumCameras);
	printf("%i lights\n", scene->mNumLights);
	printf("%i materials\n", scene->mNumMaterials);
	printf("%i meshes\n", scene->mNumMeshes);
	printf("%i textures\n", scene->mNumTextures);

	/* Get First Mesh Only*/
	const aiMesh* mesh = scene->mMeshes[0];
	printf(" %i vertices in mesh[0]\n", mesh->mNumVertices);

	/* Pass back number of vertex points in mesh */
	*point_count = mesh->mNumVertices;

	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	// We need to extract the vertices, normals, and texture coordinates

	GLfloat* points = NULL;
	GLfloat* normals = NULL;
	GLfloat* colours = NULL;
	GLfloat* texcoords = NULL;

	// Vertices
	if (mesh->HasPositions()) {

		points = new GLfloat[*point_count * 3 * sizeof(GLfloat)];

		for (int i = 0; i < *point_count; i++) {

			const aiVector3D* vector_pos = &(mesh->mVertices[i]);

			points[i * 3] = (GLfloat)vector_pos->x;
			points[i * 3 + 1] = (GLfloat)vector_pos->y;
			points[i * 3 + 2] = (GLfloat)vector_pos->z;
		}
	}

	// Normals
	if (mesh->HasNormals()) {

		printf("Mesh %s has normals!\n", file_name);

		normals = new GLfloat[*point_count * 3 * sizeof(GLfloat)];

		for (int i = 0; i < *point_count; i++) {

			const aiVector3D* normal = &(mesh->mNormals[i]);
			

			float x = (GLfloat)normal->x;
			float y = (GLfloat)normal->y;
			float z = (GLfloat)normal->z;

			normals[i * 3] = x;
			normals[i * 3 + 1] = y;
			normals[i * 3 + 2] = z;


		}

		printf("Printing normal values:\n");

		for (int i = 0; i < *point_count; i += 3) {
			
			printf("[%.5f, %.5f, %.5f], \n ", 
					normals[i],
					normals[i + 1],
					normals[i + 2]
				);

		}
	}

	// Colour vertices
	if (mesh->HasVertexColors(1)) {
		
		colours = new GLfloat[*point_count * 4 * sizeof(GLfloat)];

		for (int i = 0; i < *point_count; i++) {

			const aiColor4D* colour = &(mesh->mColors[1][i]);
			colours[i] = colour->r;
			colours[i + 1] = colour->g;
			colours[i + 2] = colour->b;
			colours[i + 3] = colour->a;
		}
	}

	// Texture Coordinates
	if (mesh->HasTextureCoords(0)) {

		texcoords = new GLfloat[*point_count * 2 * sizeof(GLfloat)];

		for (int i = 0; i < *point_count; i++) {

			const aiVector3D* texcoord = &(mesh->mTextureCoords[0][i]);

			texcoords[i * 2] = (GLfloat)texcoord->x;
			texcoords[i * 2 + 1] = (GLfloat)texcoord->y;
		}
	}

	if (mesh->HasPositions()) {

		GLuint vbo;	
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] points;
	}

	if (mesh->HasNormals()) {

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] normals;
	}


	if (mesh->HasTextureCoords(0)) {

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * *point_count * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] texcoords;
	}

	if (mesh->HasVertexColors(1)) {
		printf("Has colours!\n");
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * *point_count * sizeof(GLfloat), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] colours;
	}
	if (mesh->HasTangentsAndBitangents()) {
		// NB: could store/print tangents here
	}

	aiReleaseImport(scene);
	printf("Mesh Loaded!");

	return true;
}

#pragma endregion Mesh Functions


#pragma region Renderable Methods

void Renderable::AttachVBO(GLuint vbo, GLuint vao, GLuint index, GLuint size, GLenum type, GLsizei stride, GLboolean normalized, const void* pointer) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Renderable::GetVaoByName(const char* name) {
	return vao_maps[name];
}

#pragma endregion Renderable Methods

#pragma region Cube Methods

void CubeRenderer::InitializeMesh() {
	GLuint vao;
	int point_count = 36;
	//if (!load_mesh(MESH_FILE, &vao, &point_count)) {
	//	throw std::exception("Invalid Mesh");
	//}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertices[] = {         
		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
	};

	// Vertex VBO
	GLuint vertex_vbo;
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	float normals[] = {
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,

		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};

	// Normals VBO
	GLuint normal_vbo;
	glGenBuffers(1, &normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vao_data[vao] = point_count;
	vao_maps["Cube"] = vao;
}

CubeRenderer::CubeRenderer() {
	// InitializeMesh();
}

CubeRenderer::CubeRenderer(Shader shader) {
	this->shader = shader;
	InitializeMesh();
}

void CubeRenderer::Draw() {
	shader.UseShader();
	for (auto vao_data : vao_data) {
		GLuint vao = vao_data.first;
		int point_count = vao_data.second;
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, point_count);
		glBindVertexArray(0);
	}
}

#pragma endregion Cube Methods