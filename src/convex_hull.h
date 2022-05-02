#pragma once

#include "mathutil.h"
#include <utility>

std::pair<std::vector<Vec2>, std::vector<Vec2>> divide_by_median(const std::vector<Vec2>& points);
std::vector<Vec2> convex_hull_bruteforce(const std::vector<Vec2>& points);
void sort_by_polar_coords(std::vector<Vec2>& points);
std::vector<Vec2> convex_hull_divide_and_conquer(const std::vector<Vec2>& points);