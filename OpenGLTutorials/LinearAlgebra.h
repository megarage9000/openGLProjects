#pragma once
#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <stdio.h>
#include <math.h>

void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int size);
void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int matrixARowSize, int matrixBColSize);
void matrixVectorMultiplication(float matrixA[], float vectorB[], float resultingVector[], int matrixSize, int vectorSize);
void test();

// This uses flexible array member, look it up!
typedef const struct matrixStruct {
	int rowCount;
	int colCount;
	float values[];
} matrix;

matrix* createMatrix(int rowCount, int colCount);
matrix* createMatrix(int rowCount, int colCount, float values[]);

// Optimized versions of matrix, matrix4 and matrix3
typedef const struct matrix4Struct {
	int dimension;
	float values[16];
} matrix4;

typedef const struct matrix3Struct {
	int dimension;
	float values[3];
} matrix3;

//TODO 
/*
	1. Create more linear algebra methods
	2. Create structs for matrix and vector, and helper methods
*/

#endif LINEAR_ALGEBRA_H