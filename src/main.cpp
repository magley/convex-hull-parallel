#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"

#include "mathutil.h"
#include "convex_hull.h"

using namespace std;
using namespace tbb;

void draw_points(SDL_Renderer* rend, const vector<Vec2>& points, SDL_Color col, Vec2 size) {
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
	for (int i = 0; i < points.size(); i++) {
		Vec2 point = points[i];
		SDL_FRect r{ point.x - size.x / 2, point.y - size.y / 2, size.x, size.y };
		SDL_RenderFillRectF(rend, &r);
	}
}

void draw_polygon(SDL_Renderer* rend, const vector<Vec2>& points, SDL_Color col) {
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
	for (int i = 0; i < points.size(); i++) {
		Vec2 A = points[i];
		Vec2 B = points[(i + 1) % points.size()];
		SDL_RenderDrawLineF(rend, A.x, A.y, B.x, B.y);
	}
}

void draw_line(SDL_Renderer* rend, Vec2 A, Vec2 B, SDL_Color col) {
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
	SDL_RenderDrawLineF(rend, A.x, A.y, B.x, B.y);
}

vector<Vec2> generate_points() {
	vector<Vec2> points;

	for (int i = 0; i < 15; i++) {
		Vec2 p;
		const int margin = 100;
		p.x = random_range(margin, 480 - margin);
		p.y = random_range(margin, 480 - margin);
		points.push_back(p);
	}
	return points;
}

vector<Vec2> get_hull(const vector<Vec2>& points) {
	vector<Vec2> hull = convex_hull_divide_and_conquer(points);
	sort_by_polar_coords(hull);
	return hull;
}


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

	s = side(right[r], left[l], left[(l - 1) % left.size()]);
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

enum {
	PHASE_POINTS,
	PHASE_DIVIDE,
	PHASE_CONQUER,
	PHASE_TANGENT,
	PHASE_MERGE
};


int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* win = SDL_CreateWindow(
		"convex hull",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		480,
		480,
		SDL_WINDOW_OPENGL
	);
	SDL_Renderer* rend = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	SDL_Texture* tex_test = IMG_LoadTexture(rend, "res\\img_test.png");
	SDL_Event ev;
	bool running = true;
	int t = 16171727172; //time(NULL); 
	srand(t);

	int phase = PHASE_POINTS;

	vector<Vec2> points = generate_points();

	// Divide
	
	pair<vector<Vec2>, vector<Vec2>> divided;

	vector<Vec2> points_left;
	vector<Vec2> points_right;

	vector<Vec2> left;
	vector<Vec2> right;

	// Right-left

	int l;
	int r;

	// Tangents

	int top_l;
	int top_r;
	int bot_l;
	int bot_r;
	
	// Combine into final hull

	vector<Vec2> result;

	int i = 0;
	int time = 30;

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		i++;

		if (i % time == 0) {
			if (phase == PHASE_POINTS) {
				divided = divide_by_median(points);
				points_left = divided.first;
				points_right = divided.second;
				phase++;
			}
			else if (phase == PHASE_DIVIDE) {
				left = convex_hull_bruteforce(points_left);
				right = convex_hull_bruteforce(points_right);
				sort_by_polar_coords(left);
				sort_by_polar_coords(right);
				phase++;
			}
			else if (phase == PHASE_CONQUER) {
				l = find_rightmost(left);
				r = find_leftmost(right);

				top_l = l;
				top_r = r;
				bot_l = l;
				bot_r = r;

				phase++;
			}
			else if (phase == PHASE_TANGENT) {
				int ok = 2;

				if (merge_step(left, right, top_l, top_r))
					ok--;

				if (merge_step2(left, right, bot_l, bot_r))
					ok--;

				if (ok == 0) {
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

					phase++;
				}
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		if (phase == PHASE_POINTS) {
			draw_points(rend, points, { 100, 100, 100, 200 }, Vec2(4, 4));
		}
		else if (phase == PHASE_DIVIDE) {
			draw_points(rend, points, { 100, 100, 100, 100 }, Vec2(2, 2));
			draw_points(rend, points_left, { 255, 100, 100, 255 }, Vec2(4, 4));
			draw_points(rend, points_right, { 100, 255, 100, 255 }, Vec2(4, 4));
		}
		else if (phase == PHASE_CONQUER) {
			draw_points(rend, left, { 255, 100, 100, 255 }, Vec2(4, 4));
			draw_polygon(rend, left, { 255, 100, 100, 128 });
			draw_points(rend, right, { 100, 255, 100, 255 }, Vec2(4, 4));
			draw_polygon(rend, right, { 100, 255, 100, 128 });
		}
		else if (phase == PHASE_TANGENT) {
			draw_points(rend, left, { 255, 100, 100, 255 }, Vec2(4, 4));
			draw_polygon(rend, left, { 255, 100, 100, 128 });
			draw_points(rend, right, { 100, 255, 100, 255 }, Vec2(4, 4));
			draw_polygon(rend, right, { 100, 255, 100, 128 });

			draw_line(rend, left[top_l], right[top_r], { 255, 255, 0, 255 });
			draw_line(rend, left[bot_l], right[bot_r], { 0, 255, 255, 255 });
		}
		else if (phase == PHASE_MERGE) {
			draw_points(rend, result, { 255, 255, 255, 255 }, Vec2(4, 4));
			draw_polygon(rend, result, { 255, 255, 255, 255 });
		}

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_test);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}