#include "convex_hull.h"
#include <algorithm>
#include <set>
#include "tbb/tbb.h"
#include "tbb/task_group.h"
#include "tbb/parallel_sort.h"

using namespace tbb;
using namespace std;

vector<Vec2> serial::merge_convex(vector<Vec2>& left, vector<Vec2>& right) {
	const int l = find_rightmost(left);
	const int r = find_leftmost(right);
	int top_l = l, top_r = r;
	int bot_l = l, bot_r = r;

	while (true) {
		bool foundTop = common::merge_step(left, right, top_l, top_r, 1);
		bool foundBot = common::merge_step(left, right, bot_l, bot_r, -1);
		if (foundTop && foundBot)
			break;
	}
	
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
		return common::convex_hull_naive(points);
	}

	pair<vector<Vec2>, vector<Vec2>> divided = common::divide_by_median(points);
	vector<Vec2> points_left = divided.first;
	vector<Vec2> points_right = divided.second;

	vector<Vec2> left = convex_hull_recursion(points_left, cutoff);
	vector<Vec2> right = convex_hull_recursion(points_right, cutoff);

	return serial::merge_convex(left, right);
}

vector<Vec2> serial::convex_hull(const vector<Vec2>& points, int cutoff) {
	auto points_sorted = points;
	sort(points_sorted.begin(), points_sorted.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	return convex_hull_recursion(points_sorted, cutoff);
}