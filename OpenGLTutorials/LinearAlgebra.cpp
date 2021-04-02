#include "LinearAlgebra.h"



void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int size) {

		int index = 0;
		int currentRow = 0;
		int currentCol = 0;
		int length = (int)sqrt((double)size);

		printf("Length = %d\n", length);
		for (int row = 0; row < length; row++) {

			for (int col = 0; col < length; col++) {
				float result = 0;
				for (int i = 0; i < length; i++) {
					int column = col + (i * length);
					result += matrixA[row + i] * matrixB[column];
				}
				resultingMatrix[index] = result;
				index++;
			}			
		}
	

	// matrices are stored in column order:
	/*
		a b c d		f g h q
		e f g h		s t y o
		i j k l		q w z x
		m n o p		l c v d
	*/

}


void test() {
	float matrixA[] = {
		1.0f, 1.0f, 1.0f,
		2.0f, 2.0f, 2.0f,
		3.0f, 3.0f, 3.0f,
	};

	float resultingMatrix[9];

	
	matrixMultiplication(matrixA, matrixA, resultingMatrix, (int)sizeof(matrixA)/sizeof(matrixA[0]));
	
	int size = sizeof(resultingMatrix) / sizeof(resultingMatrix[0]);
	for (int i = 0; i < size; i++) {
		printf("%f, ", resultingMatrix[i]);
	}
	printf("Size = %d", sizeof(resultingMatrix));
}
