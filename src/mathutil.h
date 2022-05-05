#pragma once
#include <vector>

#define EPSILON 0.0000001

struct Vec2 {
	double x = 0, y = 0;
	Vec2() { ; }
	Vec2(int x, int y) : x(x), y(y) { ; }
	float get_angle_between(Vec2 p) const;
};

bool operator ==(const Vec2& A, const Vec2& B);
bool operator <(const Vec2& A, const Vec2& B);
int random_range(int mini, int maxi);
int find_topmost(const std::vector<Vec2>& arr);
int find_bottommost(const std::vector<Vec2>& arr);
int find_leftmost(const std::vector<Vec2>& arr);
int find_rightmost(const std::vector<Vec2>& arr);
float side(Vec2 A, Vec2 B, Vec2 P);