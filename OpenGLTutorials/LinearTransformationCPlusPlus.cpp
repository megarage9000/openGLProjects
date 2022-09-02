#include "LinearTransformationCPlusPlus.h"

namespace LinearTransformationCPlusPlus
{
	using namespace std;
	vector<float> identity(int size) {
		if (size == 9) {
			return vector<float>(
				begin(LinearAlgebra::IDENTITY_3),
				end(LinearAlgebra::IDENTITY_3)
				);
		}
		if (size == 16) {
			return vector<float>(
				begin(LinearAlgebra::IDENTITY_4),
				end(LinearAlgebra::IDENTITY_4)
				);
		}
		return vector<float>();
	}
	vector<float> mag_1(int size) {
		return vector<float>(size, 1.0);
	}

	vector<float> matrix_multiplication(vector<float> matrix_a, vector<float> matrix_b) {
		int length_a = matrix_a.size();
		int length_b = matrix_b.size();

		if (length_a == length_b) {
			if (length_a == 16) {
				float result[16];
				LinearAlgebra::matrix4_multi(matrix_a.data(), matrix_b.data(), result);
				return vector<float>(begin(result), end(result));
			}
			else if (length_a == 9) {
				float result[9];
				LinearAlgebra::matrix3_multi(matrix_a.data(), matrix_b.data(), result);
				return vector<float>(begin(result), end(result));
			}
		}

		throw runtime_error("Invalid matrices for multiplication");
	}

	vector<float> matrix_inverse(vector<float> matrix) {
		int length = matrix.size();

		if (length == 16) {
			float result[16];
			LinearAlgebra::matrix4_inv(matrix.data(), result);
			return vector<float>(begin(result), end(result));
		}
		else if (length == 9) {
			float result[9];
			LinearAlgebra::matrix3_inv(matrix.data(), result);
			return vector<float>(begin(result), end(result));
		}

		throw runtime_error("Matrix not valid for inverse");
	}
	
	vector<float> matrix_transpose(vector<float> matrix) {
		int length = matrix.size();

		if (length == 16) {
			float result[16];
			copy(begin(matrix), end(matrix), begin(result));
			LinearAlgebra::transpose_matrix4(result);
			return vector<float>(begin(result), end(result));
		}
		else if (length == 9) {
			float result[9];
			copy(begin(matrix), end(matrix), begin(result));
			LinearAlgebra::transpose_matrix3(result);
			return vector<float>(begin(result), end(result));
		}

		throw runtime_error("Matrix not valid for transpose");
	}

	vector<float> translate(float dx, float dy, float dz) {
		float transform_matrix[16];
		float translation_vector[3] = { dx, dy, dz };
		LinearTransformations::translate(transform_matrix, translation_vector);
		return vector<float>(begin(transform_matrix), end(transform_matrix));
	}

	vector<float> rotate_euler(float angle, bool is_x, bool is_y, bool is_z) {
		float transform_matrix[16];

		if (is_x) {
			LinearTransformations::rotateX(transform_matrix, angle);
		}

		if (is_y) {
			std::cout << "doing y!";
			LinearTransformations::rotateY(transform_matrix, angle);
		}

		if (is_z) {
			LinearTransformations::rotateZ(transform_matrix, angle);
		}

		return vector<float>(begin(transform_matrix), end(transform_matrix));
	}

	vector<float> cross_product(vector<float> vector_a, vector<float> vector_b) {
		int length_a = vector_a.size();
		int length_b = vector_b.size();

		if (length_a == length_b) {
			if (length_a == 4) {
				float result[4];
				LinearTransformations::cross_product_vec4(vector_a.data(), vector_b.data(), result);
				return vector<float>(begin(result), end(result));
			}
			else if (length_a == 3) {
				float result[3];
				LinearAlgebra::cross_product_vec3(vector_a.data(), vector_b.data(), 3, 3, result);
				return vector<float>(begin(result), end(result));
			}
		}

		throw runtime_error("Vectors are invalid for cross product");
	}

	vector<float> view_matrix(vector<float> up_vector, vector<float> focus_position, vector<float> cam_world_position) {
		float result[16];
		LinearTransformations::view_matrix(result, up_vector.data(), focus_position.data(), cam_world_position.data());
		return vector<float>(begin(result), end(result));
	}

	vector<float> projection_matrix(float near, float far, float fov, float range, float aspect) {
		float result[16];
		LinearTransformations::projection_matrix(result, near, far, fov, range, aspect);
		return vector<float>(begin(result), end(result));
	}
};
