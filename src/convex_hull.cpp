#include "convex_hull.h"
#include <algorithm>
#include <set>

using namespace std;

static bool same_side(Vec2 A, Vec2 B, const vector<Vec2>& points) {
	bool should_be = side(A, B, points[0]) > 0;
	for (int i = 1; i < points.size(); i++) {
		if (points[i] == A || points[i] == B)
			continue;

		if ((side(A, B, points[i]) > 0) != should_be)
			return false;
	}
	return true;
}

template<typename T>
static size_t RemoveDuplicatesKeepOrder(std::vector<T>& vec)
{
	std::set<T> seen;

	auto newEnd = std::remove_if(vec.begin(), vec.end(), [&seen](const T& value)
		{
			if (seen.find(value) != std::end(seen))
				return true;

			seen.insert(value);
			return false;
		});

	vec.erase(newEnd, vec.end());

	return vec.size();
}

static bool merge_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r, int tangent_side) {
	int result = 0;

	int s = 0;
	s = side(left[l], right[r], left[(int)(left.size() + l + tangent_side) % (int)left.size()]);
	if (s * tangent_side >= 0) {
		result++;
	}
	else {
		l = (int)(left.size() + l + tangent_side) % (int)left.size();
	}
	s = side(left[l], right[r], right[(int)(right.size() + r - tangent_side) % (int)right.size()]);
	if (s * tangent_side >= 0) {
		result++;
	}
	else {
		r = (int)(right.size() + r - tangent_side) % (int)right.size();
	}

	return result == 2;
}

static std::vector<Vec2> merge_convex(std::vector<Vec2>& left, std::vector<Vec2>& right) {
	const int l = find_rightmost(left);
	const int r = find_leftmost(right);
	int top_l = l;
	int top_r = r;
	int bot_l = l;
	int bot_r = r;

	while (true) {
		int ok = 2;

		if (merge_step(left, right, top_l, top_r, 1))
			ok--;

		if (merge_step(left, right, bot_l, bot_r, -1))
			ok--;

		if (ok == 0)
			break;
	}

	std::vector<Vec2> result;

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

static std::pair<std::vector<Vec2>, std::vector<Vec2>> divide_by_median(const std::vector<Vec2>& points) {
	vector<Vec2> sorted_by_x = points;
	sort(sorted_by_x.begin(), sorted_by_x.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	const int mid = sorted_by_x.size() / 2;
	const vector<Vec2> left = vector<Vec2>(sorted_by_x.begin(), sorted_by_x.begin() + mid);
	const vector<Vec2> right = vector<Vec2>(sorted_by_x.begin() + mid, sorted_by_x.end());

	return std::make_pair(left, right);
}

void sort_by_polar_coords(std::vector<Vec2>& points) {
	Vec2 reference = Vec2(0, 0);
	for (int i = 0; i < points.size(); i++) {
		reference.x += points[i].x;
		reference.y += points[i].y;
	}
	reference.x /= points.size();
	reference.y /= points.size();

	sort(points.begin(), points.end(), [reference](const Vec2& A, const Vec2& B) {
		return A.get_angle_between(reference) > B.get_angle_between(reference);
		});
}

vector<Vec2> convex_hull_bruteforce(const vector<Vec2>& points) {
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

	RemoveDuplicatesKeepOrder(result);

	return result;
}

vector<Vec2> convex_hull_divide_and_conquer(const vector<Vec2>& points) {
	// Base

	if (points.size() <= 6) {
		return convex_hull_bruteforce(points);
	}

	// Divide

	pair<vector<Vec2>, vector<Vec2>> divided = divide_by_median(points);
	vector<Vec2> points_left = divided.first;
	vector<Vec2> points_right = divided.second;

	// Conquer

	vector<Vec2> left = convex_hull_divide_and_conquer(points_left);
	vector<Vec2> right = convex_hull_divide_and_conquer(points_right);
	sort_by_polar_coords(left);
	sort_by_polar_coords(right);

	// Merge

	return merge_convex(left, right);
}

#include "tbb/task_group.h"
using namespace tbb;

std::vector<Vec2> convex_hull_divide_and_conquer_parallel(const vector<Vec2>& points) {
	// Base

	if (points.size() <= 6) {
		return convex_hull_bruteforce(points);
	}

	// Divide

	pair<vector<Vec2>, vector<Vec2>> divided = divide_by_median(points);
	vector<Vec2> points_left = divided.first;
	vector<Vec2> points_right = divided.second;

	// Conquer

	task_group g;
	vector<Vec2> left, right;

	g.run([&]() {
		left = convex_hull_divide_and_conquer_parallel(points_left);
		sort_by_polar_coords(left);
		});
	g.run([&]() {
		right = convex_hull_divide_and_conquer_parallel(points_right);
		sort_by_polar_coords(right);
		});

	g.wait();

	// Merge

	return merge_convex(left, right);
}