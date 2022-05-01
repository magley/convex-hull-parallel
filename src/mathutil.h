#pragma once
#include <vector>

struct Vec2 {
	double x = 0, y = 0;
	Vec2() { ; }
	Vec2(int x, int y) : x(x), y(y) { ; }
	float get_angle_between(Vec2 p) const;
};

bool operator ==(const Vec2& A, const Vec2& B);
int random_range(int mini, int maxi);
int find_topmost(const std::vector<Vec2>& arr);
int find_bottommost(const std::vector<Vec2>& arr);
int find_leftmost(const std::vector<Vec2>& arr);
int find_rightmost(const std::vector<Vec2>& arr);
bool on_left_side(Vec2 A, Vec2 B, Vec2 P);