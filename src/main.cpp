#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <ctime>
#include "test/test.h"
#include "render/rendutil.h"
#include "convexhull/convex_hull.h"
#include <algorithm>

using namespace std;

string file_infix = "07";
const string infile_prefix = "res/input_file_";
const string outfile_prefix = "out/statistics_";
string input_filename = infile_prefix + file_infix + ".txt";
string output_filename = outfile_prefix + file_infix + ".txt";
string output_filename_speedup = "out/speedup_cutoff.txt";

int index_of_y(vector<Vec2> points, bool minimum) {
	int index = 0;
	for (int i = 0; i < points.size(); i++) {
		if (points[i].y < points[index].y && minimum)
			index = i;
		if (points[i].y > points[index].y && !minimum)
			index = i;
	}
	return index;
}

// pre-condition
// -	points form a convex polygon
// -	points is ordered by x
// post-condition
// -	return is ordered clockwise starting from leftmost point
//
// works in O(n)
//
// a[0]: UR
// a[1]: DR
// a[2]: UL
// a[3]: DL
vector<Vec2> polar_ordering_from_x_ordering(vector<Vec2> points) {
	int min_y_index = index_of_y(points, true);
	int max_y_index = index_of_y(points, false);
	vector<Vec2> a[4];
	Vec2 A = points[0];
	Vec2 Z = *points.rbegin();
	const int n = points.size();

	for (int i = 1; i < n; i++) {
		if (i == min_y_index)
			break;
		if (points[i].y <= A.y)
			a[0].push_back(points[i]);
	}

	for (int i = 1; i < n; i++) {
		if (i == max_y_index)
			break;
		if (points[i].y >= A.y)
			a[1].push_back(points[i]);
	}

	for (int i = n - 1; i >= 0; i--) {
		if (i == min_y_index)
			break;
		if (points[i].y <= Z.y)
			a[2].push_back(points[i]);
	}

	for (int i = n - 1; i >= 0; i--) {
		if (i == max_y_index)
			break;
		if (points[i].y >= Z.y)
			a[3].push_back(points[i]);
	}

	vector<Vec2> ring;

	reverse(a[2].begin(), a[2].end());
	reverse(a[1].begin(), a[1].end());

	ring.push_back(A);
	for (auto v : a[0]) ring.push_back(v);
	ring.push_back(points[min_y_index]);
	for (auto v : a[2]) ring.push_back(v);
	ring.push_back(Z);
	for (auto v : a[3]) ring.push_back(v);
	ring.push_back(points[max_y_index]);
	for (auto v : a[1]) ring.push_back(v);

	return ring;
}


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
	SDL_Texture* tex_font = IMG_LoadTexture(rend, "res\\font.png");
	SDL_Event ev;
	bool running = true;
	srand(time(NULL));


	// ====================================================================================

	//vector<Vec2> points = generate_points(input_filename);
	//vector<Vec2> hull;
	//stats_t stats = run_test(points, hull, 58);

	vector<Vec2> convex_sorted_by_x = generate_regular_points(100);//serial::convex_hull_naive(generate_points(1000));
	sort(convex_sorted_by_x.begin(), convex_sorted_by_x.end(), [](Vec2& p1, Vec2& p2) {return p1.x < p2.x; });
	vector<Vec2> polar_in_linear_time = polar_ordering_from_x_ordering(convex_sorted_by_x);

	// ====================================================================================

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_points(rend, convex_sorted_by_x, { 255, 255, 255, 255 }, Vec2(4, 4));
		draw_polygon(rend, polar_in_linear_time, { 0, 255, 255, 255 });
		draw_polygon(rend, convex_sorted_by_x, { 255, 0, 0, 128 });

		//draw_points(rend, points, { 255, 255, 255, 128 }, Vec2(4, 4));
		//draw_points(rend, hull, { 255, 255, 255, 255 }, Vec2(4, 4));
		//draw_polygon(rend, hull, { 0, 255, 255, 255 });
		//draw_text(rend, tex_font, stats, Vec2(0, 0));

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_font);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;

}