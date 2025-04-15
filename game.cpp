#include "game.h"
#include "Tank.h"         
#include <conio.h>
#include <Windows.h>

using namespace std;

static char board[BOARD_ROWS][BOARD_COLS];

void Game::showInstructions()
{
	cout << "\nInstructions:\n";
	cout << "This is a 2-player game. Each player controls a tank using separate keys.\n\n";
	cout << "Controls:\n";
	cout << "------------------------------------------------------\n";
	cout << " Action                     | Player 1 | Player 2   \n";
	cout << "---------------------------|----------|------------\n";
	cout << " RIGHT track forward       |    E     |     O      \n";
	cout << " RIGHT track backwards     |    D     |     L      \n";
	cout << " LEFT track forward        |    Q     |     U      \n";
	cout << " LEFT track backwards      |    A     |     J      \n";
	cout << " STAY                      |    S     |     K      \n";
	cout << " Shoot                     |    W     |     I (i)  \n";
	cout << "------------------------------------------------------\n";
	cout << endl;
}


void Game::showMenu(bool& exit) {
	system("cls");
	int choice;
	int gameRunning = false;
	while (!gameRunning) {
		cout << "Menu\n";
		cout << "(1) Start a new game\n";
		cout << "(8) Present instructions and keys\n";
		cout << "(9) EXIT\n";
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			gameRunning = true;
			break;
		case 8:
			showInstructions();
			break;
		case 9:
			cout << "Exiting the game. Goodbye!\n";
			exit = true;
			return;
		default:
			cout << "Invalid choice. Please try again.\n";
		}

		cout << "\n";
	}
}

bool Game::isWall(int x, int y) {
	return board[y][x] == '#';
}

bool Game::isMine(int x, int y) {
	return board[y][x] == '@';
}


void Game::initBoard() {
	// Fill the board with empty spaces
	for (int y = 0; y < BOARD_ROWS; ++y) {
		for (int x = 0; x < BOARD_COLS; ++x) {
			board[y][x] = ' ';
		}
	}

	// Outer borders
	for (int x = 0; x < BOARD_COLS; ++x) {
		board[0][x] = '#';
		board[BOARD_ROWS - 1][x] = '#';
	}
	for (int y = 0; y < BOARD_ROWS; ++y) {
		board[y][0] = '#';
		board[y][BOARD_COLS - 1] = '#';
	}

	// Inner walls (example maze)
	for (int x = 2; x < BOARD_COLS - 2; ++x)
		board[2][x] = '#';

	for (int y = 3; y < BOARD_ROWS - 2; ++y)
		board[y][3] = '#';

	for (int x = 3; x < BOARD_COLS - 4; ++x)
		board[BOARD_ROWS - 4][x] = '#';

	for (int y = 5; y < BOARD_ROWS - 5; ++y)
		board[y][BOARD_COLS - 4] = '#';

	// Mines
	board[4][4] = '@';
	board[6][8] = '@';
	board[10][6] = '@';
}

void Game::drawBoard() {
	clrscr();
	for (int y = 0; y < BOARD_ROWS; ++y) {
		for (int x = 0; x < BOARD_COLS; ++x) {
			gotoxy(GameConfig::MINX + x, GameConfig::MINY + y);
			cout << board[y][x];
		}
	}
}

void Game::pauseGame(Tank& tank1, Tank& tank2, bool& running) {
	gotoxy(0, GameConfig::HEIGHT + GameConfig::MINY + 1);
	cout << "Game paused, press ESC again to continue or X to go back to the main menu";
	char keyPressed = 0;

	tank1.draw();
	tank2.draw();


	while (keyPressed != (char)GameConfig::eKeys::ESC && keyPressed != (char)GameConfig::eKeys::RETURN1 && keyPressed != (char)GameConfig::eKeys::RETURN2)
	{
		if (_kbhit())
			keyPressed = _getch();
	}
	if (keyPressed == (char)GameConfig::eKeys::RETURN1 || keyPressed == (char)GameConfig::eKeys::RETURN2)
		running = false;
	

	Point p1 = tank1.getPosition();
	Point p2 = tank2.getPosition();
	gotoxy(p1.getX(), p1.getY());
	cout << " ";
	gotoxy(p2.getX(), p2.getY());
	cout << " ";


	gotoxy(0, GameConfig::HEIGHT + GameConfig::MINY + 1);
	cout << "                                                                         ";

}



void Game::run() {
	bool exit = false;
	while (!exit) {
		initBoard();
		showMenu(exit);
		if (exit)
			break;
		drawBoard();
		Tank tank1(Point(10, 10), 1, GameConfig::cannonDir::EAST);
		Tank tank2(Point(70, 15), 2, GameConfig::cannonDir::WEST);
		bool running = true;
		while (running) {

			tank1.erase();
			tank2.erase();


			if (_kbhit()) {
				char k = _getch();
				if (k == (char)GameConfig::eKeys::ESC) {
					pauseGame(tank1, tank2, running);

				}
			}


			tank1.checkKeysP1();
			tank2.checkKeysP2();


			tank1.tick();
			tank2.tick();


			tank1.draw();
			tank2.draw();


			Sleep(100);
		}
	}
}
