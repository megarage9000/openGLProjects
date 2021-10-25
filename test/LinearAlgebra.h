#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#include <stdlib.h>
// Need to determine matrix multiplication


void swap(float &a, float &b);

namespace LinearAlgebra {

    // --- Matrix 4 --- // 
    // Copies contents from one matrix 4 to another
    void copy_from_matrix4(float src[], float dest[], int src_len, int dest_len);
    // Copies contents from one vector 4 to another
    void copy_from_vec4(float src[], float dest[], int src_len, int dest_len);
    // Matrix 4 on matrix 4 multiplication
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[], bool is_b_vec);
    // Assumes length are equal
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    void matrix4_multi(float a[], float b[], float result_arr[], bool is_b_vec);
    // Assumes matrix 4 len = 16 and b_vec4, result_arr len = 4
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[]);
    // Calculates inverse of a matrix
    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len);
    // Assumes length are equal
    void matrix4_inv(float a[], float result_arr[]);
    // Gets cofactor values from a subset 3x3 matrix with a given row, column
    float matrix4_cofact_val(float a[], int row, int col);
    // Performs an in-place transpose on the matrix
    void transpose_matrix4(float a[]);

    // --- Matrix 3 --- //
    void copy_from_matrix3(float src[], float dest[], int src_len, int dest_len);
    // 
    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);
    // Assumes length are equal
    void matrix3_multi(float a[], float b[], float result_arr[]);
    float determinant_matrix3(float a[]);
    
    // --- Matrix 2? --- //
}


#endif 