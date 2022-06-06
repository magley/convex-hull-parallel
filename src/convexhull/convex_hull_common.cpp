#include "convex_hull.h"
#include <algorithm>
#include <set>
#include <queue>
#include <iterator>

using namespace std;

pair<vector<Vec2>, vector<Vec2>> common::divide_by_median(const vector<Vec2>& points) {
	vector<Vec2> sorted_by_x = points;
	const int mid = sorted_by_x.size() / 2;
	const vector<Vec2> left = vector<Vec2>(sorted_by_x.begin(), sorted_by_x.begin() + mid);
	const vector<Vec2> right = vector<Vec2>(sorted_by_x.begin() + mid, sorted_by_x.end());

	return make_pair(left, right);
}

bool common::merge_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r, int tangent_side) {
	bool okA = false, okB = false;
	float s = 0;

	s = side(left[l], right[r], left[(int)(left.size() + l + tangent_side) % (int)left.size()]);
	if ((double)tangent_side * s >= EPSILON) {
		okA = true;
	}
	else {
		l = (int)(left.size() + l + tangent_side) % (int)left.size();
	}

	s = side(left[l], right[r], right[(int)(right.size() + r - tangent_side) % (int)right.size()]);
	if ((double)tangent_side * s >= EPSILON) {
		okB = true;
	}
	else {
		r = (int)(right.size() + r - tangent_side) % (int)right.size();
	}

	return okA && okB;
}

Vec2 common::get_center(std::vector<Vec2>& points) {
	Vec2 center = Vec2(0, 0);
	for (int i = 0; i < points.size(); i++) {
		center.x += points[i].x;
		center.y += points[i].y;
	}
	center.x /= points.size();
	center.y /= points.size();
	return center;
}


void common::sort_by_polar_coords(vector<Vec2>& points, Vec2 reference) {
	sort(points.begin(), points.end(), [reference](const Vec2& A, const Vec2& B) {
		return A.get_angle_between(reference) > B.get_angle_between(reference);
		});
}

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

vector<Vec2> common::convex_hull_naive(const vector<Vec2>& points) {
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
	common::sort_by_polar_coords(result, common::get_center(result));

	return result;
}


static Vec2 second_top(deque<Vec2>& stack) {
	Vec2 top = stack.back();
	stack.pop_back();
	Vec2 top2 = stack.back();
	stack.push_back(top);
	return top2;
}

vector<Vec2> common::graham_scan(const vector<Vec2>& points) {
	int p0 = 0;
	for (int i = 0; i < points.size(); i++) {
		if (points[i].y > points[p0].y) {
			p0 = i;
		}
		else if (points[i].y == points[p0].y && points[i].x < points[p0].x) {
			p0 = i;
		}
	}

	auto sorted = points;
	common::sort_by_polar_coords(sorted, points[p0]);

	deque<Vec2> stack;
	for (Vec2& p : sorted) {
		while (stack.size() > 1 && side(second_top(stack), stack.back(), p) >= 0) {
			stack.pop_back();
		}
		stack.push_back(p);
	}

	vector<Vec2> v;
	copy(stack.cbegin(), stack.cend(), back_inserter(v));
	return v;
}