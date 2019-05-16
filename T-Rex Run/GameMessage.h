#pragma once

#include "GameWindow.h"
#include <SDL_ttf.h>
#include <string>

class GameMessage{
public:
	GameMessage(const std::string &message, const std::string &font_path, int font_size, const SDL_Color &color);
	~GameMessage();
	
	void display(int x, int y, int width, int height);
	SDL_Texture *loadFont(const std::string &message, const std::string &font_path, int font_size, const SDL_Color &color);

private:
	SDL_Texture *_message_texture;
};