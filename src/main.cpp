#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/tick_count.h"

#include "mathutil.h"
#include "convex_hull.h"
#include "rendutil.h"

using namespace std;
using namespace tbb;

#define WIN_SIZE 800

vector<Vec2> generate_points() {
	vector<Vec2> points;

	for (int i = 0; i < 800; i++) {
		Vec2 p;
		const int margin = 75;
		p.x = random_range(margin, WIN_SIZE - margin);
		p.y = random_range(margin, WIN_SIZE - margin);
		points.push_back(p);
	}
	return points;
}

vector<Vec2> get_hull(const vector<Vec2>& points, int cutoff) {
	vector<Vec2> hull = convex_hull_divide_and_conquer(points, cutoff);
	sort_by_polar_coords(hull);
	return hull;
}

vector<Vec2> get_hull_parallel(const vector<Vec2>& points, int cutoff) {
	vector<Vec2> hull = convex_hull_divide_and_conquer_parallel(points, cutoff);
	sort_by_polar_coords(hull);
	return hull;
}

struct stats_t {
	int cutoff;
	double time_serial;
	double time_parallel;
	double speedup;

	stats_t() {}

	stats_t(int cutoff, double time_serial, double time_parallel) :
		cutoff(cutoff),
		time_serial(time_serial),
		time_parallel(time_parallel),
		speedup(time_serial / time_parallel)
	{}

	void print_me() const {
		cout
			<< "cutoff:  " << cutoff << "\n"
			<< "serial:  " << time_serial << "\n"
			<< "parallel:" << time_parallel << "\n"
			<< "speedup: " << speedup << "\n"
			<< "\n"
		;
	}
};

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* win = SDL_CreateWindow(
		"convex hull",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_SIZE,
		WIN_SIZE,
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
	int t = time(NULL);
	srand(t);

	vector<Vec2> points = generate_points();
	vector<Vec2> hull_parallel;
	vector<Vec2> hull;

	vector<stats_t> stats;

	for (int i = 10; i < 200; i += 1) {
		cout << "============= " << i << " ==========\n";

		tick_count time_parallel_start = tick_count::now();
		hull_parallel = get_hull_parallel(points, i);
		tick_count time_parallel_end = tick_count::now();
		double t_parallel = (time_parallel_end - time_parallel_start).seconds();
		cout << "Parallel: " << t_parallel << "s\n";


		tick_count time_serial_start = tick_count::now();
		hull = get_hull(points, i);
		tick_count time_serial_end = tick_count::now();
		double t_serial = (time_serial_end - time_serial_start).seconds();
		cout << "Serial:   " << t_serial << "s\n";

		double speedup = t_serial / t_parallel;
		cout << "Speedup: " << speedup << "\n";

		stats.push_back(stats_t(i, t_serial, t_parallel));
	}

	{
		cout << "========================\n";

		sort(stats.begin(), stats.end(), [](const stats_t& s1, const stats_t& s2) {
			return s1.time_parallel < s2.time_parallel;
		});

		stats_t best_time = stats[0];
		cout << "best_time:\n";
		best_time.print_me();

		sort(stats.begin(), stats.end(), [](const stats_t& s1, const stats_t& s2) {
			return s1.speedup > s2.speedup;
		});

		stats_t best_speedup = stats[0];
		cout << "best_speedup:\n";
		best_speedup.print_me();
	}

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_points(rend, points, { 255, 255, 255, 32 }, Vec2(4, 4));
		draw_points(rend, hull_parallel, { 255, 255, 255, 128 }, Vec2(4, 4));
		draw_polygon(rend, hull_parallel, { 0, 255, 255, 200 });

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_test);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}