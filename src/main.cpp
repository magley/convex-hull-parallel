#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"

using namespace std;
using namespace tbb;

int GUI_main(int argc, char** argv);

int main(int argc, char** argv) {
	return GUI_main(argc, argv);
}

int GUI_main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* win = SDL_CreateWindow(
		"convex hull",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	SDL_Renderer* rend = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	SDL_Texture* tex_test = IMG_LoadTexture(rend, "res\\img_test.png");

	SDL_Event ev;
	bool running = true;

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
		SDL_RenderClear(rend);

		SDL_Rect src = { 0, 0, 32, 32 };
		SDL_Rect dest = { 128, 128, 32, 32 };
		SDL_RenderCopy(rend, tex_test, &src, &dest);

		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_test);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}