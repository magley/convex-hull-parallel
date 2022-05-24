#include "rendutil.h"

#include <sstream>
using namespace std;

void draw_point(SDL_Renderer* rend, Vec2 point, SDL_Color col, Vec2 size) {
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
	SDL_FRect r{ point.x - size.x / 2, point.y - size.y / 2, size.x, size.y };
	SDL_RenderFillRectF(rend, &r);
}

void draw_points(SDL_Renderer* rend, const vector<Vec2>& points, SDL_Color col, Vec2 size) {
	for (int i = 0; i < points.size(); i++) {
		draw_point(rend, points[i], col, size);
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

void Plot::init() {
	for (int i = 0; i < stats.size(); i++) {
		if (i == 0 || stats[i].cutoff < x_min) x_min = stats[i].cutoff;
		if (i == 0 || stats[i].cutoff > x_max) x_max = stats[i].cutoff;
		if (i == 0 || stats[i].time_serial < serial_min) serial_min = stats[i].time_serial;
		if (i == 0 || stats[i].time_serial > serial_max) serial_max = stats[i].time_serial;
		if (i == 0 || stats[i].time_parallel < parallel_min) parallel_min = stats[i].time_parallel;
		if (i == 0 || stats[i].time_parallel > parallel_max) parallel_max = stats[i].time_parallel;
		if (i == 0 || stats[i].speedup > speedup_max) speedup_max = stats[i].speedup;
	}
}

void Plot::draw(SDL_Renderer* rend) {
	draw_line(rend, pos, Vec2(pos.x + w, pos.y + 0), { 255, 0, 0, 50 });
	draw_line(rend, pos, Vec2(pos.x + 0, pos.y - h), { 0, 255, 0, 50 });

	int left = pos.x;
	int right = left + w;
	int column_width = (right - left) / (stats.size());

	for (int i = 0; i < stats.size(); i++) {
		const stats_t& s = stats[i];
		double x, y;
		Vec2 point_pos;

		x = 0 + i * (w / stats.size());
		y = (stats[i].time_serial / serial_max) * h;
		point_pos = Vec2(pos.x + x, pos.y - y);
		draw_point(rend, point_pos, { 255,0,0,255 }, Vec2(column_width, column_width));

		x = 0 + i * (w / stats.size());
		y = (stats[i].time_parallel / serial_max) * h;
		point_pos = Vec2(pos.x + x, pos.y - y);
		draw_point(rend, point_pos, { 0,0,255,255 }, Vec2(column_width, column_width));

		x = 0 + i * (w / stats.size());
		y = (stats[i].speedup / speedup_max) * h;
		point_pos = Vec2(pos.x + x, pos.y - y);
		draw_point(rend, point_pos, { 255,255,0,255 }, Vec2(column_width, column_width));
	}
}

#include <iostream>

int Plot::get_at_mouse(Vec2 mouse_pos) {
	int left = pos.x;
	int right = left + w;

	if (mouse_pos.x < left)
		mouse_pos.x = left;
	if (mouse_pos.x > right)
		mouse_pos.x = right;

	int column_width = (right - left) / (stats.size());
	int col_margin = column_width / 2;
	int column_index = (mouse_pos.x - left + col_margin) / column_width;

	if (column_index < 0)
		column_index = 0;
	if (column_index >= stats.size())
		column_index = stats.size() - 1;
	return column_index;
}

SDL_Rect Plot::get_column_rect(int column_index) {
	int left = pos.x;
	int right = left + w;

	int column_width = (right - left) / (stats.size());
	int col_margin = column_width / 2;

	int x = left + column_index * column_width;

	SDL_Rect r;
	r.x = x - col_margin;
	r.y = 0;
	r.w = column_width;
	r.h = WIN_SIZE;
	return r;
}

int Plot::get_best_parallel() {
	stats_t best = stats[0];
	int best_index = 0;
	for (int i = 0; i < stats.size(); i++) {
		if (stats[i].time_parallel < best.time_parallel) {
			best = stats[i];
			best_index = i;
		}
	}
	return best_index;
}
void draw_text(SDL_Renderer* rend, SDL_Texture* tex, std::string text, Vec2 pos, SDL_Color col, SDL_Color bg) {
	const unsigned chars_per_line = FONT_W / FONT_CH_W;

	// Background

	const int p = 0; // padding
	SDL_Rect rect = {
		pos.x - p,
		pos.y - p,
		(int)(FONT_CH_W * 1.0 * text.size()) + 2 * p,
		(int)(FONT_CH_H * 1.0 + 2 * p)
	};

	SDL_SetRenderDrawColor(rend, bg.r, bg.g, bg.b, bg.a);
	SDL_RenderFillRect(rend, &rect);

	// Text

	SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
	SDL_SetTextureAlphaMod(tex, col.a);

	for (unsigned i = 0; i < text.size(); i++) {

		const char c = text[i];

		const int char_x = (c % chars_per_line) * FONT_CH_W;
		const int char_y = (c / chars_per_line) * FONT_CH_H;

		draw_img(
			rend,
			tex,
			pos.x + i * FONT_CH_W * 1.0,
			pos.y,
			(int)round(FONT_CH_W * 1.0),
			(int)round(FONT_CH_H * 1.0),
			char_x,
			char_y,
			FONT_CH_W,
			FONT_CH_H,
			SDL_FLIP_NONE
		);
	}
}

void draw_img(SDL_Renderer* rend, SDL_Texture* tex, int x, int y, int w, int h, int subx, int suby, int subw, int subh, int flip) {
	const SDL_Rect dest{ x, y, w, h };
	const SDL_Rect src{ subx, suby, subw, subh };
	SDL_RenderCopyEx(rend, tex, &src, &dest, 0, NULL, (SDL_RendererFlip)flip);
}

void draw_text(SDL_Renderer* rend, SDL_Texture* tex, stats_t stat, Vec2 pos) {
	{
		stringstream ss;
		ss << "cutoff:   " << stat.cutoff;
		draw_text(rend, tex, ss.str(), pos, { 128,128,128,255 }, { 0, 0, 0 ,0 });
	}
	{
		pos.y += FONT_CH_H;
		stringstream ss;
		ss << "serial:   " << stat.time_serial << "s";
		draw_text(rend, tex, ss.str(), pos, { 255,0,0,255 }, { 0, 0, 0 ,0 });
	}
	{
		pos.y += FONT_CH_H;
		stringstream ss;
		ss << "parallel: " << stat.time_parallel << "s";
		draw_text(rend, tex, ss.str(), pos, { 0,0,255,255 }, { 0, 0, 0 ,0 });
	}
	{
		pos.y += FONT_CH_H;
		stringstream ss;
		ss << "speedup:  " << stat.speedup;
		draw_text(rend, tex, ss.str(), pos, { 255,255,0,255 }, { 0, 0, 0 ,0 });
	}
}