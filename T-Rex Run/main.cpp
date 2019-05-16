#include "GameWindow.h"
#include "GameObject.h"
#include "ObjectShape.h"
#include "GameMessage.h"
#include "ScoreBoard.h"
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

void update(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex, ScoreBoard &playerScore) {
	ground1.ground_update();
	ground2.ground_update();
	obstacle1.obstacle_update();
	obstacle2.obstacle_update();
	t_rex.t_rex_pos_update();
	t_rex.t_rex_shape_update();
	playerScore.updateScore();
}

void scoreBoard_display(GameMessage &sc, GameMessage &hi, ScoreBoard &playerScore) {
	sc.display(770, 10, 50, 20);
	hi.display(770 + 13, 35, 37, 20);
	playerScore.displayScore();
}

void draw(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex) {
	ground1.draw();
	ground2.draw();
	obstacle1.draw();
	obstacle2.draw();
	t_rex.draw();
}

void pause(GameMessage &game_over, GameObject &retry_button, Mix_Chunk* &die_soundEffect) {
	if (GameWindow::playing) 
		Mix_PlayChannel(0, die_soundEffect, 0);

	GameWindow::playing = false;
	game_over.display(250, 100, 400, 60);
	retry_button.draw();
}

void pollEvent(GameWindow &window, GameObject &t_rex) {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		t_rex.pollEvent(event);
		window.pollEvent(event);
	}
}

bool lost(GameObject &t_rex, GameObject &obstacle1, GameObject &obstacle2) {
	int x1 = obstacle1._RECTS[0].x;
	int x2 = obstacle2._RECTS[0].x;
	for (SDL_Rect i : t_rex._RECTS) {
		if (x1 > 0 && (x1 < x2 || x2 < 0)) {
			for (SDL_Rect j : obstacle1._RECTS)
				if (SDL_HasIntersection(&i, &j))
					return true;
		}
		else {
			for (SDL_Rect j : obstacle2._RECTS)
				if (SDL_HasIntersection(&i, &j))
					return true;
		}
	}
	return false;
}

void restart(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex, ScoreBoard &playerScore) {
	ground1.restart();
	ground2.restart();
	obstacle1.restart();
	obstacle2.restart();
	t_rex.restart();
	playerScore.clearScore();
	GameWindow::playing = true;
}

int main(int argc, char* argv[]) {
	srand(time(0));

	string main_font = "font/FFFFORWA.TTF";
	SDL_Color BLACK = { 0,0,0,255 };

	GameWindow window("T-Rex Run", 900, 400);

	Mix_Chunk *die_soundEffect = Mix_LoadWAV("audio/die.wav");

	GameObject t_rex(window, t_rex_rects, 0, 115);
	GameObject ground1(window, ground_rects, -100, 200);
	GameObject ground2(window, ground_rects, -100+900, 200);
	GameObject obstacle1(window, obstacle_rects,1000,145);
	GameObject obstacle2(window, obstacle_rects, 1000+650, 145);
	GameObject retry_button(window, 100, 90, 400, 140, "texture/retry_button.png");

	GameMessage press_to_start("Press SPACE to start", main_font, 100, BLACK);
	GameMessage game_over("GAME OVER", main_font, 100, BLACK);
	GameMessage tutorial1("UP/SPACE: jumping", main_font, 100, BLACK);
	GameMessage tutorial2("DOWN: crouch/fast-falling", main_font, 100, BLACK);
	GameMessage sc("SCORE", main_font, 50, BLACK);
	GameMessage hi("HIGH", main_font, 50, BLACK);

	ScoreBoard playerScore;
	GameMessage digit0("0", main_font, 100, BLACK);		ScoreBoard::score[0] = digit0;
	GameMessage digit1("1", main_font, 100, BLACK);		ScoreBoard::score[1] = digit1;
	GameMessage digit2("2", main_font, 100, BLACK);		ScoreBoard::score[2] = digit2;
	GameMessage digit3("3", main_font, 100, BLACK);		ScoreBoard::score[3] = digit3;
	GameMessage digit4("4", main_font, 100, BLACK);		ScoreBoard::score[4] = digit4;
	GameMessage digit5("5", main_font, 100, BLACK);		ScoreBoard::score[5] = digit5;
	GameMessage digit6("6", main_font, 100, BLACK);		ScoreBoard::score[6] = digit6;
	GameMessage digit7("7", main_font, 100, BLACK);		ScoreBoard::score[7] = digit7;
	GameMessage digit8("8", main_font, 100, BLACK);		ScoreBoard::score[8] = digit8;
	GameMessage digit9("9", main_font, 100, BLACK);		ScoreBoard::score[9] = digit9;

	while (!window.isClosed()) {
		scoreBoard_display(sc, hi, playerScore);
		draw(ground1, ground2, obstacle1, obstacle2, t_rex);
		if (lost(t_rex, obstacle1, obstacle2)) 
			pause(game_over, retry_button, die_soundEffect);

		if (GameWindow::playing)
			update(ground1, ground2, obstacle1, obstacle2, t_rex, playerScore);
		else {
			if (ScoreBoard::high_score == 0) press_to_start.display(215, 100, 500, 60);	//display in the first time play
			tutorial1.display(10, 10, 150, 20);
			tutorial2.display(10, 35, 203, 20);
		}

		pollEvent(window, t_rex);
		if (lost(t_rex, obstacle1, obstacle2) && GameWindow::playing == true) 
			restart(ground1, ground2, obstacle1, obstacle2, t_rex, playerScore);

		window.clear();
	}

	Mix_FreeChunk(die_soundEffect);
	die_soundEffect = nullptr;

	return 0;
}