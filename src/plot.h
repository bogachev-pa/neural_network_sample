#ifndef _PLOT_H
#define _PLOT_H

#include "training_set.h"

#include <string>
#include <vector>

/* Forward declaraion, cause neuron.h also needs to include plot.h */
class Neuron;

class Plot
{
public:
	Plot(std::string coord_file_path);
	~Plot(void)
	{}

	class Point {
	public:
		double x;
		double y;
	};

	void init_plot_script(void) const;
	void finalize_plot_script(void) const ;
	void make_training_set_datasheet(const Training_set& training_set) const;
	void make_weights_datasheet(const Neuron *neuron, unsigned int num) const;
	void make_output_datasheet(const Neuron *neuron, unsigned int training_num) const;
	void run_plot(void) const;
private:
	std::vector<double> normalize_coordinates(const std::vector<double>& points) const;

	std::vector<Point> coordinate_plane;
	double x_max;
	double x_min;
	double y_max;
	double y_min;
};

#endif //_PLOT_H
