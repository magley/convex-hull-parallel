#pragma once

#include "mathutil.h"

bool same_side(Vec2 A, Vec2 B, const std::vector<Vec2>& points);
std::vector<Vec2> convex_hull_bruteforce(const std::vector<Vec2>& points, bool polar_sorted);