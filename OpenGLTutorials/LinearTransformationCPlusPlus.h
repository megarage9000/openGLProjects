#pragma once
#ifndef LINEAR_TRANSFORMATIONS_CPLUSPLUS
#define LINEAR_TRANSFORMATIONS_CPLUSPLUS


// Attempt Quaternion Stuff here!!!
// https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/06_vcam_with_quaternion/main.cpp

/*
	An extension to the LinearAlgebra, LinearTransformations
	namespaces, with the help of vectors!!!!

	- NOTE WILL NEED TO TRANSPOSE MATRICES UPON ENTRY
*/
#include <vector>
#include "LinearTransformations.h"

namespace LinearTransformationCPlusPlus
{

	using namespace std;

	vector<float> identity(int size);
	vector<float> mag_1(int size);


	vector<float> matrix_multiplication(vector<float> matrix_a, vector<float> matrix_b);
	vector<float> matrix_inverse(vector<float> matrix);
	vector<float> matrix_transpose(vector<float> matrix);
	

	vector<float> translate(float dx, float dy, float dz);

	vector<float> rotate_euler(float angle, bool is_x = false, bool is_y = false, bool is_z = false);

	vector<float> cross_product(vector<float> vector_a, vector<float> vector_b);

	vector<float> view_matrix(vector<float> up_vector, vector<float> focus_position, vector<float> cam_world_position);

	vector<float> projection_matrix(float near, float far, float fov, float range, float aspect);

	// Quanternion functions
	vector<float> slerp(vector<float> versor_a, vector<float> versor_b, float t);

	vector<float> versor(float angle, float x, float y, float z);

	vector<float> versor_multiplication(vector<float> versor_a, vector<float> versor_b);

	vector<float> to_quanternion(vector<float> versor);
};

#endif // ! LINEAR_TRANSFORMATIONS_CPLUSPLUS



