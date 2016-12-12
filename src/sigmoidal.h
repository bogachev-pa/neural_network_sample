#ifndef _SIGMOIDAL_H
#define _SIGMOIDAL_H

#include "neuron.h"

class Sigmoidal : public Neuron {
public:
	Sigmoidal(unsigned int num_inputs);
	~Sigmoidal(void);

	double get_output(const std::vector<double>& inputs) const;
};

#endif //_SIGMOIDAL_H
