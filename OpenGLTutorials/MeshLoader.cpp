#include "MeshLoader.h"

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




}