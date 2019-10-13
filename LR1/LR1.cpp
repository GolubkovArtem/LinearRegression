#include "stdafx.h"
#include "T2DMatrix.h"
#include "ExtraFunctions.h"
#include <iostream>

int main() {
	T2DMatrix full_matrix(0, 0);
	get_data("data.txt", full_matrix);
	matrix_print(full_matrix);
	cross_validation(full_matrix, 2, 4);
    return 0;
}



