#include "neuron.h"
#include "training_set.h"
#include "util.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

#define NUM_TEACHING_ITERATIONS 20
#define WEIGHTS_DELTA_ACCURACY 0.001

Neuron::Neuron(unsigned int num_inputs, unsigned int num_outputs) :
		num_inputs(num_inputs), num_outputs(num_outputs)
{
	for (unsigned int i = 0; i < num_inputs; i++) {
		w.push_back(std::vector<double>());
		for (unsigned int j = 0; j < num_outputs; j++) {
			w.at(i).push_back(0);
		}
	}

	print_weights();
}

Neuron::~Neuron(void)
{
}

void Neuron::print_weights(void) const
{
#ifdef NN_DEBUG
	std::cout << "Neuron weights (" << num_inputs << ", " << num_outputs << "):" << std::endl;

	for (unsigned int i = 0; i < num_inputs; i++) {
		std::cout << i << ":\t";
		for (unsigned int j = 0; j < num_outputs; j++) {
			std::cout << w.at(i).at(j) << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif
}

std::vector<double> Neuron::get_weighted_output(const std::vector<double>& inputs) const
{
	std::vector<double> weighted_output;

	/* Считаем взвешенную сумму для каждого output.
	 * Сначала итерируеум по outputs, потом по inputs. */
	for (unsigned int i = 0; i < num_outputs; i++) {
		double sum = 0;
		for (unsigned int j = 0; j < num_inputs; j++) {
			double weight = w.at(j).at(i);
			double input_signal = inputs.at(j);

			sum += weight * input_signal;
		}

		weighted_output.push_back(sum);
	}

	return weighted_output;
}

void Neuron::check_training(const Training_set& training_set) const
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

		if (output.size() > 1) {
			if (index_output == index_etalon) {
				num_correct_answers++;
			}
		} else {
			if ((int)round(output.at(0)) == (int)round(output_etalon.at(0))) {
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

void Neuron::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_inputs; i++) {
		for (unsigned int j = 0; j < num_outputs; j++) {
			w.at(i).at(j) = ((double) rand() / RAND_MAX);
		}
	}

	print_weights();
}

void Neuron::train_online(const Training_set& training_set, const Plot& plot)
{
	std::vector<Training_set::Training_data> training_data_arr = training_set.training_data_arr;
	unsigned int counter = 0;
	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		double max_delta = 0;

		for (unsigned int j = 0; j < training_data_arr.size(); j++) {
			std::vector<double> input = training_data_arr.at(j).input;
			std::vector<double> output_etalon = training_data_arr.at(j).output;
			std::vector<double> output;

			output = get_output(input);

			for (unsigned int k = 0; k < num_inputs; k++) {
				for (unsigned int l = 0; l < num_outputs; l++) {
					double delta = 0.5 * input.at(k) * (output.at(l) - output_etalon.at(l));

					if (fabs(delta) >= max_delta)
						max_delta = fabs(delta);

					w.at(k).at(l) -= delta;
				}
			}

			print_weights();

			plot.make_weights_datasheet(this, counter++);
		}

		plot.make_output_datasheet(this, i);

		if (max_delta < WEIGHTS_DELTA_ACCURACY) {
			std::cout << "Weights delta is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			return;
		}
	}
}
