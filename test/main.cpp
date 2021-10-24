#include <iostream>
#include <array>
#include <stdio.h>
#include <string.h>
#include "LinearAlgebra.h"




int main() {
   
    float matrix_a[16] = {
        15, 14, 13, 12,
        1, 2, 3, 4,
        80, 90, 70, 60,
        10, 9, 8, 7
    };
    float matrix_b[16] = {
        18, 19, 20, 21,
        5, 6, 7, 8,
        100, 90, 80, 70,
        50, 20, 75, 9
    };
    float result[16];
    LinearAlgebra::matrix4_multi(matrix_a, matrix_b, 16, 16, result);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << result[i * 4 + j] << ",";
        }
        std::cout << '\n';
    }
}