#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#include <stdlib.h>
#include <iterator>
#include <array>
// Need to determine matrix multiplication



namespace LinearAlgebra {
    void matrix4_multi(float a[], float b[], int a_len, int b_len, float result_arr[]);
    // Assumes length are equal
    void matrix4_multi(float a[], float b[], float result_arr[]);
}


#endif 