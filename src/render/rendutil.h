#pragma once

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <vector>
#include "utility/mathutil.h"
#include "test/test.h"
#include <string>

#define FONT_CH_W 18
#define FONT_CH_H 24
#define FONT_W 252

void draw_points(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col, Vec2 size);
void draw_polygon(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col);
void draw_line(SDL_Renderer* rend, Vec2 A, Vec2 B, SDL_Color col);

void draw_stats(SDL_Renderer* rend, const std::vector<stats_t>& stats);
void draw_img(SDL_Renderer* rend, SDL_Texture* tex, int x, int y, int w, int h, int subx, int suby, int subw, int subh, int flip);
void draw_text(SDL_Renderer* rend, SDL_Texture* tex, std::string text, Vec2 pos, SDL_Color col, SDL_Color bg);

void draw_text(SDL_Renderer* rend, SDL_Texture* tex, stats_t stat, Vec2 pos);


struct Plot {
private:
	int x_min, x_max;
	double serial_min;
	double serial_max;
	double parallel_min;
	double parallel_max;
	double speedup_max;
	double margin;
	double w, h;
	Vec2 pos;
public:
	std::vector<stats_t>& stats;
	Plot(std::vector<stats_t>& stats, int margin) : stats(stats), margin(margin) {
		pos.x = margin;
		pos.y = WIN_SIZE - margin;
		w = WIN_SIZE - 2 * margin;
		h = WIN_SIZE - 2 * margin;
		init();
	}

	void init();
	void draw(SDL_Renderer* rend);
	int get_at_mouse(Vec2 mouse_pos);
	SDL_Rect get_column_rect(int col_id);
	int get_best_parallel();
};