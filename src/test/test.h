#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "utility/mathutil.h"

#define WIN_SIZE 800

struct stats_t {
	int cutoff = 0;
	double time_serial = 0;
	double time_parallel = 0;
	double speedup = 0;

	stats_t() {}

	stats_t(int cutoff, double time_serial, double time_parallel) :
		cutoff(cutoff),
		time_serial(time_serial),
		time_parallel(time_parallel),
		speedup(time_serial / time_parallel)
	{}

	void print_me() const {
		std::cout
			<< "cutoff:  " << cutoff << "\n"
			<< "serial:  " << time_serial << "\n"
			<< "parallel:" << time_parallel << "\n"
			<< "speedup: " << speedup << "\n"
			<< "\n"
			;
	}
};

std::vector<Vec2> generate_points(std::string filename);
std::vector<Vec2> generate_points(int num_of_points);
void output_points(const std::vector<Vec2>& points, std::string filename);
std::vector<Vec2> get_hull(const std::vector<Vec2>& points, int cutoff);
std::vector<Vec2> get_hull_parallel(const std::vector<Vec2>& points, int cutoff);
std::vector<Vec2> perform_test(const std::vector<Vec2>& points);