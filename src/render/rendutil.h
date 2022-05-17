#pragma once

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <vector>
#include "utility/mathutil.h"

void draw_points(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col, Vec2 size);
void draw_polygon(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col);
void draw_line(SDL_Renderer* rend, Vec2 A, Vec2 B, SDL_Color col);