#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <ctime>
#include "test/test.h"
#include "render/rendutil.h"
#include "convexhull/convex_hull.h"
#include <algorithm>

using namespace std;

string file_infix = "06";
string fname_in = "res/input_file_" + file_infix + ".txt";
string fname_out_speedup = "out/stats_speedup.txt";
string fname_out_speedup_graham = "out/stats_speedup_graham.txt";
string fname_out_speedup_naive = "out/stats_speedup_naive.txt";
string fname_out_cutoff = "out/stats_cutoff" + file_infix + ".txt";
string fname_out_best_cutoff = "out/stats_best_cutoff.txt";

enum Command { NONE = 0, RUN, GENERATE};
string fname = "";
int sample_size = 0;
bool should_run_test = false;
bool used_io = false;
int command = NONE;

void handle_cmd_args(int argc, char** argv) {
	vector<string> args;
	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}

	for (int i = 0; i < args.size(); i++) {
		if (args[i] == "-i") {
			if (command) {
				throw new int();
			}

			fname = args[++i];
			command = RUN;
		}

		if (args[i] == "-g") {
			if (command) {
				throw new int();
			}
			
			fname = args[++i];
			sample_size = atoi(args[++i].c_str());
			command = GENERATE;
		}

		if (args[i] == "-t") {
			should_run_test = true;
		}
	}

	if (args[1] == "-h" || !command) {
		cout << "Usage:\n";
		cout << "\t ConvexHull -i fname [-t]\n";
		cout << "\t\t run from points defined in `fname`, with -t for optional analysis\n";
		cout << "\n";
		cout << "\t ConvexHull -g fname num\n";
		cout << "\t\t generate `num` many points and write to `fname`\n";
		cout << "\t";
		exit(0);
	}
}

int main(int argc, char** argv) {
	handle_cmd_args(argc, argv);

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* win = SDL_CreateWindow("convex hull", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_SIZE, WIN_SIZE, SDL_WINDOW_OPENGL);
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	SDL_Texture* tex_font = IMG_LoadTexture(rend, "res\\font.png");
	SDL_Event ev;
	bool running = true;
	srand(time(NULL));

	// ====================================================================================

	vector<Vec2> points;
	vector<Vec2> hull;
	stats_t stats;

	if (command == Command::GENERATE) {
		points = generate_points(sample_size);
		output_points(points, fname);
		cout << "Output at " << fname;
		return 0;
	}

	if (command == Command::RUN) {
		points = generate_points(fname);
		if (should_run_test) {
			stats = run_test(points, hull, 20);
		}
		else {
			hull = parallel::convex_hull(points, 20);
		}
	}

	// ====================================================================================

	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_polygon(rend, hull, { 255, 255, 255, 255 });
		draw_points(rend, points, { 255, 255, 255, 255 }, Vec2(3, 3));
		draw_text(rend, tex_font, stats, Vec2(0, 0));
		SDL_RenderPresent(rend);
	}

	SDL_DestroyTexture(tex_font);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;

}