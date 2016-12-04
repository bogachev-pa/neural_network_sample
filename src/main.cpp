#include "neuron.h"
#include "perceptron.h"
#include "training_set.h"

#include <iostream>
#include <iomanip>

void configure_stdout(void)
{
	// std::cout << std::fixed << std::setprecision(3);
}

int main(int argc, char **argv)
{
	configure_stdout();

	if (argc != 2) {
		std::cout << "Incorrect number of arguments" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << argv[0] << " <input_file_path>" << std::endl;
		return 1;
	}

	Training_set t = Training_set(argv[1]);
	Perceptron n = Perceptron(t.num_inputs, t.num_outputs);

	std::cout << "Check nn before training:" << std::endl;
	n.check_training(t);

	std::cout << "Train nn." << std::endl;
	n.train_online(t);

	std::cout << "Check nn after training:" << std::endl;
	n.check_training(t);

	return 0;
}
