#pragma once

#include "utility/mathutil.h"

namespace common {
	std::pair<std::vector<Vec2>, std::vector<Vec2>> divide_by_median(const std::vector<Vec2>& points);
	bool merge_step(std::vector<Vec2>& left, std::vector<Vec2>& right, int& l, int& r, int tangent_side);
	Vec2 get_center(std::vector<Vec2>& points);
}

namespace serial {
	std::vector<Vec2> convex_hull_naive(const std::vector<Vec2>& points);
	std::vector<Vec2> merge_convex(std::vector<Vec2>& left, std::vector<Vec2>& right);
	void sort_by_polar_coords(std::vector<Vec2>& points, Vec2 reference);
	std::vector<Vec2> convex_hull(const std::vector<Vec2>& points, int cutoff);
}

namespace parallel {
	std::vector<Vec2> merge_convex(std::vector<Vec2>& left, std::vector<Vec2>& right);
	void sort_by_polar_coords(std::vector<Vec2>& points, Vec2 reference);
	std::vector<Vec2> convex_hull(const std::vector<Vec2>& points, int cutoff);
}