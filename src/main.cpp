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

	vector<Vec2> points = generate_points(input_filename);
	vector<Vec2> hull;
	stats_t stats = run_test(points, hull, 58);

	// ====================================================================================

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_points(rend, points, { 255, 255, 255, 128 }, Vec2(4, 4));
		draw_points(rend, hull, { 255, 255, 255, 255 }, Vec2(4, 4));
		draw_polygon(rend, hull, { 0, 255, 255, 255 });

		draw_text(rend, tex_font, stats, Vec2(0, 0));

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_font);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;

}