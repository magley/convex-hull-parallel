#include "test.h"

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cassert>

#include "tbb/tick_count.h"
#include "convex_hull.h"
#include "rendutil.h"

using namespace std;
using namespace tbb;



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

vector<Vec2> perform_test(const vector<Vec2>& points) {
	vector<Vec2> hull_parallel;
	vector<Vec2> hull;
	vector<stats_t> stats;

	for (int i = points.size() * 0.05; i < points.size() * 0.5; i += points.size() / 10) {
		cout << "============= " << i << " ==========\n";

		tick_count time_parallel_start = tick_count::now();
		hull_parallel = get_hull_parallel(points, i);
		tick_count time_parallel_end = tick_count::now();
		double t_parallel = (time_parallel_end - time_parallel_start).seconds();
		cout << "Parallel: " << t_parallel << "s\n";

		tick_count time_serial_start = tick_count::now();
		hull = get_hull(points, i);
		tick_count time_serial_end = tick_count::now();
		double t_serial = (time_serial_end - time_serial_start).seconds();
		cout << "Serial:   " << t_serial << "s\n";

		double speedup = t_serial / t_parallel;
		cout << "Speedup: " << speedup << "\n";

		stats.push_back(stats_t(i, t_serial, t_parallel));
	}
	{
		cout << "========================\n";

		sort(stats.begin(), stats.end(), [](const stats_t& s1, const stats_t& s2) {
			return s1.time_parallel < s2.time_parallel;
			});

		stats_t best_time = stats[0];
		cout << "best_time:\n";
		best_time.print_me();

		sort(stats.begin(), stats.end(), [](const stats_t& s1, const stats_t& s2) {
			return s1.speedup > s2.speedup;
			});

		stats_t best_speedup = stats[0];
		cout << "best_speedup:\n";
		best_speedup.print_me();
	}

	assert(is_permutation(hull.cbegin(), hull.cend(), hull_parallel.cbegin()));

	return hull_parallel;
}