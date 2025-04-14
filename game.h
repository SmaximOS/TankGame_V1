#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "general.h"
#include "gameConfig.h"

const int BOARD_ROWS = GameConfig::HEIGHT;
const int BOARD_COLS = GameConfig::WIDTH;





class Game
{
	
public:
	Game() {};
	void run();
	void gameRun();
	void drawBoard();
	void showInstructions();
	void initBoard();
	void showMenu();
	bool isWall(int x, int y);
	bool isMine(int x, int y);
};

