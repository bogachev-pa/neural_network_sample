#ifndef _NEURON_H
#define _NEURON_H

#include "plot.h"
#include "training_set.h"

#include <vector>

class Neuron {
public:
	Neuron(unsigned int num_inputs, unsigned int num_outputs);
	virtual ~Neuron(void);

	virtual std::vector<double> get_output(const std::vector<double>& inputs) const = 0;
	void check_training(const Training_set& training_set) const;
	void train_online(const Training_set& training_set, const Plot& plot);

	std::vector<std::vector<double> > w;
	unsigned int num_inputs;
	unsigned int num_outputs;

protected:
	void print_weights(void) const;
	void set_random_weights(void);
	std::vector<double> get_weighted_output(const std::vector<double>& inputs) const;
};

#endif //_NEURON_H
