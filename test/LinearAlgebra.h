#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#include <stdlib.h>
// Need to determine matrix multiplication


void swap(float &a, float &b);

namespace LinearAlgebra {

    // --- Matrix 4 --- // 
    void copy_from_matrix4(float src[], float dest[], int src_len, int dest_len);

    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);
    // Assumes length are equal
    void matrix4_multi(float a[], float b[], float result_arr[]);

    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len);
    // Assumes length are equal
    void matrix4_inv(float a[], float result_arr[]);

    float matrix4_cofact_val(float a[], int row, int col);
    // Performs an in-place transpose on the matrix
    void transpose_matrix4(float a[]);

    // --- Matrix 3 --- //
    void copy_from_matrix3(float src[], float dest[], int src_len, int dest_len);
    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);
    // Assumes length are equal
    void matrix3_multi(float a[], float b[], float result_arr[]);
    float determinant_matrix3(float a[]);
    
    // --- Matrix 2? --- //
}


#endif 