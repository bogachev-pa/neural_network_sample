#include "perceptron.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Perceptron::Perceptron(unsigned int num_inputs, unsigned int num_outputs)
		: Neuron(num_inputs, num_outputs)
{
}

Perceptron::~Perceptron(void)
{
}

std::vector<double> Perceptron::get_output(const std::vector<double>& input)
{
	std::vector<double> weighted_output;
	std::vector<double> output;

	weighted_output = get_weighted_output(input);

	for (unsigned int i = 0; i < num_outputs; i++) {
		double value = weighted_output.at(i);

#ifdef NN_DEBUG
		// std::cout << "weighted_output[" << i << "] = " << value << std::endl;
#endif
		output.push_back(value >= 0 ? 1 : 0);
	}

	return output;
}
