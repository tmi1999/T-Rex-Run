#include "GameMessage.h"
#include <iostream>

GameMessage::GameMessage(const std::string &message, const std::string &font_path, int font_size, const SDL_Color &color)
{
	_message_texture = loadFont(message, font_path, font_size, color);
}

GameMessage::~GameMessage() {
	SDL_DestroyTexture(_message_texture);
}

void GameMessage::display(int x, int y, int width, int height) {
	SDL_Rect rect = { x,y,width,height };
	SDL_RenderCopy(GameWindow::_renderer, _message_texture, 0, &rect);
}

SDL_Texture *GameMessage::loadFont(const std::string &message, const std::string &font_path, int font_size, const SDL_Color &color) {
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	SDL_Surface *message_surface = TTF_RenderText_Solid(font, message.c_str(), color);
	SDL_Texture *message_texture = SDL_CreateTextureFromSurface(GameWindow::_renderer, message_surface);

	SDL_FreeSurface(message_surface);
	return message_texture;
}