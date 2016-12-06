#include "util.h"

#include <vector>

unsigned int get_index_max(const std::vector<double>& arr)
{
	double max = arr.at(0);
	unsigned int index_max = 0;

	for (unsigned int i = 1; i < arr.size(); i++) {
		if (arr.at(i) > max) {
			max = arr.at(i);
			index_max = i;
		}
	}

	return index_max;
}

/* FIXME: работает только для 2-х выходов */
unsigned int output_to_code(std::vector<double> output)
{
	if (output.at(0) == 0 && output.at(1) == 0)
		return 0;

	if (output.at(0) == 0 && output.at(1) == 1)
		return 1;

	if (output.at(0) == 1 && output.at(1) == 1)
		return 2;

	return 1;
}
