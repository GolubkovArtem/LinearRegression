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
	T2DMatrix(size_t height, size_t width, const std::vector<float> & matrix);
	T2DMatrix(const T2DMatrix & matrix);
	T2DMatrix & operator = (const T2DMatrix & matrix);

	void ChangeTranspose();

	size_t GetHeight() const;
	size_t GetWidth() const;
	T2DMatrix GetTranspose() const;

	float & operator ()(size_t height, size_t width);
	float operator ()(size_t height, size_t width) const;
	T2DMatrix operator * (const T2DMatrix & matrix) const;
	T2DMatrix BottomConcatenation(const T2DMatrix & matrix);

	T2DMatrix GetReverse() const;
	T2DMatrix GetRectangleMinor(size_t row_first, size_t row_last, size_t col_first, size_t col_last, bool include = false) const;
	T2DMatrix GetMinor(const std::vector<size_t> & row_nums, const std::vector<size_t> & col_nums, bool include = false) const;

};

void matrix_print(const T2DMatrix & matrix);