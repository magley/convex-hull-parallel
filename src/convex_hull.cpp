#include "convex_hull.h"
#include <algorithm>

using namespace std;

static bool same_side(Vec2 A, Vec2 B, const vector<Vec2>& points) {
	bool should_be = on_left_side(A, B, points[0]);
	for (int i = 1; i < points.size(); i++) {
		if (points[i] == A || points[i] == B)
			continue;

		if (on_left_side(A, B, points[i]) != should_be)
			return false;
	}
	return true;
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
		return A.get_angle_between(reference) < B.get_angle_between(reference);
	});
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

	vector<Vec2> result;

	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (same_side(points[i], points[j], points)) {
				result.push_back(points[i]);
				result.push_back(points[j]);
			}
		}
	}
	return result;
}

std::vector<Vec2> convex_hull_divide_and_conquer(const std::vector<Vec2>& points) {
	return points;
}