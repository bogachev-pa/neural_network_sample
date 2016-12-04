#ifndef _NEURON_H
#define _NEURON_H

#include "training_set.h"

#include <vector>

class Neuron {
public:
	Neuron(unsigned int num_inputs, unsigned int num_outputs);
	virtual ~Neuron(void);

	virtual std::vector<double> get_output(const std::vector<double>& inputs) = 0;
	void check_training(const Training_set& training_set);
	void train_online(const Training_set& training_set);
protected:
	void print_weights(void);
	void set_random_weights(void);
	std::vector<double> get_weighted_output(const std::vector<double>& inputs);

	std::vector<std::vector<double> > w;
	unsigned int num_inputs;
	unsigned int num_outputs;
};

#endif //_NEURON_H
