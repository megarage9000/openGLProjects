#include <iostream>
#include <array>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iomanip>      // std::setprecision
#include "LinearAlgebra.h"

void testMatrix3() {
    float matrix_a[9] = {
        42, 32, 10,
        64, 20, 19.3,
        100, 200, 0
    };

    float matrix_b[9] = {
        31, 22, 1,
        67, 0, 25.3,
        200, 300, 50
    };

    float vec_c[9] = {
        13, 23, 43
    };
    std::cout << "---------Matrix A-3 inverse----------\n";
    float result[9];
    LinearAlgebra::matrix3_inv(matrix_a, result);
    LinearAlgebra::print_mat3(result, 9);
    std::cout << "---------Matrix A-3 inverse x Identity----------\n";
    float identity[9];
    LinearAlgebra::matrix3_multi(result, matrix_a, identity);
    LinearAlgebra::print_mat3(identity, 9);
    std::cout << "---------Matrix B-3 x Vector C-3 ----------\n";
    float result_vec3[3];
    LinearAlgebra::print_vector(result_vec3, 3);
    std::cout << '\n';
    std::cout << "--------Matrix A-3 Transpose-----------\n";
    float transpose[9];
    LinearAlgebra::transpose_matrix3(matrix_a);
    LinearAlgebra::print_mat3(matrix_a, 9);
    std::cout << "--------Matrix A-3 Transpose-----------\n";
    LinearAlgebra::transpose_matrix3(matrix_a);
    LinearAlgebra::print_mat3(matrix_a, 9);
}


void testMatrix4() {
    float matrix_a[16] = {
        15, 20, 13, 12,
        1, 23, 3, 4,
        80, 90, 70, 60,
        10, 9, 8, 7
    };
    float matrix_b[16] = {
        1, 2, 30, 4,
        11, 18, 19, 20, 
        12, 5, 6, 7,
        13, 100, 90, 80, 
    };

    float vec_c[4] {
        2.0f, 3.0f, 100.0f, 80.0f
    };
    std::cout << "---------Invert Matrix A----------\n";
    float result[16];
    LinearAlgebra::matrix4_inv(matrix_a, result);
    LinearAlgebra::print_mat4(result, 16);
    std::cout << "--------Matrix A inverse x Identity-----------\n";
    float identity[16];
    LinearAlgebra::matrix4_multi(result, matrix_a, identity);
    LinearAlgebra::print_mat4(identity, 16);
    std::cout << "---------Matrix B x Vector C----------\n";
    float result_vec4[4];
    LinearAlgebra::print_vector(result_vec4, 4);
    std::cout << '\n';
}

void testVectors() {
    float vector_a[4] {4, 3, 1, 11};
    float vector_b[4] {6, 2, 1, 2};

    std::cout << "---------Normalize Vector A as result A----------\n";
    LinearAlgebra::normalize_vector(vector_a, 4);
    LinearAlgebra::print_vector(vector_a, 4);

    std::cout << "----------result A magnitude after normalization------------\n";
    float len_a = LinearAlgebra::vector_magnitude(vector_a, 4);
    std::cout << len_a << '\n';

    std::cout << "----------Vector A dot Vector B------------\n";
    len_a = LinearAlgebra::dot_product(vector_a, vector_b, 4, 4);
    std::cout << len_a << '\n';
    

    std::cout << "----------Vector A cross Vector B------------\n";
    float result_b[3];
    LinearAlgebra::cross_product_vec3(vector_a, vector_b, 4, 4, result_b);
    LinearAlgebra::print_vector(result_b, 3);
}

int main() {
    // testMatrix4();
    // testMatrix3();
    testVectors();
}
