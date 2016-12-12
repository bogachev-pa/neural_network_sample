#include "training_set.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

Training_set::Training_set(std::string init_file_path)
{
	std::ifstream init_file;
	std::string line;
	std::stringstream iss;

	init_file.open(init_file_path.c_str());

	if (!init_file.is_open()) {
		std::cout << "failed to open file " << init_file_path << std::endl;
		throw std::invalid_argument(init_file_path);
	}

	if (!getline(init_file, line)) {
		std::cout << "failed to read metadata from file " << init_file_path << std::endl;
		throw std::invalid_argument(init_file_path);
	}

	iss.str(line);
	iss >> num_inputs;
	iss >> num_outputs;
	iss.clear();

	while (getline(init_file, line)) {
		if (line.find("//") != std::string::npos) {
			continue;
		}

		Training_data *t_data = new Training_data();
		iss.str(line);

		/* Поляризационный сигнал */
		t_data->input.push_back(1);

		for (unsigned int i = 0; i < num_inputs; i++) {
			double input;

			iss >> input;
			t_data->input.push_back(input);
		}

		for (unsigned int i = 0; i < num_outputs; i++) {
			double output;

			iss >> output;
			t_data->output.push_back(output);
		}

		training_data_arr.push_back(t_data);
		iss.clear();
	}

	/* Учет поляризационного сигнала */
	num_inputs++;

	print_data();
}

Training_set::~Training_set(void)
{
	for (unsigned int i = 0; i < training_data_arr.size(); i++) {
		delete training_data_arr.at(i);
	}
}

void Training_set::print_data(void)
{
#ifdef NN_DEBUGp
	std::cout << "Training set:" << std::endl;

	for (unsigned int i = 0; i < training_data_arr.size(); i++) {
		Training_data *t_data = training_data_arr.at(i);

		for (unsigned int j = 0; j < num_inputs; j++) {
			std::cout << t_data->input.at(j) << " ";
		}

		for (unsigned int j = 0; j < num_outputs; j++) {
			std::cout << t_data->output.at(j) << " ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif
}
