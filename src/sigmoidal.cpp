#include "sigmoidal.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Sigmoidal::Sigmoidal(unsigned int num_inputs) : Neuron(num_inputs)
{
}

Sigmoidal::~Sigmoidal(void)
{
}

double Sigmoidal::get_output(const std::vector<double>& input) const
{
	double value = get_weighted_output(input);

	return 1 / (1 + exp(-1 * SIGMOIDAL_B_COEF * value));
}
