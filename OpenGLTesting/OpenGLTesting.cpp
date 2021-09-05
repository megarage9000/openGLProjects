#include "pch.h"
#include "CppUnitTest.h"
#include "../OpenGLTutorials/LinearAlgebra.h"
#include "../OpenGLTutorials/LinearAlgebra.cpp"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OpenGLTesting
{
	TEST_CLASS(OpenGLTesting)
	{
	public:
		
		TEST_METHOD(matrix4MultiplicationTest)
		{
			float valuesA[] = {
				312, 21, 12, 14,
				21, 2, 20, 12,
				58, 4, 31, 1,
				3, 3, 43, 9
			};

			float valuesB[] = {
				52, 58, 45, 7,
				2, 0, 1, 6,
				3, 3, 12, 89,
				100, 48, 21, 21
			};

			float valuesC[] = {
				17702, 18804, 14499, 3672,
				2356, 1854, 1439, 2191,
				3217, 3505, 3007, 3210,
				1191, 735, 843, 4055
			};

			matrix4 matrixA = createMatrix4(
				valuesA, 
				16);
			matrix4 matrixB = createMatrix4(
				valuesB,
				16);
			matrix4 matrixC = createMatrix4(
				valuesC,
				16);

			matrix4 matrixResult = matrix4Multiplication(matrixA, matrixB);
			
			printf("Matrix A:\n");
			printMatrix4(matrixA);
			printf("Matrix B:\n");
			printMatrix4(matrixB);
			printf("Matrix C (Supposed Result):\n");
			printMatrix4(matrixC);
			printf("Actual result:\n");
			printMatrix4(matrixResult);

			
			Assert::IsTrue(areEqualMatrix4(matrixResult, matrixC));
		}
	};
}
