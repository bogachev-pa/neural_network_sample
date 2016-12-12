#ifndef _NEURAL_NETWORK_H
#define _NEURAL_NETWORK_H

#include "plot.h"
#include "training_set.h"
#include "layer.h"

#include <cmath>
#include <cfloat>
#include <vector>

#define TRAIN_ERROR_NU_UP_MODIFIER 1.5
#define TRAIN_ERROR_NU_DOWN_MODIFIER 2
#define TRAIN_ERROR_UP_STREAK_LIMIT 1

class Neural_network {
public:
	class Train_error_neuron {
	public:
		Train_error_neuron(void) : error_prev(DBL_MAX), nu(0.5), successes_in_a_row(0)
		{}

		void update(double error_new)
		{
			error_new = fabs(error_new);

			if (error_new < error_prev) {
				successes_in_a_row++;
				if (successes_in_a_row >= TRAIN_ERROR_UP_STREAK_LIMIT) {
					nu *= TRAIN_ERROR_NU_UP_MODIFIER;
					successes_in_a_row = 0;
				}
			} else {
				nu /= TRAIN_ERROR_NU_DOWN_MODIFIER;
				successes_in_a_row = 0;
			}

			error_prev = error_new;
		}

		double error_prev;
		double nu;
		unsigned int successes_in_a_row;
	};

	class Train_error_nn {
	public:
		Train_error_nn(const std::vector<Layer *>& layers)
		{
			for (unsigned int i = 0; i < layers.size(); i++) {
				error_neurons.push_back(std::vector<Train_error_neuron *>());

				for (unsigned int j = 0; j < layers.at(i)->num_neurons; j++) {
					error_neurons.at(i).push_back(new Train_error_neuron());
				}
			}
		}

		double get_max_nu(void)
		{
			double nu_max = 0;

			for (unsigned int i = 0; i < error_neurons.size(); i++) {
				for (unsigned int j = 0; j < error_neurons.at(i).size(); j++) {
					double nu = error_neurons.at(i).at(j)->nu;
					if (nu > nu_max)
						nu_max = nu;
				}
			}

			return nu_max;
		}

		std::vector<std::vector<Train_error_neuron *> > error_neurons;
	};

	Neural_network(unsigned int num_inputs, unsigned int num_outputs,
			unsigned int num_layers);
	virtual ~Neural_network(void);

	void check_training(const Training_set *training_set) const;
	void train_online(const Training_set *training_set, const Plot *plot);
	void train_offline(const Training_set *training_set, const Plot *plot);
	std::vector<double> get_output(const std::vector<double>& input) const;

	unsigned int num_inputs;
	unsigned int num_outputs;
	unsigned int num_layers;

	std::vector<Layer *> layers;
protected:
	void set_random_weights(void);
	double train_neuron_offline(const Training_set *training_set,
			Neuron *neuron, unsigned int num, double nu);
};

#endif //_NEURAL_NETWORK_H
