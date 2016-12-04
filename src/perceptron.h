#ifndef _PERCEPTRON_H
#define _PERCEPTRON_H

#include "neuron.h"

class Perceptron : public Neuron {
public:
	Perceptron(unsigned int num_inputs, unsigned int num_outputs);
	~Perceptron(void);

	std::vector<double> get_output(const std::vector<double>& inputs) const;
};

#endif //_PERCEPTRON_H
