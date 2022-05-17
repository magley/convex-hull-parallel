#include "convex_hull.h"
#include <algorithm>

using namespace std;

pair<vector<Vec2>, vector<Vec2>> common::divide_by_median(const vector<Vec2>& points) {
	vector<Vec2> sorted_by_x = points;
	sort(sorted_by_x.begin(), sorted_by_x.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	const int mid = sorted_by_x.size() / 2;
	const vector<Vec2> left = vector<Vec2>(sorted_by_x.begin(), sorted_by_x.begin() + mid);
	const vector<Vec2> right = vector<Vec2>(sorted_by_x.begin() + mid, sorted_by_x.end());

	return make_pair(left, right);
}


bool common::merge_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r, int tangent_side) {
	int result = 0;

	int s = 0;
	s = side(left[l], right[r], left[(int)(left.size() + l + tangent_side) % (int)left.size()]);
	if (s * tangent_side >= EPSILON) {
		result++;
	}
	else {
		l = (int)(left.size() + l + tangent_side) % (int)left.size();
	}
	s = side(left[l], right[r], right[(int)(right.size() + r - tangent_side) % (int)right.size()]);
	if (s * tangent_side >= EPSILON) {
		result++;
	}
	else {
		r = (int)(right.size() + r - tangent_side) % (int)right.size();
	}

	return result == 2;
}
