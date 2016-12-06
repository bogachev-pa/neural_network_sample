#include "perceptron.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Perceptron::Perceptron(unsigned int num_inputs) : Neuron(num_inputs)
{
}

Perceptron::~Perceptron(void)
{
}

double Perceptron::get_output(const std::vector<double>& input) const
{
	double value = get_weighted_output(input);

	return value >= 0 ? 1 : 0;
}
