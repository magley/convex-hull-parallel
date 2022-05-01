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

	srand(time(NULL));
	vector<Vec2> points;
	for (int i = 0; i < 100; i++) {
		Vec2 p;
		const int margin = 150;
		p.x = random_range(margin, 480 - margin);
		p.y = random_range(margin, 480 - margin);
		points.push_back(p);
	}

	vector<Vec2> hull = convex_hull_bruteforce(points, true);

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_points(rend, points, { 225, 225, 255, 64 }, Vec2(4, 4));
		draw_points(rend, hull, { 225, 225, 255, 255 }, Vec2(4, 4));
		draw_polygon(rend, hull, { 255, 0, 0, 255 });

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_test);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}