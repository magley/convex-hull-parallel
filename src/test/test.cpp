#include "test.h"

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cassert>
#include <set>

#include "tbb/tick_count.h"
#include "convexhull/convex_hull.h"
#include "render/rendutil.h"

using namespace std;
using namespace tbb;

static bool verbose = true;

template <class T>
static void print(T t) {
	if (verbose) {
		cout << t;
	}
}

template <class T, class... Args>
static void print(T t, Args... args) {
	if (verbose) {
		cout << t;
		print(args...);
	}
}

//*****************************************************************************
// points
//*****************************************************************************

vector<Vec2> generate_points(string filename) {
	ifstream f(filename);
	vector<Vec2> points;
	double x, y;
	while (f >> x >> y) {
		points.push_back(Vec2(x, y));
	}

	return points;
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

void output_points(const vector<Vec2>& points, string filename) {
	ofstream f(filename);
	f.clear();

	for (const Vec2& v : points) {
		f << v.x << " " << v.y << "\n";
	}
}

//*****************************************************************************
// tests
//*****************************************************************************

struct Compare
{
	bool operator () (const Vec2& A, const Vec2& B) const  { if (A.x == B.x) return A.y < B.y; return A.x < B.x; }
};

stats_t run_test(const vector<Vec2>& input, vector<Vec2>& output, int cutoff) {
	print("input.size=", input.size(), ", cutoff= ", cutoff, " ");

	tick_count time_parallel_start = tick_count::now();
	output = parallel::convex_hull(input, cutoff);
	tick_count time_parallel_end = tick_count::now();
	print("[P");

	tick_count time_serial_start = tick_count::now();
	auto outSer = serial::convex_hull(input, cutoff);
	tick_count time_serial_end = tick_count::now();
	print("S]\n");

	set <Vec2, Compare> msP(output.begin(), output.end());
	set <Vec2, Compare> msS(outSer.begin(), outSer.end());
	assert(msP == msS);

	return stats_t(
		input.size(), 
		cutoff, 
		(time_serial_end - time_serial_start).seconds(),
		(time_parallel_end - time_parallel_start).seconds()
	);
}

vector<stats_t> run_speedup_test(int min_points, int max_points) {
	if (min_points < 3)
		min_points = 3;
	if (min_points > max_points)
		max_points = min_points;

	vector<stats_t> stats;
	
	for (int i = min_points; i <= max_points; i++) {
		vector<Vec2> _;
		vector<Vec2> input = generate_points(i);
		const int cutoff = min(50U, input.size() / 2);
		stats.push_back(stats_t(run_test(input, _, cutoff)));
	}

	return stats;
}

vector<stats_t> run_cutoff_test(const std::vector<Vec2>& input, std::vector<Vec2>& output) {
	vector<stats_t> statistics;

	int cutoff_min = 15;
	int cutoff_max = 100;
	if (cutoff_min > input.size()) {
		cutoff_min = input.size();
	}
	if (cutoff_max > input.size()) {
		cutoff_max = input.size();
	}

	for (int i = cutoff_min; i <= cutoff_max; i++) {
		print(i, "/", cutoff_max, " ");
		statistics.push_back(run_test(input, output, i));
	}

	return statistics;
}

vector<stats_t> run_best_cutoff_test(int min_points, int max_points) {
	vector<stats_t> best_cutoff;
	vector<Vec2> _;

	for (int n = min_points; n <= max_points; n++) {
		vector<stats_t> cutoff_for_n = run_cutoff_test(generate_points(n), _);

		int best_i = 0;
		for (int i = 0; i < cutoff_for_n.size(); i += 10) {
			if (cutoff_for_n[i].time_parallel < cutoff_for_n[best_i].time_parallel){
				best_i = i;
			}
		}

		best_cutoff.push_back(cutoff_for_n[best_i]);
	}

	return best_cutoff;
}

//*****************************************************************************
// output statistics
//*****************************************************************************

void output_stats(const std::vector<stats_t>& stats, std::string filename) {
	ofstream o(filename);
	for (auto& s : stats) {
		o << s.point_count << " " << s.cutoff << " " << s.time_serial << " " << s.time_parallel << " " << s.speedup << "\n";
	}
}