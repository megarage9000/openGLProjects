#pragma once
#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <stdio.h>
#include <math.h>
#include <malloc.h>

void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int size);
void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int matrixARowSize, int matrixBColSize);
void matrixVectorMultiplication(float matrixA[], float vectorB[], float resultingVector[], int matrixSize, int vectorSize);
void test();


// Optimized versions of matrix4 and matrix3
typedef struct matrix4Struct {
	int dimension;
	float values[16];
} matrix4;

typedef struct matrix3Struct {
	int dimension;
	float values[3];
} matrix3;

matrix4 createMatrix4();
matrix4 createMatrix4(float values[], int valuesSize);
matrix4 matrix4Multiplication(matrix4 matrixA, matrix4 matrixB);

matrix3 createMatrix3();
matrix3 createMatrix3(float values[]);

//TODO 
/*
	1. Create more linear algebra methods
	2. Create structs for matrix and vector, and helper methods
*/

#endif LINEAR_ALGEBRA_H