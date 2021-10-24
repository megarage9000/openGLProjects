#include <iostream>
#include <array>
#include <stdio.h>
#include <string.h>
#include "LinearAlgebra.h"




int main() {
   
    float matrix_a[9] = {
        15, 14, 13, 
        1, 2, 3, 
        80, 90, 70
    };
    float matrix_b[9] = {
        18, 19, 20, 
        5, 6, 7, 
        100, 90, 80
    };
    float result[9];
    float det = LinearAlgebra::determinant_mat_3(matrix_a);
    std::cout << "det = " << det << '\n';
}