#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "utility/mathutil.h"

#define WIN_SIZE 900

/// <summary>
/// Stores relevant data for one test case of the serial
/// and parallel divide and conquer algorithm.
/// </summary>
struct stats_t {
	int point_count = 0;
	int cutoff = 0;
	double time_serial = 0;
	double time_parallel = 0;
	double speedup = 0;

	stats_t() {}

	stats_t(int point_count, int cutoff, double time_serial, double time_parallel) :
		point_count(point_count),
		cutoff(cutoff),
		time_serial(time_serial),
		time_parallel(time_parallel),
		speedup(time_serial / time_parallel)
	{}
};

/// <summary>
/// Reads points from file. 
/// </summary>
/// <param name="filename">Name of the file.</param>
/// <returns>Vector representing the data read from disk.</returns>
std::vector<Vec2> generate_points(std::string filename);

/// <summary>
/// Generates points on the screen.
/// </summary>
/// <param name="num_of_points">How many points to generate.</param>
/// <returns>Vector of all generated points.</returns>
std::vector<Vec2> generate_points(int num_of_points);

/// <summary>
/// Writes vector of points to a file. File format follows the convention
/// used by generate_points().
/// </summary>
/// <param name="points">Vector of points.</param>
/// <param name="filename">Name of the new file.</param>
void output_points(const std::vector<Vec2>& points, std::string filename);

/// <summary>
/// Calls the serial and parallel convex hull algorithm on the input with
/// the specified cutoff.
/// </summary>
/// <param name="input">Algorithm input: a set of points.</param>
/// <param name="output">Algorithm output: the convex hull.</param>
/// <param name="cutoff">Recursion cutoff.</param>
/// <returns>stats_t object for the executed algorithms</returns>
stats_t run_test(const std::vector<Vec2>& input, std::vector<Vec2>& output, int cutoff);

/// <summary>
/// Calls run_test for randomly generated points in range [min_points, max_points].
/// </summary>
/// <param name="min_points">Number of points of the smallest test case.</param>
/// <param name="max_points">Number of points of the largest test case.</param>
/// <returns>One stats_t object for each test.</returns>
std::vector<stats_t> run_speedup_test(int min_points, int max_points);

std::vector<stats_t> run_speedup_graham(int min_points, int max_points);

/// <summary>
/// Calls run_test for given set of points with different values for cutoff.
/// </summary>
/// <param name="input">Input set of points.</param>
/// <param name="output">Output set of points.</param>
/// <returns>One stats_t object for each test.</returns>
std::vector<stats_t> run_cutoff_test(const std::vector<Vec2>& input, std::vector<Vec2>& output);

/// <summary>
/// Calls run_cutoff_test for randomly generated points in range [min_points, max_points]
/// and saves stats of the best run for each generated set of points.
/// </summary>
/// <param name="min_points">Number of points of the smallest test case.</param>
/// <param name="max_points">Number of points of the largest test case.</param>
/// <returns>One stats_t object for each test, representing statistics which 
/// resulted in smallest parallel execution time.</returns>
std::vector<stats_t> run_best_cutoff_test(int min_points, int max_points);

/// <summary>
/// Writes vector of stats into a file.
/// </summary>
/// <param name="stats">Stats vector.</param>
/// <param name="filename">Filename.</param>
void output_stats(const std::vector<stats_t>& stats, std::string filename);