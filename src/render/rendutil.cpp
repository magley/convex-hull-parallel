#include "rendutil.h"

using namespace std;

void draw_points(SDL_Renderer* rend, const vector<Vec2>& points, SDL_Color col, Vec2 size) {
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
	for (int i = 0; i < points.size(); i++) {
		Vec2 point = points[i];
		SDL_FRect r{ point.x - size.x / 2, point.y - size.y / 2, size.x, size.y };
		SDL_RenderFillRectF(rend, &r);
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
