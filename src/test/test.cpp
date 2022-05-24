#include "test.h"

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cassert>

#include "tbb/tick_count.h"
#include "convexhull/convex_hull.h"
#include "render/rendutil.h"

using namespace std;
using namespace tbb;

std::vector<stats_t> run_tests(const std::vector<Vec2>& points, std::vector<Vec2>& out_points) {
	std::vector<stats_t> statistics;

	int cutoff_min, cutoff_max, cutoff_step;

	cutoff_min = 6;
	cutoff_max = (int)std::max(10.0, points.size() * 0.1);

	if (cutoff_max < cutoff_min)
		cutoff_max = cutoff_min;
	if (cutoff_min > points.size()) {
		cutoff_min = points.size();
	}
	if (cutoff_max > points.size()) {
		cutoff_max = points.size();
	}

	cutoff_step = 1;

	cout << "Testing between " << cutoff_min << " and " << cutoff_max << "\n";

	for (int i = cutoff_min; i <= cutoff_max; i += cutoff_step) {
		statistics.push_back(run_test(points, out_points, i));
	}

	return statistics;
}

stats_t run_test(const std::vector<Vec2>& points, std::vector<Vec2>& out_points, int cutoff) {
	cout << "Testing " << cutoff << "...\n";
	tick_count time_parallel_start = tick_count::now();
	out_points = get_hull_parallel(points, cutoff);
	tick_count time_parallel_end = tick_count::now();
	double t_parallel = (time_parallel_end - time_parallel_start).seconds();

	tick_count time_serial_start = tick_count::now();
	get_hull(points, cutoff);
	tick_count time_serial_end = tick_count::now();
	double t_serial = (time_serial_end - time_serial_start).seconds();

	double speedup = t_serial / t_parallel;

	return stats_t(points.size(), cutoff, t_serial, t_parallel);
}

vector<Vec2> generate_points(string filename) {
	ifstream f(filename);
	vector<Vec2> points;
	double x, y;
	while (f >> x >> y) {
		points.push_back(Vec2(x, y));
	}

	return points;
}

void output_points(const vector<Vec2>& points, string filename) {
	ofstream f(filename);
	f.clear();

	for (const Vec2& v : points) {
		f << v.x << " " << v.y << "\n";
	}
}

vector<Vec2> generate_points(int num_of_points) {
	vector<Vec2> points;

	for (int i = 0; i < num_of_points; i++) {
		Vec2 p;
		double pctg = (double)i / num_of_points;
		const int margin = 200 * pctg;
		p.x = random_range(margin, WIN_SIZE - margin);
		p.y = random_range(margin, WIN_SIZE - margin);
		points.push_back(p);
	}

	return points;
}

vector<Vec2> get_hull(const vector<Vec2>& points, int cutoff) {
	vector<Vec2> hull = serial::convex_hull(points, cutoff);
	serial::sort_by_polar_coords(hull);
	return hull;
}

vector<Vec2> get_hull_parallel(const vector<Vec2>& points, int cutoff) {
	vector<Vec2> hull = parallel::convex_hull(points, cutoff);
	parallel::sort_by_polar_coords(hull);
	return hull;
}