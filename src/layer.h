#ifndef _LAYER_H
#define _LAYER_H

#include "neuron.h"

#include <vector>

class Layer {
public:
	Layer(unsigned int num_inputs, unsigned int num_neurons);
	~Layer(void);

	std::vector<Neuron *> neurons;

	unsigned int num_neurons;

	std::vector<double> get_output(const std::vector<double>& input) const;
	double train_online(const std::vector<double>& input,
			const std::vector<double>& output_etalon);
	void set_random_weights(void);
};

#endif //_LAYER_H
