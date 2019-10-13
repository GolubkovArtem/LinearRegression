#pragma once
#include "T2DMatrix.h"

T2DMatrix simple_linear_regression(const T2DMatrix & features, const T2DMatrix & answers);

void shuffle(std::vector<float> & data);

void cross_validation(const T2DMatrix & full_matrix, size_t control_number = 1, size_t blocks_number = 20);

float standard_error(const T2DMatrix & features, const T2DMatrix & answers, const T2DMatrix & work_matrix);

std::vector<std::vector<float>> get_combinations(float collection_size, float selected_number);

void get_data(const std::string & file_name, T2DMatrix & full_matrix);
