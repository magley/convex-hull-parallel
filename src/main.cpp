#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <ctime>
#include "test.h"
#include "rendutil.h"

using namespace std;

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

	vector<Vec2> points = generate_points("res/input_file_06.txt");
	vector<Vec2> hull = perform_test(points);

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

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_test);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}