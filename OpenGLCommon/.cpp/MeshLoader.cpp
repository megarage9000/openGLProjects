#include "../.h/MeshLoader.h"

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

	if (mesh->HasTangentsAndBitangents()) {
		// NB: could store/print tangents here
	}

	aiReleaseImport(scene);
	printf("Mesh Loaded!");

	return true;
}