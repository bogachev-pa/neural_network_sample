#ifndef _NEURON_H
#define _NEURON_H

#include "plot.h"
#include "training_set.h"

#include <vector>

class Neuron {
public:
	Neuron(unsigned int num_inputs);
	virtual ~Neuron(void);

	virtual double get_output(const std::vector<double>& inputs) const = 0;
	void set_random_weights(void);
	double train_online(const std::vector<double>& input,
			double output_etalon);
	void print_weights(void) const;

	std::vector<double> w;
	unsigned int num_inputs;
	double get_weighted_output(const std::vector<double>& inputs) const;
};

#endif //_NEURON_H
