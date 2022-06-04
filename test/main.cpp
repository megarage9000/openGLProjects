#include <iostream>
#include <array>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iomanip>      // std::setprecision
#include "LinearAlgebra.h"




int main() {
   
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

    float result[16];
    LinearAlgebra::matrix4_inv(matrix_a, result);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << result[i * 4 + j] << " ";
        }
        std:: cout << '\n';
    }

    float identity[16];
    LinearAlgebra::matrix4_multi(result, matrix_a, identity);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << round(identity[i * 4 + j]) << " ";
        }
        std:: cout << '\n';
    }

    float result_vec4[4];
    LinearAlgebra::matrix4_vec4_multi(matrix_b, vec_c, result_vec4);
    for(int i = 0; i < 4; i++) {
        std::cout << result_vec4[i] << " ";
    }
    std::cout << '\n';

}