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

void print_usage(char **argv)
{
	std::cout << "Incorrect number of arguments" << std::endl;
	std::cout << "Usage:" << std::endl;
#if defined(NN_PLOT_COORD)
	std::cout << argv[0] << " <input_file_path> <coord_file_path>" << std::endl;
#elif defined(NN_CHECK_SEPARATE)
	std::cout << argv[0] << " <input_file_path> <check_file_path>" << std::endl;
#else
	std::cout << argv[0] << " <input_file_path>" << std::endl;
#endif
}

int check_args(int argc)
{
#if defined(NN_PLOT_COORD)
	int expected_argc = 3;
#elif defined(NN_CHECK_SEPARATE)
	int expected_argc = 3;
#else
	int expected_argc = 2;
#endif

	return argc != expected_argc;
}

int main(int argc, char **argv)
{
	Plot *p = NULL;
	std::string input_file_path;
	std::string coord_file_path;
	Training_set *t_check;

	configure_stdout();

	if (check_args(argc)) {
		print_usage(argv);
		return 1;
	}

	input_file_path = std::string(argv[1]);
	Training_set *t = new Training_set(input_file_path);
	t_check = t;
	Neural_network *nn = new Neural_network(t->num_inputs, t->num_outputs,
			NN_NUM_LAYERS);

	p = new Plot();
	p->init_weights(t->num_inputs, t->num_outputs);

#ifdef NN_PLOT_COORD
	coord_file_path = std::string(argv[2]);
	p->init_coord(coord_file_path);
	p->make_training_set_datasheet(t);
#endif

#ifdef NN_CHECK_SEPARATE
	std::string check_file_path = std::string(argv[2]);
	Training_set *t2 = new Training_set(check_file_path);
	t_check = t2;
#endif

	std::cout << "Check nn before training:" << std::endl;
	nn->check_training(t_check);

	std::cout << "Train nn." << std::endl;

#if defined(NN_TRAIN_BACK_PROPAGATE)
	nn->train_back_propagate(t, p);
#else
	nn->train_offline(t, p);
	/* nn->train_online(t, p); */
#endif

	std::cout << "Check nn after training:" << std::endl;
	nn->check_training(t_check);

#ifdef NN_PLOT_COORD
	p->finalize_coord();
	/* p->run_plot(); */
#endif

	delete p;
	delete nn;
	delete t;

	return 0;
}
