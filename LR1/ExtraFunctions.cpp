#include "stdafx.h"
#include "ExtraFunctions.h"

T2DMatrix simple_linear_regression(const T2DMatrix & features, const T2DMatrix & answers) {
	T2DMatrix result((features.GetTranspose() * features).GetReverse());
	if (result.GetHeight() == 0 || features.GetHeight() != answers.GetHeight()) {
		T2DMatrix error(0, 0);
		return error;
	}
	return result * features.GetTranspose() * answers;
}
