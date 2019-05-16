#include "GameObject.h"
#include "ScoreBoard.h"
#include <iostream>

#define standing 0;
#define jump 0;
#define running1 1;
#define running2 2;
#define crouch_jumping 3;
#define crouch_running1 4;
#define crouch_running2 5;

GameObject::GameObject(GameWindow &window,const std::vector<std::vector<std::vector<int>>> &rects_info, int x, int y) :
	GameWindow(window), _rects_info(rects_info), _x(x), _y(y)
{}

GameObject::GameObject(GameWindow &window, int w, int h, int x, int y, const std::string &img_path) :
	GameWindow(window), _w(w), _h(h), _x(x), _y(y)
{	
	SDL_Surface *surface = IMG_Load(img_path.c_str());
	if (!surface) {
		std::cerr << "Failed to create surface. " << SDL_GetError() << std::endl;
	}

	_texture = SDL_CreateTextureFromSurface(GameWindow::_renderer, surface);
	if (!_texture) {
		std::cerr << "Failed to create texture. " << SDL_GetError() << std::endl;
	}
	SDL_FreeSurface(surface);
}


GameObject::~GameObject() {
	SDL_DestroyTexture(_texture);
}


void GameObject::draw() {
	if (_texture) {
		SDL_Rect rect = { _x, _y, _w, _h };
		SDL_RenderCopy(GameWindow::_renderer, _texture, 0, &rect);
	}
	else {
		for (std::vector<int> i : _rects_info[shape]) {
			SDL_Rect rect = 
			{ 
				i[2] + _x + addition_x,
				i[3] + _y + addition_y,
				i[0], i[1] 
			};
			SDL_SetRenderDrawColor(GameWindow::_renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(GameWindow::_renderer, &rect);

			if (_RECTS.size() < _rects_info[shape].size()) _RECTS.push_back(rect);
			else {
				_RECTS.clear();
				_RECTS.push_back(rect);
			}
		}
	}
}

void GameObject::ground_update() {
	int add_speed = ScoreBoard::current_score/100;
	addition_x -= speed + add_speed;
	int pos = _rects_info[shape][0][2] + _x + addition_x + _rects_info[shape][0][0];
	if (pos <= 0) 		// outside of the screen
		addition_x += 2*_width;
}

void GameObject::obstacle_update() {
	int add_speed = ScoreBoard::current_score/100;
	addition_x -= speed + add_speed;

	std::vector<int> appear_rate;
	if (ScoreBoard::current_score <= 200) appear_rate = { 0,1,1,1,1,2,2,2,2,3,3,3,4};
	else if (ScoreBoard::current_score <= 500) appear_rate = { 0,1,1,2,2,2,2,3,3,3,4,4,4,5,6 };
	else if (ScoreBoard::current_score <= 700)	appear_rate = { 1,1,2,2,2,3,3,3,3,4,4,4,5,5,6,7,7 };
	else appear_rate = { 1,2,3,3,3,3,4,4,4,4,5,5,6,6,7,7 };

	int pos = _rects_info[shape][0][2] + _x + addition_x + _rects_info[shape][0][0];
	if (pos <= -300) {	// outside of the screen
		addition_x += _width + 320;
		shape = appear_rate[rand() % appear_rate.size()];
	}
}


int gravity = 30;
double addition_gravity = 0;
double t = 0.1;
int max_velocity = 100;
int velocity = 100;
void GameObject::t_rex_pos_update() {
	if (jumping) {
		if (fast_falling) addition_gravity += 4;
		addition_y = -(velocity * t) + ((gravity + addition_gravity) * t * t / 2);
		t += 0.15;
		if (addition_y >= 0) {
			jumping = false;
			fast_falling = false;
			t = 0.1;
			addition_y = 0;
			addition_gravity = 0;
		}
	}
	else t = 0.1;
}


int step = 0;
void GameObject::t_rex_shape_update() {
	if (GameWindow::playing) step++;
	if (GameWindow::playing == false) shape = 0;
	else if (jumping) {
		if (crouching) shape = 3;
		else shape = 0;
	}
	else if (crouching) {
		if (shape != 4 && shape != 5) shape = 4;
		if (step % 7 == 0) {
			if (shape != 4) {
				step = 0;
				shape = 4;
			} else if (shape != 5) {
				step = 0;
				shape = 5;
			}
		}
	}
	else {
		if (shape != 1 && shape != 2) shape = 1;
		if (step % 7 == 0) {
			if (shape != 1) {
				step = 0;
				shape = 1;
			} else if (shape != 2) {
				step = 0;
				shape = 2;
			}
		}
	}
}



void GameObject::pollEvent(SDL_Event event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
			if (!jumping) Mix_PlayChannel(0, jump_soundEffect, 0);
			jumping = true;
			crouching = false;
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			if (jumping && !fast_falling) {
				fast_falling = true;
				crouching = true;
			}
			else if (!jumping && !crouching) crouching = true;
		}
		break;
	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_DOWN) crouching = false;
		else if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) && addition_y >= -40 && addition_y < 0)
			addition_gravity += 10;
		break;

	default:
		break;
	}
}

void GameObject::restart() {
	shape = 0;
	addition_x = 0;
	addition_y = 0;
	jumping = false;
	fast_falling = false;
	crouching = false;
 }
