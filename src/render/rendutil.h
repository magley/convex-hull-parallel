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

/// <summary>
/// Draw a set of points as rectangles.
/// </summary>
/// <param name="rend">SDL Renderer</param>
/// <param name="points">Set of points</param>
/// <param name="col">Color to draw the points with</param>
/// <param name="size">Size of each point in pixels</param>
void draw_points(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col, Vec2 size);

/// <summary>
/// pre-condition: Points must be sorted by polar coordinates.
/// 
/// Draws a closed polygon.
/// </summary>
/// <param name="rend">SDL Renderer</param>
/// <param name="points">Set of points</param>
/// <param name="col">Color to draw the polygon with</param>
void draw_polygon(SDL_Renderer* rend, const std::vector<Vec2>& points, SDL_Color col);

/// <summary>
/// Draws a line segment between two points.
/// </summary>
/// <param name="rend">SDL Renderer</param>
/// <param name="A">First point</param>
/// <param name="B">Second point</param>
/// <param name="col">Color to draw the line with</param>
void draw_line(SDL_Renderer* rend, Vec2 A, Vec2 B, SDL_Color col);

/// <summary>
/// Draws an image.
/// </summary>
/// <param name="rend">SDL Renderer</param>
/// <param name="tex">SDL Texture to draw</param>
/// <param name="x">X position on screen to draw the image</param>
/// <param name="y">Y position on screen to draw the image</param>
/// <param name="w">Width to draw by</param>
/// <param name="h">Height to draw by</param>
/// <param name="subx">Leftmost coordinate of the subimage to draw</param>
/// <param name="suby">Topmost coordinate of the subimage to draw</param>
/// <param name="subw">Width of the subimage to draw</param>
/// <param name="subh">Height of the subimage to draw</param>
/// <param name="flip">How to flip the image, one of SDL_FLIP_</param>
void draw_img(SDL_Renderer* rend, SDL_Texture* tex, int x, int y, int w, int h, int subx, int suby, int subw, int subh, int flip);

/// <summary>
/// Draws text.
/// </summary>
/// <param name="rend">SDL Renderer</param>
/// <param name="tex">Fontmap</param>
/// <param name="text">Text to render, supports newline</param>
/// <param name="pos">Where to draw the text</param>
/// <param name="col">Color of the text</param>
/// <param name="bg">Color of the background</param>
void draw_text(SDL_Renderer* rend, SDL_Texture* tex, std::string text, Vec2 pos, SDL_Color col, SDL_Color bg);

/// <summary>
/// Draws the given stat as text.
/// </summary>
void draw_text(SDL_Renderer* rend, SDL_Texture* tex, stats_t stat, Vec2 pos);