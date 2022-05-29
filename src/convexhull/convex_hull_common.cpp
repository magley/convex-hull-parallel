#include "convex_hull.h"
#include <algorithm>

using namespace std;

pair<vector<Vec2>, vector<Vec2>> common::divide_by_median(const vector<Vec2>& points) {
	// pre-condition: points are sorted by x.
	vector<Vec2> sorted_by_x = points;
	const int mid = sorted_by_x.size() / 2;
	const vector<Vec2> left = vector<Vec2>(sorted_by_x.begin(), sorted_by_x.begin() + mid);
	const vector<Vec2> right = vector<Vec2>(sorted_by_x.begin() + mid, sorted_by_x.end());

	return make_pair(left, right);
}

bool common::merge_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r, int tangent_side) {
	// pre-condition: left and right are sorted by polar coords.
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
