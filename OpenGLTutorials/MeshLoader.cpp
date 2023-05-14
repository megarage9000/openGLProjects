#include "MeshLoader.h"

bool load_mesh(const char* file_name, GLuint* vao, int* point_count) {
	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate);

	return false;
}