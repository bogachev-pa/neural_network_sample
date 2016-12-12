#include "layer.h"
#include "neuron.h"
#include "perceptron.h"
#include "sigmoidal.h"

#include <cmath>
#include <vector>

Layer::Layer(unsigned int num_inputs, unsigned int num_neurons)
	: num_neurons(num_neurons)
{
	for (unsigned int i = 0; i < num_neurons; i++) {
#if defined(NN_USE_SIGMOIDAL)
		neurons.push_back(new Sigmoidal(num_inputs));
#elif defined(NN_USE_PERCEPTRON)
		neurons.push_back(new Perceptron(num_inputs));
#else
#error "No supported neuron type config is set"
#endif
	}
}

Layer::~Layer(void)
{
	for (unsigned int i = 0; i < neurons.size(); i++) {
		delete neurons.at(i);
	}
}

std::vector<double> Layer::get_output(const std::vector<double>& input) const
{
	std::vector<double> output;

	for (unsigned int i = 0; i < num_neurons; i++) {
		Neuron *neuron = neurons.at(i);
		double output_value;

		output_value = neuron->get_output(input);
		output.push_back(output_value);
	}

	return output;
}

double Layer::train_online(const std::vector<double>& input,
			const std::vector<double>& output_etalon)
{
	double max_delta = 0;

	for (unsigned int i = 0; i < num_neurons; i++) {
		Neuron *neuron = neurons.at(i);
		double delta;

		delta = neuron->train_online(input, output_etalon.at(i));

		if (fabs(delta) > max_delta)
			max_delta = fabs(delta);
	}

	return max_delta;
}

void Layer::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_neurons; i++) {
		neurons.at(i)->set_random_weights();
	}
}
