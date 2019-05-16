#include <SDL_ttf.h>
#include "ScoreBoard.h"
#include <sstream>
#include <iostream>

double ScoreBoard::current_score = 0;
int ScoreBoard::high_score = 0;
std::vector<GameMessage> ScoreBoard::score(10, GameMessage("0", "0", 0, { 0,0,0,0 }));

void ScoreBoard::displayScore() {
	int n = current_score;
	int h = high_score;
	int pos_x = 885;
	if (n == 0) score[0].display(pos_x, 10, 10, 20);
	if (h == 0) score[0].display(pos_x, 35, 10, 20);

	for (int i = 0; i < 6; i++) {
		int r = n % 10;
		if (r == 1) score[r].display(pos_x + 4, 10, 5, 20);
		else score[r].display(pos_x, 10, 10, 20);
		pos_x -= 10;
		n /= 10;
	}
	pos_x = 885;
	for (int i = 0; i < 6; i++) {
		int r = h % 10;
		if (r == 1) score[r].display(pos_x + 4, 35, 5, 20);
		else score[r].display(pos_x, 35, 10, 20);
		pos_x -= 10;
		h /= 10;
	}
}

void ScoreBoard::updateScore() {
	current_score += 0.08 ;
	if (current_score > high_score) 
		high_score = current_score;
}