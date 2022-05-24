#include "rendutil.h"

#include <iostream>
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


void draw_stats(SDL_Renderer* rend, const std::vector<stats_t>& stats) {
	int x_min = stats[0].cutoff, x_max = stats[0].cutoff;

	double serial_min = stats[0].time_serial;
	double serial_max = stats[0].time_serial;
	double parallel_min = stats[0].time_parallel;
	double parallel_max = stats[0].time_parallel;
	double speedup_max = stats[0].speedup;

	for (int i = 0; i < stats.size(); i++) {
		if (stats[i].cutoff < x_min) x_min = stats[i].cutoff;
		if (stats[i].cutoff > x_max) x_max = stats[i].cutoff;
		if (stats[i].time_serial < serial_min) serial_min = stats[i].time_serial;
		if (stats[i].time_serial > serial_max) serial_max = stats[i].time_serial;
		if (stats[i].time_parallel < parallel_min) parallel_min = stats[i].time_parallel;
		if (stats[i].time_parallel > parallel_max) parallel_max = stats[i].time_parallel;
		if (stats[i].speedup > speedup_max) speedup_max = stats[i].speedup;
	}

	const double margin = 32;

	double graph_w = WIN_SIZE - 2 * margin;
	double graph_h = WIN_SIZE - 2 * margin;

	double x_step = (x_max - x_min) / graph_w;

	Vec2 origin = Vec2(margin, WIN_SIZE - margin);

	draw_line(rend, origin, Vec2(origin.x + graph_w, origin.y + 0), { 255, 0, 0, 50 });
	draw_line(rend, origin, Vec2(origin.x + 0, origin.y - graph_h), { 0, 255, 0, 50 });

	for (int i = 0; i < stats.size(); i++) {
		const stats_t& s = stats[i];
		double x, y;
		Vec2 point_pos;

		x = 0 + i * (graph_w / stats.size());
		y = (stats[i].time_serial / serial_max) * graph_h;
		point_pos = Vec2(origin.x + x, origin.y - y);
		draw_point(rend, point_pos, { 255,0,0,255 }, Vec2(4, 4));

		x = 0 + i * (graph_w / stats.size());
		y = (stats[i].time_parallel / serial_max) * graph_h;
		point_pos = Vec2(origin.x + x, origin.y - y);
		draw_point(rend, point_pos, { 0,0,255,255 }, Vec2(4, 4));

		x = 0 + i * (graph_w / stats.size());
		y = (stats[i].speedup / speedup_max) * graph_h;
		point_pos = Vec2(origin.x + x, origin.y - y);
		draw_point(rend, point_pos, { 255,255,0,255 }, Vec2(4, 4));
	}
	// x axis is cutoff
	// y axis: one for eaach with mark things
}

void draw_stat(stats_t& stat) {

}