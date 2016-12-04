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
