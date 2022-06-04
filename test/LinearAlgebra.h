#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#define EPISLON 0.000001
#include <stdlib.h>
#include <cmath>




void swap(float &a, float &b);
bool float_equals(float a, float b);

namespace LinearAlgebra {

    // --- Matrix 4 --- //

    // Copies contents from one matrix 4 to another
    void copy_from_matrix4(float src[], float dest[], int src_len, int dest_len);

    // Copies contents from one vector 4 to another
    void copy_from_vec4(float src[], float dest[], int src_len, int dest_len);

    // Performs matrix 4 x matrix 4
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);

    // Assumes array lengths are correct
    // Performs matrix 4 x matrix 4 / matrix 4 x vector 4 calculations
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    void matrix4_multi(float a[], float b[], float result_arr[], bool is_b_vec);

    // --- Overloaded functions Matrix 4 --- //
    // Performs a matrix 4(a_mat4) x vector 4(b_vec4) multiplication
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[], int a_len, int b_len, int result_len);

    // Assumes matrix 4 len = 16 and b_vec4, result_arr len = 4, performs
    // a matrix 4(a_mat4) x vector 4(b_vec4) multiplication
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[]);  

    // Assumes a, b, and result_arr lens = 16
    // Performs a matrix 4 x matrix 4 calculation
    void matrix4_multi(float a[], float b[], float result_arr[]);
    
    // Calculates inverse of a matrix
    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len);

    // Assumes length are correct. Calculates inverse of a matrix 4
    // - Uses the Minors + Cofactors + Adjugate method
    // - Link: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
    void matrix4_inv(float a[], float result_arr[]);

    // Gets minors values from a subset 3x3 matrix with a given row, column
    float matrix4_minors_val(float a[], int row, int col);

    // Performs an in-place transpose on the matrix
    void transpose_matrix4(float a[]);

    // --- Matrix 3 --- //

    // Copies contents from matrix 3 to another
    void copy_from_matrix3(float src[], float dest[], int src_len, int dest_len);

    // Copies contents from vector 3 to another
    void copy_from_vec3(float src[], float dest[], int src_len, int dest_len);
    
    // Performs matrix 3 x matrix 3
    // - For matrix 3 x vector calculation, set b to a vector 3, and pass true to is_b_vec
    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);
    
    // Assumes array lengths are correct
    // Performs matrix 3 x matrix 3 / matrix 3 x vector 3 calculation
    // - For matrix 3 x vector calculation, set b to a vector 3, and pass true to is_b_vec
    void matrix3_multi(float a[], float b[], float result_arr[], bool is_b_vec);

    // --- Overloaded functions Matrix 3 --- //
    // Performs matrix 3(a_mat3) x vector 3(b_vec3) multiplication
    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[], int a_len, int b_len, int result_len);

    // Performs matrix 3(a_mat3) x vector 3(b_vec3) multiplication
    // - Assumes a_mat3 len = 9, b_vec3 and result_arr len = 3
    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[]);

    // Performs matrix 3 x matrix 3 multiplication
    // - Assumes a, b, result_arr len = 9
    void matrix3_multi(float a[], float b[], float result_arr[]);

    // Assumes length are correct. Calculates inverse of a matrix 3
    void matrix3_inv(float a[], float result_arr[]);

    // Returns determinant of a given matrix3
    float determinant_matrix3(float a[]);
}

#endif 