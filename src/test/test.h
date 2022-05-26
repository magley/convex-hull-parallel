#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "utility/mathutil.h"

#define WIN_SIZE 900

struct stats_t {
	int cutoff = 0;
	double time_serial = 0;
	double time_parallel = 0;
	double speedup = 0;
	int point_count = 0;

	stats_t() {}

	stats_t(int point_count, int cutoff, double time_serial, double time_parallel) :
		point_count(point_count),
		cutoff(cutoff),
		time_serial(time_serial),
		time_parallel(time_parallel),
		speedup(time_serial / time_parallel)
	{}

	void print_me() const {
		std::cout
			<< "====================================\n"
			<< "points:  " << point_count << "\n"
			<< "cutoff:  " << cutoff << "\n"
			<< "serial:  " << time_serial << "\n"
			<< "parallel:" << time_parallel << "\n"
			<< "speedup: " << speedup << "\n"
		;
	}
};


std::vector<stats_t> run_tests(const std::vector<Vec2>& points, std::vector<Vec2>& out_points);
stats_t run_test(const std::vector<Vec2>& points, std::vector<Vec2>& out_points, int cutoff);
void write_test_result_to_file(std::string fname, const std::vector<stats_t>& stats);

std::vector<stats_t> run_speedup_test(int minPoints, int maxPoints, int step);
void write_speedup_result_to_file(std::string fname, const std::vector<stats_t>& stats);


std::vector<Vec2> generate_points(std::string filename);
std::vector<Vec2> generate_points(int num_of_points);
void output_points(const std::vector<Vec2>& points, std::string filename);
std::vector<Vec2> get_hull(const std::vector<Vec2>& points, int cutoff);
std::vector<Vec2> get_hull_parallel(const std::vector<Vec2>& points, int cutoff);