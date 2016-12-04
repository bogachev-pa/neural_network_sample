#ifndef _TRAINING_SET_H
#define _TRAINING_SET_H

#include <iostream>
#include <string>
#include <vector>

class Training_set
{
public:
	class Training_data
	{
	public:
		Training_data(void)
		{}

		~Training_data(void)
		{}

		std::vector<double> input;
		std::vector<double> output;
	};

	Training_set(std::string init_file_path);
	~Training_set(void);

	void print_data(void);

	std::vector<Training_data> training_data_arr;

	unsigned int num_inputs;
	unsigned int num_outputs;
};

#endif //_TRAINING_SET_H
