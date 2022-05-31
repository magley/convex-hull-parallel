#pragma once

#include "utility/mathutil.h"

namespace common {
	/// <summary>
	/// Brute force convex hull algorithm O(n^3).
	/// </summary>
	/// <param name="points">Input points</param>
	/// <returns>Convex hull, sorted by polar coordinates</returns>
	std::vector<Vec2> convex_hull_naive(const std::vector<Vec2>& points);
	/// <summary>
	/// Sortes a set of points by polar coordinates against a provided reference point.
	/// </summary>
	/// <param name="points">List of points</param>
	/// <param name="reference">Reference point, if unsure use get_center()</param>
	void sort_by_polar_coords(std::vector<Vec2>& points, Vec2 reference);
	/// <summary>
	/// pre-condition: points are sorted by their x-coordinate in an ascending order.
	/// Splits the list into two equal halves by their x-coordinate.
	/// </summary>
	/// <param name="points">List of points</param>
	/// <returns>A pair of vectors, the first being set of left points, 
	/// and the second being the right set of points</returns>
	std::pair<std::vector<Vec2>, std::vector<Vec2>> divide_by_median(const std::vector<Vec2>& points);
	/// <summary>
	/// pre-condition: left and right are disjoint convex sets.
	/// pre-condition: left and right are sorted by polar coordinates.
	/// 
	/// Performs a single step of the convex hull merge algorithm.
	/// </summary>
	/// <param name="left">Set of left points</param>
	/// <param name="right">Set of right points</param>
	/// <param name="l">[ref] Index of current left bound for the tangent</param>
	/// <param name="r">[ref] Index of current right bound for the tangent</param>
	/// <param name="tangent_side">1 if top tangent, -1 if bottom tangent</param>
	/// <returns>true if tangent is found, false otherwise</returns>
	bool merge_step(std::vector<Vec2>& left, std::vector<Vec2>& right, int& l, int& r, int tangent_side);
	/// <summary>
	/// Returns the center of the given points.
	/// </summary>
	Vec2 get_center(std::vector<Vec2>& points);
}

namespace serial {
	/// <summary>
	/// pre-condition: left and right are disjoint convex sets.
	/// pre-condition: left and right are sorted by polar coordinates.
	/// 
	/// Performs the merge algorithm.
	/// </summary>
	/// <param name="left">Set of left points</param>
	/// <param name="right">Set of right points</param>
	/// <returns>Merged hull</returns>
	std::vector<Vec2> merge_convex(std::vector<Vec2>& left, std::vector<Vec2>& right);
	/// <summary>
	/// Computes the convex hull using divide and conquer.
	/// </summary>
	/// <param name="points">Input set</param>
	/// <param name="cutoff">Cutoff for the recursion</param>
	/// <returns>Convex hull of the input set</returns>
	std::vector<Vec2> convex_hull(const std::vector<Vec2>& points, int cutoff);
}

namespace parallel {
	/// <summary>
	/// pre-condition: left and right are disjoint convex sets.
	/// pre-condition: left and right are sorted by polar coordinates.
	/// 
	/// Performs the merge algorithm.
	/// </summary>
	/// <param name="left">Set of left points</param>
	/// <param name="right">Set of right points</param>
	/// <returns>Merged hull</returns>
	std::vector<Vec2> merge_convex(std::vector<Vec2>& left, std::vector<Vec2>& right);
	/// <summary>
	/// Computes the convex hull using divide and conquer.
	/// </summary>
	/// <param name="points">Input set</param>
	/// <param name="cutoff">Cutoff for the recursion</param>
	/// <returns>Convex hull of the input set</returns>
	std::vector<Vec2> convex_hull(const std::vector<Vec2>& points, int cutoff);
}