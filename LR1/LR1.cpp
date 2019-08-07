#include "stdafx.h"
#include "T2DMatrix.h"
#include "ExtraFunctions.h"
#include <iostream>

int main() {
	float F[12] = {10, 14, 21, 23, 27, 32, 39, 45, 55, 61, 62, 68};
	float y[12] = {3.8, 4.8, 5.9, 6.1, 6.2, 6.3, 6.6, 7.4, 8.5, 9.7, 10.5, 12.4};
	T2DMatrix features(12, 1, F);
	T2DMatrix answers(12, 1, y);
	matrix_print(simple_linear_regression(features, answers));
    return 0;
}


