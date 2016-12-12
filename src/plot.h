#ifndef _PLOT_H
#define _PLOT_H

#include "training_set.h"

#include <string>
#include <vector>

/* Forward declaraion, cause neuron.h also needs to include plot.h */
class Neural_network;

class Plot
{
public:
	Plot(void);
	~Plot(void)
	{}

	class Point {
	public:
		double x;
		double y;
	};

	void init_weights(unsigned int num_inputs, unsigned int num_outputs) const;
	void init_coord(std::string coord_file_path);
	void finalize_coord(void) const ;
	void make_training_set_datasheet(const Training_set *training_set) const;
	void make_weights_datasheet(const Neural_network *nn, unsigned int num) const;
	void make_output_datasheet(const Neural_network *nn, unsigned int training_num) const;
	void run_plot(void) const;
private:
	std::vector<double> normalize_coordinates(const std::vector<double>& points) const;

	std::vector<Point *> coordinate_plane;
	double x_max;
	double x_min;
	double y_max;
	double y_min;

	bool coord_initialized;
};

#endif //_PLOT_H
