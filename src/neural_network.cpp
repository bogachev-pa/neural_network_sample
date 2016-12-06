#include "neural_network.h"
#include "layer.h"
#include "plot.h"
#include "training_set.h"
#include "util.h"

#include <cmath>
#include <vector>

#define NUM_TEACHING_ITERATIONS 20
#define WEIGHTS_DELTA_ACCURACY 0.001

Neural_network::Neural_network(unsigned int num_inputs,
		unsigned int num_outputs,
		unsigned int num_layers)
		: num_inputs(num_inputs),
		num_outputs(num_outputs),
		num_layers(num_layers)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		layers.push_back(Layer(num_inputs, num_outputs));
	}
}

Neural_network::~Neural_network(void)
{
}

std::vector<double> Neural_network::get_output(
		const std::vector<double>& input) const
{
	std::vector<double> intermediate_input = input;
	std::vector<double> output;

	/* TODO: number of inputs/outputs synchronization? */
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer layer = layers.at(i);

		output = layer.get_output(intermediate_input);
		intermediate_input = output;
	}

	return output;
}

void Neural_network::check_training(const Training_set& training_set) const
{
	unsigned int num_correct_answers = 0;
	std::vector<Training_set::Training_data> training_data_arr = training_set.training_data_arr;

	for (unsigned int i = 0; i < training_data_arr.size(); i++) {
		std::vector<double> input = training_data_arr.at(i).input;
		std::vector<double> output_etalon = training_data_arr.at(i).output;
		std::vector<double> output;
		unsigned int index_output;
		unsigned int index_etalon;

		output = get_output(input);
		index_output = get_index_max(output);
		index_etalon = get_index_max(output_etalon);

		if (output.size() == 1) {
			if ((int)round(output.at(0)) == (int)round(output_etalon.at(0))) {
				num_correct_answers++;
			}
		} else if (output.size() == 2) {
			unsigned int answer = output_to_code(output);
			unsigned int etalon = output_to_code(output_etalon);

			if (answer == etalon) {
				num_correct_answers++;
			}

		} else {
			if (index_output == index_etalon) {
				num_correct_answers++;
			}
		}

#ifdef NN_DEBUG
		std::cout << "Training data #" << i << ":" << std::endl;
		std::cout << "IN ";
		for (unsigned int j = 0; j < input.size(); j++) {
			std::cout << input.at(j) << " ";
		}
		std::cout  << std::endl;

		std::cout << "OUT ";
		for (unsigned int j = 0; j < output.size(); j++) {
			std::cout << output.at(j) << " ";
		}
		std::cout << "-> " << index_output;
		std::cout  << std::endl;

		std::cout << "ETALON ";
		for (unsigned int j = 0; j < output_etalon.size(); j++) {
			std::cout << output_etalon.at(j) << " ";
		}
		std::cout << "-> " << index_etalon;
		std::cout  << std::endl << std::endl;

#endif
	}

	std::cout << num_correct_answers << " out of " << training_data_arr.size() << " answers are correct" << std::endl;
}

void Neural_network::train_online(const Training_set& training_set, const Plot& plot)
{
	std::vector<Training_set::Training_data> training_data_arr
			= training_set.training_data_arr;

	unsigned int counter = 0;
	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		double max_delta = 0;

		for (unsigned int j = 0; j < training_data_arr.size(); j++) {
			std::vector<double> input = training_data_arr.at(j).input;
			std::vector<double> output_etalon = training_data_arr.at(j).output;

			for (unsigned int k = 0; k < num_layers; k++) {
				double delta;

				delta = layers.at(k).train_online(input, output_etalon);

				if (fabs(delta) > max_delta)
					max_delta = fabs(delta);
			}

			plot.make_weights_datasheet(this, counter++);
		}

		plot.make_output_datasheet(this, i);

		if (max_delta < WEIGHTS_DELTA_ACCURACY) {
			std::cout << "Weights delta is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			return;
		}
	}
}

void Neural_network::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer l = layers.at(i);

		l.set_random_weights();
	}
}
