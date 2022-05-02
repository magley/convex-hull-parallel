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

// counter-clockwise 
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


vector<Vec2> convex_hull_bruteforce(const vector<Vec2>& points) {
	/* The brute force algorithm works by checking
	 * whether each pair of 2 points forms a line
	 * segment which is also an edge of the hull.
	 * Two points form an edge if all other points
	 * from the array are on the same side of the
	 * line segment.
	 *
	 * Running time: O(n^3)
	 */

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

static std::vector<Vec2> merge(const std::vector<Vec2>& hullA, const std::vector<Vec2>& hullB);

std::pair<std::vector<Vec2>, std::vector<Vec2>> divide_by_median(const std::vector<Vec2>& points) {
	vector<Vec2> sorted_by_x = points;
	sort(sorted_by_x.begin(), sorted_by_x.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	const int mid = sorted_by_x.size() / 2;
	const vector<Vec2> left = vector<Vec2>(sorted_by_x.begin(), sorted_by_x.begin() + mid);
	const vector<Vec2> right = vector<Vec2>(sorted_by_x.begin() + mid, sorted_by_x.end());

	return std::make_pair(left, right);
}

bool tangent_top_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r) {
	int result = 0;

	int s = 0;
	s = side(left[l], right[r], left[(l + 1) % left.size()]);
	if (s >= 0) {
		result++;
	}
	else {
		l = (l + 1) % left.size();
	}
	s = side(left[l], right[r], right[(r - 1) % right.size()]);
	if (s >= 0) {
		result++;
	}
	else {
		r = (r - 1) % right.size();
	}

	return result == 2;
}

bool tangent_bottom_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r) {
	int result = 0;

	int s = 0;
	s = side(right[r], left[l], left[(l - 1) % left.size()]);
	if (s >= 0) {
		result++;
	}
	else {
		l = (l - 1) % left.size();
	}
	s = side(left[l], right[r], right[(r + 1) % right.size()]);
	if (s <= 0) {
		result++;
	}
	else {
		r = (r + 1) % right.size();
	}

	return result == 2;
}

std::vector<Vec2> convex_hull_divide_and_conquer(const std::vector<Vec2>& points) {
	// Base

	if (points.size() <= 6) {
		return convex_hull_bruteforce(points);
	}

	// Divide

	auto divided = divide_by_median(points);
	auto left = divided.first;
	auto right = divided.second;

	// Conquer

	left = convex_hull_divide_and_conquer(left);
	right = convex_hull_divide_and_conquer(right);
	sort_by_polar_coords(left);
	sort_by_polar_coords(right);

	// Tangents

	const int l = find_rightmost(left);
	const int r = find_leftmost(right);

	int top_l = l;
	int top_r = r;
	int bot_l = l;
	int bot_r = r;

	while (true) {
		if (tangent_top_step(left, right, top_l, top_r))
			break;
	}

	while (true) {
		if (tangent_bottom_step(left, right, bot_l, bot_r))
			break;
	}

	// Combine into final hull

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

/*
std::vector<Vec2> convex_hull_divide_and_conquer(const std::vector<Vec2>& points) {
	if (points.size() <= 6) {
		return convex_hull_bruteforce(points);
	}

	// Divide

	auto divided = divide_by_median(points);

	// Conquer

	vector<Vec2> hull_left = convex_hull_divide_and_conquer(divided.first);
	vector<Vec2> hull_right = convex_hull_divide_and_conquer(divided.second);

	sort_by_polar_coords(hull_left);
	sort_by_polar_coords(hull_right);

	// Merge

	return merge(hull_left, hull_right);
}

struct tangent_data {
	int index_A, index_B;
};

static tangent_data tangent_top(const std::vector<Vec2>& hullA, const std::vector<Vec2>& hullB) {
	int A_right = find_rightmost(hullA);
	int B_left = find_leftmost(hullB);

	bool done = false;
	while (!done) {
		done = true;

		while (side(hullB[B_left], hullA[A_right], hullA[(A_right + 1) % hullA.size()]) >= 0)
			A_right = (A_right + 1) % hullA.size();
		while (side(hullA[A_right], hullB[B_left], hullB[(B_left - 1) % hullB.size()]) <= 0) {
			B_left = (B_left - 1) % hullB.size();
			done = false;
		}
	}

	return tangent_data{ A_right, B_left };
}

static tangent_data tangent_bot(const std::vector<Vec2>& hullA, const std::vector<Vec2>& hullB) {
	int A_right = find_rightmost(hullA);
	int B_left = find_leftmost(hullB);

	bool done = false;
	while (!done) {
		done = true;

		while (side(hullA[A_right], hullB[B_left], hullB[(B_left + 1) % hullB.size()]) <= 0)
			B_left = (B_left + 1) % hullB.size();
		while (side(hullB[B_left], hullA[A_right], hullA[(A_right - 1) % hullA.size()]) >= 0) {
			A_right = (A_right - 1) % hullA.size();
			done = false;
		}
	}

	return tangent_data{ A_right, B_left };
}

static std::vector<Vec2> merge(const std::vector<Vec2>& hullA, const std::vector<Vec2>& hullB) {
	int top_A_index, top_B_index, bot_A_index, bot_B_index;
	int top_dir, bot_dir;
	{
		tangent_data d = tangent_top(hullA, hullB);
		top_A_index = d.index_A;
		top_B_index = d.index_B;
	}
	{
		tangent_data d = tangent_bot(hullA, hullB);
		bot_A_index = d.index_A;
		bot_B_index = d.index_B;
	}

	//return vector<Vec2> {
	//	hullA[bot_A_index], hullB[bot_B_index],
	//};

	vector<Vec2> result;

	for (int i = top_A_index; i != bot_A_index; i = (i + 1) % hullA.size()) {
		result.push_back(hullA[i]);
	}
	for (int i = top_B_index; i != bot_B_index; i = (i - 1) % hullB.size()) {
		result.push_back(hullB[i]);
	}
	
	return result;
}*/

/*

bool merge_step(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r) {
	int result = 0;

	int s = 0;
	s = side(left[l], right[r], left[(l + 1) % left.size()]);
	if (s >= 0) {
		result++;
	}
	else {
		l = (l + 1) % left.size();
	}
	s = side(left[l], right[r], right[(r - 1) % right.size()]);
	if (s >= 0) {
		result++;
	}
	else {
		r = (r - 1) % right.size();
	}

	return result == 2;
}

bool merge_step2(vector<Vec2>& left, vector<Vec2>& right, int& l, int& r) {
	int result = 0;

	int s = 0;
	s = side(left[l], right[l], left[(l - 1) % left.size()]);
	if (s <= 0) {
		result++;
	}
	else {
		l = (l - 1) % left.size();
	}
	s = side(left[l], right[r], right[(r + 1) % right.size()]);
	if (s <= 0) {
		result++;
	}
	else {
		r = (r + 1) % right.size();
	}

	return result == 2;
}


*/