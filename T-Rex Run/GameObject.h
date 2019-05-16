#pragma once

#include "GameWindow.h"
#include <SDL_image.h>
#include <string>
#include <vector>

class GameObject : public GameWindow {
public:
	GameObject(GameWindow &window, int w, int h, int x, int y, const std::string &img_path);
	GameObject(GameWindow &window, const std::vector<std::vector<std::vector<int>>> &rects_info, int x, int y);
	~GameObject();

	std::vector<SDL_Rect> _RECTS;

	void draw();
	void ground_update();
	void obstacle_update();
	void t_rex_pos_update();
	void t_rex_shape_update();
	void pollEvent(SDL_Event event);
	void restart();

	bool jumping = false;
	bool crouching = false;
	bool fast_falling = false;
private:
public:
	int _w, _h;
	int _x, _y;
	std::vector<std::vector<std::vector<int>>> _rects_info;
public:
	int shape = 0;
	int addition_x = 0;
	int addition_y = 0;

	SDL_Texture *_texture = nullptr;
	int speed = 5;
};