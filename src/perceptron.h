#ifndef _PERCEPTRON_H
#define _PERCEPTRON_H

#include "neuron.h"

class Perceptron : public Neuron {
public:
	Perceptron(unsigned int num_inputs);
	~Perceptron(void);

	double get_output(const std::vector<double>& inputs) const;
};

#endif //_PERCEPTRON_H
