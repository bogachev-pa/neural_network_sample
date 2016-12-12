#include "neural_network.h"
#include "layer.h"
#include "plot.h"
#include "training_set.h"
#include "util.h"

#include <cmath>
#include <climits>
#include <vector>

#define NUM_TEACHING_ITERATIONS 50
#define TRAINING_ACCURACY 0.001
#define VALIDATION_ACCURACY 0.01

Neural_network::Neural_network(unsigned int num_inputs,
		unsigned int num_outputs,
		unsigned int num_layers)
		: num_inputs(num_inputs),
		num_outputs(num_outputs),
		num_layers(num_layers)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		layers.push_back(new Layer(num_inputs, num_outputs));
	}
}

Neural_network::~Neural_network(void)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		delete layers.at(i);
	}
}

std::vector<double> Neural_network::get_output(
		const std::vector<double>& input) const
{
	std::vector<double> intermediate_input = input;
	std::vector<double> output;

	/* TODO: number of inputs/outputs synchronization? */
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer *layer = layers.at(i);

		output = layer->get_output(intermediate_input);
		intermediate_input = output;
	}

	return output;
}

void Neural_network::check_training(const Training_set *training_set) const
{
	unsigned int num_correct_answers = 0;
	std::vector<Training_set::Training_data *> training_data_arr = training_set->training_data_arr;

	for (unsigned int i = 0; i < training_data_arr.size(); i++) {
		Training_set::Training_data *t_data = training_data_arr.at(i);
		std::vector<double> input = t_data->input;
		std::vector<double> output_etalon = t_data->output;
		std::vector<double> output;
		output = get_output(input);

#ifdef NN_CLASSIFICATION_TASK
		unsigned int index_output = get_index_max(output);
		unsigned int index_etalon = get_index_max(output_etalon);

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
#else
		unsigned int num_correct_answers_cur = 0;

		for (unsigned int j = 0; j < output.size(); j++) {
			double delta = fabs(output_etalon.at(j) - output.at(j));
			if (delta < VALIDATION_ACCURACY) {
				num_correct_answers_cur++;
			}
		}

		if (num_correct_answers_cur == output.size()) {
			num_correct_answers++;
		}
#endif

#ifdef NN_DEBUG
		std::cout << "Training data #" << i << ":" << std::endl;
		std::cout << "IN ";
		for (unsigned int j = 0; j < input.size(); j++) {
			std::cout << input.at(j) << " ";
		}
		std::cout << std::endl;

		std::cout << "OUT ";
		for (unsigned int j = 0; j < output.size(); j++) {
			std::cout << output.at(j) << " ";
		}
#ifdef NN_CLASSIFICATION_TASK
		if (output.size() == 2)
			std::cout << "-> " << index_output;
#endif
		std::cout << std::endl;

		std::cout << "ETALON ";
		for (unsigned int j = 0; j < output_etalon.size(); j++) {
			std::cout << output_etalon.at(j) << " ";
		}
#ifdef NN_CLASSIFICATION_TASK
		if (output.size() == 2)
			std::cout << "-> " << index_etalon;
#endif
		std::cout << std::endl << std::endl;

#endif
	}

	std::cout << num_correct_answers << " out of " << training_data_arr.size() << " answers are correct" << std::endl;
}

void Neural_network::train_online(const Training_set *training_set, const Plot *plot)
{
	std::vector<Training_set::Training_data *> training_data_arr
			= training_set->training_data_arr;
	unsigned int counter = 0;

	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		double max_delta = 0;

		for (unsigned int j = 0; j < training_data_arr.size(); j++) {
			std::vector<double> input = training_data_arr.at(j)->input;
			std::vector<double> output_etalon = training_data_arr.at(j)->output;

			for (unsigned int k = 0; k < num_layers; k++) {
				Layer *layer = layers.at(k);
				double delta;

				delta = layer->train_online(input, output_etalon);

				if (fabs(delta) > max_delta)
					max_delta = fabs(delta);
			}

			plot->make_weights_datasheet(this, counter++);
		}

		plot->make_output_datasheet(this, i);

		if (max_delta < TRAINING_ACCURACY) {
			std::cout << "Weights delta is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			return;
		}
	}
}

double Neural_network::train_neuron_offline(const Training_set *training_set,
		Neuron *neuron, unsigned int num, double nu)
{
	std::vector<Training_set::Training_data *> training_data_arr
			= training_set->training_data_arr;
	double error_sum = 0;
	std::vector<double> deltas;

	for (unsigned int i = 0; i < neuron->num_inputs; i++) {
		deltas.push_back(0);
	}

	for (unsigned int j = 0; j < training_data_arr.size(); j++) {
		for (unsigned int i = 0; i < neuron->num_inputs; i++) {
			Training_set::Training_data *t_data = training_data_arr.at(j);

			std::vector<double> input = t_data->input;
			double output_etalon = t_data->output.at(num);
			double output_value;
			double delta;

			output_value = neuron->get_output(input);
			delta = nu * (output_value - output_etalon) * input.at(i);

			deltas.at(i) += delta;
			error_sum += 0.5 * pow(output_value - output_etalon, 2);
		}
	}

	for (unsigned int i = 0; i < neuron->num_inputs; i++) {
		neuron->w.at(i) -= deltas.at(i);
	}

	return error_sum;
}

void Neural_network::train_offline(const Training_set *training_set, const Plot *plot)
{
	unsigned int counter = 0;
	double nu_max;

	Train_error_nn train_error_nn = Train_error_nn(layers);

	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		for (unsigned int j = 0; j < num_layers; j++) {
			Layer *layer = layers.at(j);
			for (unsigned int k = 0; k < layer->num_neurons; k++) {
				Train_error_neuron *train_error_neuron = train_error_nn.error_neurons.at(j).at(k);
				Neuron *neuron = layer->neurons.at(k);
				double error_cur;

				neuron->print_weights();
				error_cur = train_neuron_offline(training_set,
						neuron, k, train_error_neuron->nu);

				train_error_neuron->update(error_cur);
			}
		}

		plot->make_weights_datasheet(this, counter++);
		plot->make_output_datasheet(this, i);

		nu_max = train_error_nn.get_max_nu();
		if (nu_max < TRAINING_ACCURACY) {
			std::cout << "Nu max is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			return;
		}
	}
}

void Neural_network::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer *l = layers.at(i);

		l->set_random_weights();
	}
}
