#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <ctime>
#include "test/test.h"
#include "render/rendutil.h"

using namespace std;


string input_filename = "res/input_file_06.txt";
int run_analysis = 1;

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

	const vector<Vec2> points = generate_points(5000);
	vector<Vec2> hull;
	vector<stats_t> stats = run_tests(points, hull);

	Plot g(stats, 32);
	int mouse_x, mouse_y;

	// ====================================================================================

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		SDL_GetMouseState(&mouse_x, &mouse_y);

		//draw_points(rend, points, { 255, 255, 255, 128 }, Vec2(4, 4));
		//draw_points(rend, hull, { 255, 255, 255, 255 }, Vec2(4, 4));
		//draw_polygon(rend, hull, { 0, 255, 255, 255 });

		int best = g.get_best_parallel();
		SDL_Rect r = g.get_column_rect(best);
		SDL_SetRenderDrawColor(rend, 64, 64, 255, 32);
		SDL_RenderFillRect(rend, &r);

		int highlighted = g.get_at_mouse(Vec2(mouse_x, mouse_y));
		r = g.get_column_rect(highlighted);
		SDL_SetRenderDrawColor(rend, 255, 255, 64, 32);
		SDL_RenderFillRect(rend, &r);

		g.draw(rend);
		draw_text(rend, tex_font, g.stats[highlighted], Vec2(0, 0));


		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_font);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}