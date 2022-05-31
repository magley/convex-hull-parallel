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
		draw_text(rend, tex, ss.str(), pos, { 0,255,255,255 }, { 0, 0, 0 ,0 });
	}
	{
		pos.y += FONT_CH_H;
		stringstream ss;
		ss << "speedup:  " << stat.speedup;
		draw_text(rend, tex, ss.str(), pos, { 255,255,0,255 }, { 0, 0, 0 ,0 });
	}
}