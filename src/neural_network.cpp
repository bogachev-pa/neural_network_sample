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

#if defined(NN_CLASSIFICATION_TASK)
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
#elif defined(NN_TIC_TAC_TOE_TASK)
		std::vector<double> output_normalized;
		unsigned int num_correct_answers_cur = 0;

		for (unsigned int j = 0; j < output.size(); j++) {
			double output_cur;
			double delta;

			/* FIXME: hardcode */
			if (output.at(j) < 0.2) {
				output_cur = 0;
			} else if (output.at(j) > 0.8) {
				output_cur = 1;
			} else {
				output_cur = 0.5;
			}

			output_normalized.push_back(output_cur);

			delta = fabs(output_etalon.at(j) - output_cur);
			if (delta < VALIDATION_ACCURACY) {
				num_correct_answers_cur++;
			}
		}

		if (num_correct_answers_cur == output.size()) {
			num_correct_answers++;
		}
#else
#error "No supported task type config is set"
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

#ifdef NN_TIC_TAC_TOE_TASK
		std::cout << "OUT NORMALIZED ";
		for (unsigned int j = 0; j < output.size(); j++) {
			std::cout << output_normalized.at(j) << " ";
		}
		std::cout << std::endl;
#endif

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

/* FIXME: adjust */
#define NU 4

void Neural_network::train_back_propagate_one(const Training_set::Training_data *training_data)
{
	std::vector<double> input = training_data->input;
	std::vector<double> output_etalon = training_data->output;
	std::vector<double> output;
	Layer *last_layer = layers.at(num_layers - 1);
	Layer *current_layer;

	output = get_output(input);

	for (unsigned int i = 0; i < last_layer->num_neurons; i++) {
		Neuron *neuron = last_layer->neurons.at(i);

		neuron->delta_cur = output.at(i) - output_etalon.at(i);
	}

	/* Back propagation */
	for (int i = num_layers - 2; i >= 0; i--) {
		current_layer = layers.at(i);

		for (unsigned int j = 0; j < current_layer->num_neurons; j++) {
			Neuron *neuron = current_layer->neurons.at(j);

			double delta_cur = 0;
			for (unsigned int k = 0; k < last_layer->num_neurons; k++) {
				Neuron *next_neuron = last_layer->neurons.at(k);

				double weight = next_neuron->w.at(j);
				double delta = next_neuron->delta_cur;

				delta_cur += weight * delta;
			}

			neuron->delta_cur = delta_cur;
		}

		last_layer = current_layer;
	}

	/* Calculating new weights */
	std::vector<double> current_input = input;

	for (unsigned int i = 0; i < num_layers; i++) {
		Layer *layer = layers.at(i);
		std::vector<double> current_output = layer->get_output(current_input);

		for (unsigned int j = 0; j < layer->num_neurons; j++) {
			Neuron *neuron = layer->neurons.at(j);
			double w_output = neuron->get_output(current_input);
			double d_S = w_output * (1 - w_output);

			for (unsigned int k = 0; k < neuron->num_inputs; k++) {
				neuron->w.at(k) -= NU * neuron->delta_cur * d_S * current_input.at(k);
			}
		}

		current_input = current_output;
	}
}

void Neural_network::train_back_propagate(const Training_set *training_set, const Plot *plot)
{
	std::vector<Training_set::Training_data *> training_data_arr
			= training_set->training_data_arr;

	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		for (unsigned int j = 0; j < training_data_arr.size(); j++) {
			train_back_propagate_one(training_data_arr.at(j));
		}
	}

	print_weights();
}

void Neural_network::train_online(const Training_set *training_set, const Plot *plot)
{
	std::vector<Training_set::Training_data *> training_data_arr
			= training_set->training_data_arr;
	unsigned int counter = 0;

	if (num_layers > 1) {
		std::cout << "Online training not allowed for multi-layer nn." << std::endl;
		std::cout << "Use back-propagation training." << std::endl;
	}

	Layer *layer = layers.at(0);

	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		double max_delta = 0;

		for (unsigned int j = 0; j < training_data_arr.size(); j++) {
			std::vector<double> input = training_data_arr.at(j)->input;
			std::vector<double> output_etalon = training_data_arr.at(j)->output;
			double delta;

			delta = layer->train_online(input, output_etalon);

			if (fabs(delta) > max_delta)
				max_delta = fabs(delta);

			plot->make_weights_datasheet(this, counter++);
		}

		plot->make_output_datasheet(this, i);

		if (max_delta < TRAINING_ACCURACY) {
			std::cout << "Weights delta is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			return;
		}
	}

	print_weights();
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

	if (num_layers > 1) {
		std::cout << "Offline training not allowed for multi-layer nn." << std::endl;
		std::cout << "Use back-propagation training." << std::endl;
	}

	Train_error_nn train_error_nn = Train_error_nn(layers);
	Layer *layer = layers.at(0);

	set_random_weights();

	for (unsigned int i = 0; i < NUM_TEACHING_ITERATIONS; i++) {
		for (unsigned int k = 0; k < layer->num_neurons; k++) {
			Train_error_neuron *train_error_neuron = train_error_nn.error_neurons.at(0).at(k);
			Neuron *neuron = layer->neurons.at(k);
			double error_cur;

			error_cur = train_neuron_offline(training_set,
					neuron, k, train_error_neuron->nu);

			train_error_neuron->update(error_cur);
		}

		plot->make_weights_datasheet(this, counter++);
		plot->make_output_datasheet(this, i);

		nu_max = train_error_nn.get_max_nu();
		if (nu_max < TRAINING_ACCURACY) {
			std::cout << "Nu max is too small, so exiting after " << i + 1 << " iterations." << std::endl;
			print_weights();
			return;
		}
	}

	print_weights();
}

void Neural_network::set_random_weights(void)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer *l = layers.at(i);

		l->set_random_weights();
	}
}

void Neural_network::print_weights(void)
{
	for (unsigned int i = 0; i < num_layers; i++) {
		Layer *layer = layers.at(i);
		for (unsigned int j = 0; j < layer->num_neurons; j++) {
			Neuron *neuron = layer->neurons.at(j);

			neuron->print_weights();
		}
	}
}