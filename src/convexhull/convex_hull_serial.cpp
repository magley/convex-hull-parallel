#include "convex_hull.h"
#include <algorithm>
#include <set>
#include "tbb/tbb.h"
#include "tbb/task_group.h"
#include "tbb/parallel_sort.h"

using namespace tbb;
using namespace std;

static bool same_side(Vec2 A, Vec2 B, const vector<Vec2>& points) {
	bool should_be = side(A, B, points[0]) > EPSILON;
	for (int i = 1; i < points.size(); i++) {
		if (points[i] == A || points[i] == B)
			continue;

		if ((side(A, B, points[i]) > EPSILON) != should_be)
			return false;
	}
	return true;
}

template<typename T>
static size_t remove_duplicates(vector<T>& vec)
{
	set<T> seen;
	auto newEnd = remove_if(vec.begin(), vec.end(), [&seen](const T& value)
	{
		if (seen.find(value) != end(seen))
			return true;
		seen.insert(value);
		return false;
	});

	vec.erase(newEnd, vec.end());

	return vec.size();
}

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

void serial::sort_by_polar_coords(vector<Vec2>& points, Vec2 reference) {
	sort(points.begin(), points.end(), [reference](const Vec2& A, const Vec2& B) {
		return A.get_angle_between(reference) > B.get_angle_between(reference);
	});
}

vector<Vec2> serial::convex_hull_naive(const vector<Vec2>& points) {
	if (points.size() <= 3)
		return points;

	vector<Vec2> result;

	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (same_side(points[i], points[j], points)) {
				result.push_back(points[i]);
				result.push_back(points[j]);
			}
		}
	}

	remove_duplicates(result);
	serial::sort_by_polar_coords(result, common::get_center(result));

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

	vector<Vec2> left = convex_hull_recursion(points_left, cutoff);
	vector<Vec2> right = convex_hull_recursion(points_right, cutoff);

	return serial::merge_convex(left, right);
}

vector<Vec2> serial::convex_hull(const vector<Vec2>& points, int cutoff) {
	auto points_sorted = points;
	sort(points_sorted.begin(), points_sorted.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	return convex_hull_recursion(points_sorted, cutoff);
}