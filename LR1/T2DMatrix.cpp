#include "stdafx.h"
#include "T2DMatrix.h"
#include <iostream>

T2DMatrix::~T2DMatrix() {
	for (size_t iterator = 0; iterator < Height; ++iterator) {
		delete[] Matrix[iterator];
	}
	delete[] Matrix;
}

T2DMatrix::T2DMatrix(size_t height, size_t width, float * matrix) {
	Height = height;
	Width = width;
	Transpose = false;
	Matrix = new float *[Height];
	for (size_t iterator = 0; iterator < Height; ++iterator) {
		Matrix[iterator] = new float[Width];
	}
	if (matrix != nullptr) {
		for (size_t iterator = 0; iterator < Height; ++iterator) {
			for (size_t iterator_2 = 0; iterator_2 < Width; ++iterator_2) {
				Matrix[iterator][iterator_2] = matrix[iterator * Width + iterator_2];
			}
		}
	}
}

T2DMatrix::T2DMatrix(const T2DMatrix & matrix) {
	Height = matrix.Height;
	Width = matrix.Width;
	Transpose = matrix.Transpose;
	Matrix = new float *[Height];
	for (size_t iterator = 0; iterator < Height; ++iterator) {
		Matrix[iterator] = new float[Width];
	}
	for (size_t iterator = 0; iterator < GetHeight(); ++iterator) {
		for (size_t iterator_2 = 0; iterator_2 < GetWidth(); ++iterator_2) {
			(*this)(iterator, iterator_2) = matrix(iterator, iterator_2);
		}
	}
}

void T2DMatrix::ChangeTranspose() {
	Transpose = !Transpose;
}

size_t T2DMatrix::GetHeight() const {
	if (Transpose) {
		return Width;
	}
	return Height;
}

size_t T2DMatrix::GetWidth() const {
	if (Transpose) {
		return Height;
	}
	return Width;
}

T2DMatrix T2DMatrix::GetTranspose() const {
	T2DMatrix result(*this);
	result.ChangeTranspose();
	return result;
}

float & T2DMatrix::operator ()(size_t height, size_t width) {
	if (Transpose) {
		return Matrix[width][height];
	}
	return Matrix[height][width];
}

float T2DMatrix::operator ()(size_t height, size_t width) const {
	if (Transpose) {
		return Matrix[width][height];
	}
	return Matrix[height][width];
}

T2DMatrix T2DMatrix::operator * (const T2DMatrix & matrix) const {
	if (GetWidth() != matrix.GetHeight()) {
		T2DMatrix error(0, 0);
		return error;
		//error
	}
	T2DMatrix result(GetHeight(), matrix.GetWidth());
	for (size_t iterator = 0; iterator < result.GetHeight(); ++iterator) {
		for (size_t iterator_2 = 0; iterator_2 < result.GetWidth(); ++iterator_2) {
			result(iterator, iterator_2) = 0;
			for (size_t iterator_3 = 0; iterator_3 < GetWidth(); ++iterator_3) {
				result(iterator, iterator_2) += (*this)(iterator, iterator_3) * matrix(iterator_3, iterator_2);
			}
		}
	}
	return result;
}

T2DMatrix T2DMatrix::GetReverse() const {
	if (GetHeight() != GetWidth()) {
		T2DMatrix error(0, 0);
		return error;
	}
	T2DMatrix identity(GetHeight(), GetWidth());
	for (size_t iterator = 0; iterator < GetHeight(); ++iterator) {
		for (size_t iterator_2 = 0; iterator_2 < GetWidth(); ++iterator_2) {
			identity(iterator, iterator_2) = ((iterator == iterator_2) ? 1. : 0.);
		}
	}
	T2DMatrix matrix_copy(*this);
	for (size_t iterator = 0; iterator < GetHeight(); ++iterator) {
		float diagonal_elem = matrix_copy(iterator, iterator);
		size_t row_number = iterator;
		while (diagonal_elem == 0 && row_number < GetHeight() - 1) {
			++row_number;
			diagonal_elem = matrix_copy(row_number, iterator);
		}
		if (diagonal_elem == 0) {
			T2DMatrix error(0, 0);
			return error;
		}
		else if (row_number != iterator) {
			std::swap(matrix_copy.Matrix[row_number], matrix_copy.Matrix[iterator]);
			std::swap(identity.Matrix[row_number], identity.Matrix[iterator]);
		}
		if (matrix_copy(iterator, iterator) != 1) {
			float factor = matrix_copy(iterator, iterator);
			for (size_t iterator_2 = 0; iterator_2 < GetWidth(); ++iterator_2) {
				matrix_copy(iterator, iterator_2) /= factor;
				identity(iterator, iterator_2) /= factor;
			}
		}
		for (size_t iterator_2 = 0; iterator_2 < GetHeight(); ++iterator_2) {
			if (iterator_2 == iterator || matrix_copy(iterator_2, iterator) == 0) {
				continue;
			}
			float factor = matrix_copy(iterator_2, iterator) / matrix_copy(iterator, iterator);
			for (size_t iterator_3 = 0; iterator_3 < GetWidth(); ++iterator_3) {
				matrix_copy(iterator_2, iterator_3) -= matrix_copy(iterator, iterator_3) * factor;
				identity(iterator_2, iterator_3) -= identity(iterator, iterator_3) * factor;
			}
		}
	}
	return identity;
}

T2DMatrix T2DMatrix::GetMinor(size_t row_num, size_t col_num) const {
	return GetMinor(std::vector<size_t>{row_num}, std::vector<size_t>{col_num});
}

T2DMatrix T2DMatrix::GetMinor(const std::vector<size_t> & row_nums, const std::vector<size_t> & col_nums) const {	
	std::vector<size_t> new_row_nums;
	for (size_t iterator = 0; iterator < GetHeight(); ++iterator) {
		new_row_nums.push_back(iterator);
		for (auto & iterator_2 : row_nums) {
			if (iterator_2 == iterator) {
				new_row_nums.pop_back();
				break;
			}
		}
	}
	std::vector<size_t> new_col_nums;
	for (size_t iterator = 0; iterator < GetWidth(); ++iterator) {
		new_col_nums.push_back(iterator);
		for (auto & iterator_2 : col_nums) {
			if (iterator_2 == iterator) {
				new_col_nums.pop_back();
				break;
			}
		}
	}
	T2DMatrix result(new_row_nums.size(), new_col_nums.size());
	size_t row_num = 0;
	for (auto & iterator : new_row_nums) {
		size_t col_num = 0;
		for (auto & iterator_2 : new_col_nums) {
			result(row_num, col_num) = (*this)(iterator, iterator_2);
			++col_num;
		}
		++row_num;
	}
	return result;
}

void matrix_print(const T2DMatrix & matrix) {
	for (size_t iterator = 0; iterator < matrix.GetHeight(); ++iterator) {
		for (size_t iterator_2 = 0; iterator_2 < matrix.GetWidth(); ++iterator_2) {
			for (size_t iterator_3 = 0; iterator_3 < 5 - log10(fabs(matrix(iterator, iterator_2)) + 1); ++iterator_3) {
				std::cout << " ";
			}
			if (matrix(iterator, iterator_2) >= 0) {
				std::cout << " ";
			}
			if (matrix(iterator, iterator_2) == 0) {
				printf("%.3f ", fabs(matrix(iterator, iterator_2)));
			}
			else {
				printf("%.3f ", matrix(iterator, iterator_2));
			}
		}
		std::cout << "\n";
	}

}