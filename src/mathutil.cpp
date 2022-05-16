#include "mathutil.h"
#include <cmath>
using namespace std;

float Vec2::get_angle_between(Vec2 p) const {
	return atan2(double(p.y - this->y), double(p.x - this->x));
}

bool operator ==(const Vec2& A, const Vec2& B) {
	return A.x == B.x && A.y == B.y;
}

bool operator <(const Vec2& A, const Vec2& B) {
	if (A.x != B.x) {
		return A.x < B.x;
	}
	return A.y < B.y;
}

int random_range(int mini, int maxi) {
	return rand() % (maxi + 1 - mini) + mini;
}

int find_leftmost(const vector<Vec2>& arr) {
	int k = 0;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i].x < arr[k].x) {
			k = i;
		}
	}
	return k;
}

int find_rightmost(const vector<Vec2>& arr) {
	int k = 0;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i].x > arr[k].x) {
			k = i;
		}
	}
	return k;
}

float side(Vec2 A, Vec2 B, Vec2 P) {
	return ((B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x));
}