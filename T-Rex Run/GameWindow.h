#pragma once


#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

class GameWindow {
public:
	GameWindow(const std::string title, int width, int height);
	~GameWindow();

	bool init();
	void pollEvent(SDL_Event event);
	void clear();
	bool isClosed() { return _closed; }

	static bool playing;
protected:
	int _width;
	int _height;
	Mix_Chunk *jump_soundEffect;
private:
	std::string _title;
	bool _closed = false;
	
	SDL_Window *_window = nullptr;
public:
	static SDL_Renderer *_renderer;
};