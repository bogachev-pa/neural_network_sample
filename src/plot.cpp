#include "neuron.h"
#include "plot.h"
#include "util.h"

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <cmath>

#define PLOT_FOLDER "plot/"
#define PLOT_TEMP_FOLDER PLOT_FOLDER "temp/"
#define PLOT_PATH PLOT_FOLDER "plot.pl"
#define PLOT_WEIGHTS_PATH PLOT_FOLDER "plot_weights.pl"
#define PLOT_WEIGHTS_TEMP_PATH PLOT_TEMP_FOLDER "w.dat"
#define POINTS_OUTPUT_PATH PLOT_FOLDER "points_output.dat"
#define NUMBER_OF_POINTS 3

Plot::Plot(std::string coord_file_path)
{
	std::ifstream coord_file;
	std::string line;
	std::stringstream iss;

	coord_file.open(coord_file_path.c_str());

	if (!coord_file.is_open()) {
		std::cout << "failed to open file " << coord_file_path << std::endl;
		throw std::invalid_argument(coord_file_path);
	}

	while (getline(coord_file, line)) {
		Point point;

		iss.str(line);
		iss >> point.x;
		iss >> point.y;
		iss.clear();

		coordinate_plane.push_back(point);
	}


	x_min = coordinate_plane.at(0).x;
	x_max = coordinate_plane.at(0).x;
	y_min = coordinate_plane.at(0).y;
	y_max = coordinate_plane.at(0).y;

	for (unsigned int i = 1; i < coordinate_plane.size(); i++) {
		double x = coordinate_plane.at(i).x;
		double y = coordinate_plane.at(i).y;

		if (x > x_max)
			x_max = x;

		if (x < x_min)
			x_min = x;

		if (y > y_max)
			y_max = y;

		if (y < y_min)
			y_min = y;
	}

#ifdef NN_DEBUG
	std::cout << "Basic points of coordinate plane:" << std::endl;
	for (unsigned int i = 0; i < coordinate_plane.size(); ++i) {
		std::cout << coordinate_plane.at(i).x << " " << coordinate_plane.at(i).y << std::endl;
	}
#endif
}

void Plot::run_plot(void) const
{
	if (system("gnuplot " PLOT_PATH)) {
		std::cout << "Failed to run gnuplot" << std::endl;
	}
}

void Plot::init_plot_script(void) const
{
	if (system("rm -rf " PLOT_FOLDER)) {
		std::cout << "Failed to remove plot folder " << PLOT_FOLDER << std::endl;
		throw std::invalid_argument(PLOT_FOLDER);
	}

	if (system("mkdir -p " PLOT_FOLDER)) {
		std::cout << "Failed to create folder " << PLOT_FOLDER << std::endl;
		throw std::invalid_argument(PLOT_FOLDER);
	}

	if (system("mkdir -p " PLOT_TEMP_FOLDER)) {
		std::cout << "Failed to create folder " << PLOT_TEMP_FOLDER << std::endl;
		throw std::invalid_argument(PLOT_TEMP_FOLDER);
	}

	std::ofstream script(PLOT_PATH);
	script << "set xlabel \"x1\";\n";
	script << "set ylabel \"x2\";\n";
	script << "set colorbox vertical user origin .02,.1 size .04,.8;\n";
	script << "set view 0,0;\n";
	script << "unset ztics;\n";
	script << "set term x11;\n";
	script.close();

	std::ofstream weights_script(PLOT_WEIGHTS_PATH);
	weights_script << "set term x11;" << std::endl;
	weights_script << "reset;" << std::endl;
	weights_script << "set style data lines;" << std::endl;
	/* FIXME: hardcode */
	weights_script << "plot \"" << PLOT_WEIGHTS_TEMP_PATH << "\" using 1:3 title \"w1\" lt 2 lc rgb \"blue\" lw 3, "
			<< "\"" << PLOT_WEIGHTS_TEMP_PATH << "\" using 1:4 title \"w2\" lt 2 lc rgb \"red\" lw 3;" << std::endl;
	weights_script.close();
}

void Plot::finalize_plot_script(void) const
{
	std::ofstream script(PLOT_PATH, std::ofstream::app);
	script << "pause -1;" << std::endl;
	script.close();

	std::ofstream weights_script(PLOT_WEIGHTS_PATH, std::ofstream::app);
	weights_script << "pause -1;" << std::endl;
	weights_script.close();
}

std::vector<double> Plot::normalize_coordinates(const std::vector<double>& points) const
{
	std::vector<double> normalized_point;
	double width = x_max - x_min;
	double height = y_max - y_min;

	if (points.size() != NUMBER_OF_POINTS) {
		std::cout << "invalid number of inputs for the plot" << std::endl;
		throw std::invalid_argument("");
	}

	/* point[0] - поляризатор, пропускаем */
	normalized_point.push_back(x_min + points.at(1) * width);
	normalized_point.push_back(y_min + points.at(2) * height);

	return normalized_point;
}

void Plot::make_training_set_datasheet(const Training_set& training_set) const
{
	std::ofstream datasheet;

	datasheet.open(POINTS_OUTPUT_PATH);

	for (unsigned long i = 0; i < training_set.training_data_arr.size(); ++i) {
		Training_set::Training_data t_data = training_set.training_data_arr.at(i);
		std::vector<double> normalized_points;
		unsigned int output_index;
		double output_value;

		normalized_points = normalize_coordinates(t_data.input);
		output_index = get_index_max(t_data.output);

		if (t_data.output.size() > 1)
			output_value = output_index;
		else
			output_value = (int)round(t_data.output.at(0));

		datasheet << normalized_points.at(0) << "\t" << normalized_points.at(1) << "\t" << output_value << std::endl;
	}

	datasheet.close();
}

void Plot::make_weights_datasheet(const Neuron *neuron, unsigned int num) const
{
	std::ofstream datasheet;

	datasheet.open(PLOT_WEIGHTS_TEMP_PATH, std::ofstream::app);

	datasheet << num << " ";

	for (unsigned int i = 0; i < neuron->num_inputs; i++) {
		for (unsigned int j = 0; j < neuron->num_outputs; j++) {
			datasheet << neuron->w.at(i).at(j) << " ";
		}
	}

	datasheet << std::endl;

	datasheet.close();
}

void Plot::make_output_datasheet(const Neuron *neuron, unsigned int training_num) const
{
	std::ofstream datasheet;
	std::string datasheet_path;
	std::stringstream iss;

	iss << PLOT_TEMP_FOLDER;
	iss << training_num;
	iss << ".dat";
	datasheet_path = iss.str();
	iss.clear();

	datasheet.open(datasheet_path.c_str());

	for (double x1 = 0; x1 < 1.01; x1 += 0.01) {
		for (double x2 = 0; x2 < 1.01; x2 += 0.01) {
			std::vector<double> points;
			std::vector<double> output;
			std::vector<double> normalized_point;
			unsigned int index_output;
			double output_value;

			points.push_back(1.0);
			points.push_back(x1);
			points.push_back(x2);

			output = neuron->get_output(points);
			index_output = get_index_max(output);
			normalized_point = normalize_coordinates(points);

			if (output.size() > 1)
				output_value = index_output;
			else
				output_value = (int)round(output.at(0));

			datasheet << normalized_point.at(0) << "\t" << normalized_point.at(1) << "\t" << output_value << std::endl;
		}

		datasheet << std::endl;
	}

	datasheet.close();

	std::ofstream script(PLOT_PATH, std::ofstream::app);
	script << "splot \'" << datasheet_path << "\' notitle with pm3d, \'" << POINTS_OUTPUT_PATH
			<<"\' title \'Training set\' w p pt 8 lc rgb \'green\';" << std::endl;
	script << "pause 0.1;" << std::endl;
}
