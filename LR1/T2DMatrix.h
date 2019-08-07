#pragma once
#include <vector>

class T2DMatrix {
private:
	size_t Height;
	size_t Width;
	float ** Matrix;
	bool Transpose;
public:
	~T2DMatrix();
	T2DMatrix() = delete;
	T2DMatrix(size_t height, size_t width, float * matrix = nullptr);
	T2DMatrix(const T2DMatrix & matrix);

	void ChangeTranspose();

	size_t GetHeight() const;
	size_t GetWidth() const;
	T2DMatrix GetTranspose() const;

	float & operator ()(size_t height, size_t width);
	float operator ()(size_t height, size_t width) const;
	T2DMatrix operator * (const T2DMatrix & matrix) const;

	T2DMatrix GetReverse() const;
	T2DMatrix GetMinor(size_t row_num, size_t col_num) const;
	T2DMatrix GetMinor(const std::vector<size_t> & row_nums, const std::vector<size_t> & col_nums) const;
};

void matrix_print(const T2DMatrix & matrix);