#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "general.h"
#include "gameConfig.h"
#include "tank.h"

const int BOARD_ROWS = GameConfig::HEIGHT;
const int BOARD_COLS = GameConfig::WIDTH;





class Game
{
	
public:
	Game() {};
	void run();
	void drawBoard();
	void showInstructions();
	void initBoard();
	void showMenu(bool& exit);
	bool isWall(int x, int y);
	bool isMine(int x, int y);
	void pauseGame(Tank& tank1, Tank& tank2, bool& running);
};

