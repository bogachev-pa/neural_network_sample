#ifndef _SIGMOIDAL_H
#define _SIGMOIDAL_H

#include "neuron.h"

#define SIGMOIDAL_B_COEF (0.4)

class Sigmoidal : public Neuron {
public:
	Sigmoidal(unsigned int num_inputs);
	~Sigmoidal(void);

	double get_output(const std::vector<double>& inputs) const;
};

#endif //_SIGMOIDAL_H
