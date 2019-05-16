#pragma once
#include "GameMessage.h"
#include <SDL_ttf.h>
#include <vector>


class ScoreBoard {
public:
	void clearScore() { current_score = 0; }
	void updateScore();
	void displayScore();

	static double current_score;
	static int high_score;
	static std::vector<GameMessage> score;
};