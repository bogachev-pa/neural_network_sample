#include "neuron.h"
#include "training_set.h"
#include "util.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

Neuron::Neuron(unsigned int num_inputs) : num_inputs(num_inputs)
{
	for (unsigned int i = 0; i < num_inputs; i++) {
		w.push_back(0);
	}

	print_weights();
}

Neuron::~Neuron(void)
{
}

void Neuron::print_weights(void) const
{
#ifdef NN_DEBUG
	std::cout << "Neuron weights: ";
	for (unsigned int i = 0; i < num_inputs; i++) {
		std::cout << w.at(i) << " ";
	}

	std::cout << std::endl;
#endif
}

double Neuron::get_weighted_output(const std::vector<double>& inputs) const
{
	double weighted_output = 0;

	for (unsigned int i = 0; i < num_inputs; i++) {
		double weight = w.at(i);
		double input_signal = inputs.at(i);

		weighted_output += weight * input_signal;
	}

	return weighted_output;
}

void Neuron::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_inputs; i++) {
		w.at(i) = ((double) rand() / RAND_MAX);
	}

	print_weights();
}

double Neuron::train_online(const std::vector<double>& input,
		double output_etalon)
{
	double output;
	double max_delta = 0;

	output = get_output(input);

	for (unsigned int i = 0; i < num_inputs; i++) {
		double delta = 0.5 * input.at(i) * (output - output_etalon);

		w.at(i) -= delta;

		if (fabs(delta) > max_delta)
			max_delta = fabs(delta);
	}

	return max_delta;
}
