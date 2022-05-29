#include "convex_hull.h"
#include "tbb/tbb.h"
#include "tbb/task_group.h"
#include "tbb/parallel_sort.h"

using namespace std;
using namespace tbb;

void parallel::sort_by_polar_coords(vector<Vec2>& points, Vec2 reference) {
	parallel_sort(points.begin(), points.end(), [reference](const Vec2& A, const Vec2& B) {
		return A.get_angle_between(reference) > B.get_angle_between(reference);
	});
}

vector<Vec2> parallel::merge_convex(vector<Vec2>& left, vector<Vec2>& right) {
	const int l = find_rightmost(left);
	const int r = find_leftmost(right);
	int top_l = l;
	int top_r = r;
	int bot_l = l;
	int bot_r = r;

	task_group grp;

	grp.run([&]() {
		while (true) {
			if (common::merge_step(left, right, top_l, top_r, 1))
				break;
		}
	});
	grp.run([&]() {
		while (true) {
			if (common::merge_step(left, right, bot_l, bot_r, -1))
				break;
		}
	});
	grp.wait();

	vector<Vec2> result;

	for (int i = top_l; ; i = (i + 1) % left.size()) {
		result.push_back(left[i]);
		if (i == bot_l)
			break;
	}
	for (int i = bot_r; ; i = (i + 1) % right.size()) {
		result.push_back(right[i]);
		if (i == top_r)
			break;
	}

	return result;
}

static vector<Vec2> convex_hull_recursion(const vector<Vec2>& points, int cutoff) {
	if (cutoff < 8)
		cutoff = 8;
	if (points.size() <= cutoff) {
		return serial::convex_hull_naive(points);
	}

	pair<vector<Vec2>, vector<Vec2>> divided = common::divide_by_median(points);
	vector<Vec2> points_left = divided.first;
	vector<Vec2> points_right = divided.second;

	task_group g;
	vector<Vec2> left, right;

	g.run([&]() { left = convex_hull_recursion(points_left, cutoff); });
	g.run([&]() { right = convex_hull_recursion(points_right, cutoff); });

	g.wait();

	return parallel::merge_convex(left, right);
}

vector<Vec2> parallel::convex_hull(const vector<Vec2>& points, int cutoff) {
	auto points_sorted = points;
	parallel_sort(points_sorted.begin(), points_sorted.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	return convex_hull_recursion(points_sorted, cutoff);
}