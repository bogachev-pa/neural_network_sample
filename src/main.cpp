#include "neural_network.h"
#include "perceptron.h"
#include "training_set.h"
#include "plot.h"

#include <iostream>
#include <iomanip>

void configure_stdout(void)
{
	// std::cout << std::fixed << std::setprecision(3);
}

int main(int argc, char **argv)
{
	std::string input_file_path;
	std::string coord_file_path;

	configure_stdout();

	if (argc != 3) {
		std::cout << "Incorrect number of arguments" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << argv[0] << " <input_file_path> <coord_file_path>" << std::endl;
		return 1;
	}

	input_file_path = std::string(argv[1]);
	coord_file_path = std::string(argv[2]);

	Training_set t = Training_set(input_file_path);
	/* For now hardcode number of layers as 1. */
	Neural_network nn = Neural_network(t.num_inputs, t.num_outputs, 1);
	Plot p = Plot(coord_file_path);

	p.init_plot_script(t.num_inputs, t.num_outputs);
	p.make_training_set_datasheet(t);

	std::cout << "Check nn before training:" << std::endl;
	nn.check_training(t);

	std::cout << "Train nn." << std::endl;
	/* nn.train_online(t, p); */
	nn.train_offline(t, p);

	std::cout << "Check nn after training:" << std::endl;
	nn.check_training(t);

	p.finalize_plot_script();

	/* p.run_plot(); */

	return 0;
}
