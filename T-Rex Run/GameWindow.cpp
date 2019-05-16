#include "GameWindow.h"
#include <iostream>
#include <vector>

SDL_Renderer *GameWindow::_renderer;
bool GameWindow::playing = false;

GameWindow::GameWindow(std::string title, int width, int height) :
	_title(title), _width(width), _height(height)
{
	if (!init()) {
		_closed = true;
	}
	jump_soundEffect = Mix_LoadWAV("audio/jump.wav");
}

GameWindow::~GameWindow() {
	SDL_DestroyRenderer(GameWindow::_renderer);
	SDL_DestroyWindow(_window);
	IMG_Quit;
	TTF_Quit;
	Mix_FreeChunk(jump_soundEffect);
	jump_soundEffect = nullptr;
	Mix_Quit;
	SDL_Quit;
}

bool GameWindow::init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "Failed to initalize SDL. " << SDL_GetError() << std::endl;
		return false;
	} 

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "Failed to Open Audio. " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "Failed to initalize IMG. " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initalizw TTF. " << SDL_GetError() << std::endl;
		return false;
	}

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0);
	if (_window == nullptr) {
		std::cerr << "Fail to Create Window. " << SDL_GetError() << std::endl;
		return false;
	} 

	GameWindow::_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (GameWindow::_renderer == nullptr) {
		std::cerr << "Fail to Create Renderer. " << SDL_GetError() << std::endl;
		return false;
	} 

	return true;
};

void GameWindow::pollEvent(SDL_Event event) {
	switch (event.type) {
		case SDL_QUIT:
			_closed = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				playing = true;
			}
			break;
		case SDL_MOUSEBUTTONDOWN: {
			int mouse_x = event.motion.x;
			int mouse_y = event.motion.y;
			if (playing == false && mouse_x >= 429 && mouse_y >= 166 && mouse_x <= 474 && mouse_y <= 197) {
				playing = true;
			}
			break;
		}


		default:
			break;
	}
}


void GameWindow::clear() {
	SDL_RenderPresent(GameWindow::_renderer);
	SDL_SetRenderDrawColor(GameWindow::_renderer,200,200,200,255);
	SDL_RenderClear(GameWindow::_renderer);	
}