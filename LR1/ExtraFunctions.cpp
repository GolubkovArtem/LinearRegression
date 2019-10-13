#include "stdafx.h"
#include "ExtraFunctions.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>


T2DMatrix simple_linear_regression(const T2DMatrix & features, const T2DMatrix & answers) {
	T2DMatrix result((features.GetTranspose() * features).GetReverse());
	if (result.GetHeight() == 0 || features.GetHeight() != answers.GetHeight()) {
		T2DMatrix error(0, 0);
		return error;
	}
	return result * features.GetTranspose() * answers;
}

void shuffle(std::vector<float> & data) {
	srand(time(0));
	std::shuffle(data.begin(), data.end(), std::default_random_engine(rand()));
	
}

void cross_validation(const T2DMatrix & full_matrix, size_t control_number, size_t blocks_number) {
	if (control_number >= blocks_number || full_matrix.GetHeight() / blocks_number < 1) {
		return;
	}
	std::vector<float> new_order;
	for (size_t iterator = 0; iterator < full_matrix.GetHeight(); ++iterator) {
		new_order.push_back(iterator);
	}
	shuffle(new_order);
	std::vector<std::vector<size_t>> blocks_rows;
	for (size_t iterator = 0; iterator < blocks_number; ++iterator) {
		std::vector<size_t> buf;
		for (size_t iterator_2 = iterator * new_order.size() / blocks_number;
			iterator_2 < (iterator + 1) * new_order.size() / blocks_number; ++iterator_2) {
			buf.push_back(new_order[iterator_2]);
		}
		blocks_rows.push_back(buf);
	}
	std::vector<size_t> blocks_cols;
	for (size_t iterator = 0; iterator < full_matrix.GetWidth(); ++iterator) {
		blocks_cols.push_back(iterator);
	}
	std::vector<T2DMatrix> blocks;
	for (auto & i : blocks_rows) {
		blocks.push_back(full_matrix.GetMinor(i, blocks_cols, true));
	}
	T2DMatrix work_matrix(0, 0);
	float main_error = -1;
	for (auto & combination : get_combinations(blocks_number, control_number)) {
		T2DMatrix study_data(0, full_matrix.GetWidth());
		T2DMatrix test_data(0, full_matrix.GetWidth());
		for (size_t iterator = 0; iterator < combination.size(); ++iterator) {
			if (combination[iterator] == true) {
				test_data.BottomConcatenation(blocks[iterator]);
			}
			else {
				study_data.BottomConcatenation(blocks[iterator]);
			}
		}
		T2DMatrix buf_matrix = simple_linear_regression(study_data.GetRectangleMinor(0, study_data.GetHeight() - 1, 0, study_data.GetWidth() - 2, true),
			study_data.GetRectangleMinor(0, study_data.GetHeight() - 1, study_data.GetWidth() - 1, study_data.GetWidth() - 1, true));
		float error = standard_error(test_data.GetRectangleMinor(0, test_data.GetHeight() - 1, 0, test_data.GetWidth() - 2, true),
			test_data.GetRectangleMinor(0, test_data.GetHeight() - 1, test_data.GetWidth() - 1, test_data.GetWidth() - 1, true), buf_matrix);
		if (main_error == -1 || error < main_error) {
			main_error = error;
			work_matrix = buf_matrix;
		}
	}
	std::cout << "work_matrix\n";
	matrix_print(work_matrix);
	std::cout << "\nwith standard error on study data = " << main_error << "\n";
}

float standard_error(const T2DMatrix & features, const T2DMatrix & answers, const T2DMatrix & work_matrix) {
	float result = 0;
	for (size_t iterator = 0; iterator < features.GetWidth(); ++iterator) {
		result += pow(answers(iterator, 0) - (features.GetRectangleMinor(iterator, iterator, 0, features.GetWidth(), true).GetTranspose() * work_matrix)(0, 0), 2);
	}
	result /= answers.GetHeight();
	return pow(result, 0.5);
}

std::vector<std::vector<float>> get_combinations(float collection_size, float selected_number) {
	std::vector<std::vector<float>> result;
	std::vector<float> extra_vector(collection_size - selected_number, 0);
	for (int iterator = 0; iterator < selected_number; ++iterator) {
		extra_vector.push_back(1);
	}
	int extra_iterator = collection_size - 1;
	while (extra_iterator != 0) {
		result.push_back(extra_vector);
		extra_iterator = collection_size - 1;
		while (extra_iterator != 0) {
			if (extra_vector[extra_iterator] == 1 && extra_vector[extra_iterator - 1] == 0) {
				extra_vector[extra_iterator] = 0;
				extra_vector[extra_iterator - 1] = 1;
				int unit_counter = 0;
				for (int iterator = extra_iterator + 1; iterator < collection_size; ++iterator)
				{
					if (extra_vector[iterator]) {
						++unit_counter;
					}
					extra_vector[iterator] = 0;
				}
				for (int iterator = collection_size - 1; iterator > collection_size - 1 - unit_counter; --iterator) {
					extra_vector[iterator] = 1;
				}
				break;
			}
			--extra_iterator;
		}
	}
	return result;
}

void get_data(const std::string & file_name, T2DMatrix & full_matrix) {
	std::ifstream file(file_name);
	std::string row;
	std::vector<float> result_row;
	size_t width = 0;
	while (std::getline(file, row)) {
		std::string buf = "";
		for (auto & letter : row) {
			if (isdigit(letter) != 0 || letter == *".") {
				buf += letter;
			}
			else if (letter == *",") {
				result_row.push_back(::atof(buf.c_str()));
				buf.clear();
			}
		}
		result_row.push_back(::atof(buf.c_str()));
		if (width == 0) {
			width = result_row.size();
		}
	}
	file.close();
	full_matrix = T2DMatrix(result_row.size() / width, width, result_row);
}