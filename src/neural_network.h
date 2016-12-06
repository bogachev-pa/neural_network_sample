#ifndef _NEURAL_NETWORK_H
#define _NEURAL_NETWORK_H

#include "plot.h"
#include "training_set.h"
#include "layer.h"

#include <vector>

class Neural_network {
public:
	Neural_network(unsigned int num_inputs, unsigned int num_outputs,
			unsigned int num_layers);
	virtual ~Neural_network(void);

	void check_training(const Training_set& training_set) const;
	void train_online(const Training_set& training_set, const Plot& plot);
	std::vector<double> get_output(const std::vector<double>& input) const;

	unsigned int num_inputs;
	unsigned int num_outputs;
	unsigned int num_layers;

	std::vector<Layer> layers;
protected:
	void set_random_weights(void);
};

#endif //_NEURAL_NETWORK_H
