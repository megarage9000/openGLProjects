#include "LinearAlgebra.h"



void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int size) {

		int index = 0;
		int length = (int)sqrt((double)size);

		for (int rowA = 0; rowA < length; rowA++) {
			for (int colB = 0; colB < length; colB++) {
				float result = 0;
				for (int i = 0; i < length; i++) {
					int columnResult = colB + (i * length);
					int rowResult = (rowA * length) + i;
					result += matrixA[rowResult] * matrixB[columnResult];
				}
				resultingMatrix[index] = result;
				index++;
			}
		}

	// matrices are stored in column
	// order:
	/*
		a b c d		f g h q
		e f g h		s t y o
		i j k l		q w z x
		m n o p		l c v d
	*/
}


void test() {
	float matrixA[] = {
		2.0f, 2.0f, 2.0f, 2.0f,
		3.0f, 3.0f, 3.0f, 3.0f,
		4.0f, 4.0f, 4.0f, 4.0f,
		5.0f, 5.0f, 5.0f, 5.0f
	};

	float matrixB[] = {
		3.0f, 3.0f, 3.0f, 3.0f,
		6.0f, 6.0f, 6.0f, 6.0f,
		7.0f, 7.0f, 7.0f, 7.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};


	float resultingMatrix[16];
	
	matrixMultiplication(matrixA, matrixB, resultingMatrix, (int)sizeof(matrixA)/sizeof(matrixA[0]));
	
	int size = sizeof(resultingMatrix) / sizeof(resultingMatrix[0]);
	for (int i = 0; i < size; i++) {
		printf("%f, ", resultingMatrix[i]);
	}
	printf("Size = %d", size);
}
