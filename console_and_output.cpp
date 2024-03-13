#include "snake.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <dos.h>
#include <exception>

using namespace std;

void fixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize;
	bufferSize.X = 150; bufferSize.Y = 50;
	SetConsoleScreenBufferSize(hConsoleWindow, bufferSize);
	//RECT r;
	//GetWindowRect(consoleWindow, &r); //stores the console's current dimensions
	//MoveWindow(consoleWindow, r.left, r.top, 930, 568, TRUE);
}

void GoToXY(int x, int y) {
	COORD coord;
	coord.X = x, coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setcursor(bool visible, DWORD size) {// set bool visible = 0 - invisible, bool visible = 1 - visible
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setOutputColor(int text, int BG) {
	//textColor+ BGColor*16
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, text + BG * 16);
}

void DrawBoard(int x, int y, int width, int height, int color) {
	setOutputColor(color, White);
	GoToXY(x, y); cout << '\xC9';
	GoToXY(x, y + height + y); cout << '\xC8';
	GoToXY(x + width, y); cout << '\xBB';
	GoToXY(x + width, y + height + y); cout << '\xBC';
	GoToXY(x + 1, y); cout << '\xCD';
	for (int i = x; i < x + width - 2; i++)
		std::cout << '\xCD';
	GoToXY(x + 1, y + height + y); std::cout << '\xCD';
	for (int i = x; i < x + width - 2; i++)
		std::cout << '\xCD';

	for (int i = y + 1; i < y + height + y; i++) {
		GoToXY(x, i);  std::cout << '\xBA';
		GoToXY(x + width, i); std::cout << '\xBA';
	}
	setOutputColor(BWhite, White);
	GoToXY(0, 0);
}

void statusBoard() {
	DrawBoard(WIDTH_CONSOLE + 3, 4, 35, 5, 12);
	setOutputColor(Blue, White);
	GoToXY(WIDTH_CONSOLE + 5, 5);
	std::cout << "W, A, S, D to move";
	GoToXY(WIDTH_CONSOLE + 5, 6);
	std::cout << "P to pause or resume";
	GoToXY(WIDTH_CONSOLE + 5, 7);
	std::cout << "O to save and quit";
	GoToXY(WIDTH_CONSOLE + 5, 8);
	std::cout << "ESC to exit";
	GoToXY(WIDTH_CONSOLE + 5, 9);
	std::cout << "size: " << SIZE_SNAKE << " speed: " << SPEED;
	GoToXY(WIDTH_CONSOLE + 5, 10);
	std::cout << "score: " << SCORE;
	setOutputColor(BWhite, White);
	if (LEVEL_UP) {
		GoToXY(WIDTH_CONSOLE + 14, HEIGH_CONSOLE - 5);
		setOutputColor(Green, White);
		cout << "<<=_FINISHED_=>>";
		GoToXY(WIDTH_CONSOLE + 3, HEIGH_CONSOLE - 3);
		setOutputColor(Red, White);
		std::cout << "Enter gate and press any key to continue.";
	}
}

void foodOut(bool type) {
	setOutputColor(LRed, White);
	char chr;
	if (type) chr = 3;
	else chr = ' ';
	GoToXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	std::cout << chr;
	setOutputColor(BWhite, White);
}

void snakeOut(bool type) {
	setOutputColor(Green, White);
	if (type) {
		char sChar[] = { '2','0', '1', '2', '7', '4', '7', '4',
						'2', '0', '1', '2', '7', '4', '0', '4',
						'2', '0', '1', '2', '7', '2', '0', '4',
						'2', '0', '1', '2', '7', '2', '1', '3',
						'2', '0', '1', '2', '7', '3', '3', '5' };
		int index = 0;
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			GoToXY(snake[i].x, snake[i].y);
			std::cout << sChar[index++];
		}
		GoToXY(snake[SIZE_SNAKE - 1].x, snake[SIZE_SNAKE - 1].y);
		std::cout << 'O';
	}
	else
		for (int i = 0; i < SIZE_SNAKE; i++) {
			GoToXY(snake[i].x, snake[i].y);
			std::cout << ' ';
		}
	setOutputColor(BWhite, White);
}

void gateOut() {
	for (int i = 0; i <= 2; i++)
	{
		GoToXY(GATE[i].x, GATE[i].y);
		setOutputColor(Blue, White);
		cout << (char)222;
	}
	setOutputColor(Blue, White);
	GoToXY(GATE[3].x, GATE[3].y); cout << (char)220;
	setOutputColor(Blue, White);
	GoToXY(GATE[5].x, GATE[5].y); cout << (char)223;
	setOutputColor(Blue, White);
	GoToXY(GATE[4].x, GATE[4].y); cout << (char)176;
	setOutputColor(Blue, White);
	GoToXY(GATE[6].x, GATE[6].y); cout << (char)220;
	setOutputColor(Blue, White);
	GoToXY(GATE[7].x, GATE[7].y); cout << (char)223;
}

void wallOut()
{
	switch (LEVEL_STATE)
	{
	case 2:
	{
		for (int i = 3; i < HEIGH_CONSOLE - 2; i++)
		{
			GoToXY(WIDTH_CONSOLE / 2 - 3, i);
			setOutputColor(LYellow, White);
			cout << (char)219;
			if (i == HEIGH_CONSOLE / 2 - 2) i = HEIGH_CONSOLE / 2 + 1;
		}
		for (int i = 3; i < HEIGH_CONSOLE - 2; i++)
		{
			GoToXY(WIDTH_CONSOLE / 2 + 3, i);
			setOutputColor(LPurple, White);
			cout << (char)219;
			if (i == HEIGH_CONSOLE / 2 - 2) i = HEIGH_CONSOLE / 2 + 1;

		}
		for (int i = 4; i < WIDTH_CONSOLE - 3; i++)
		{
			GoToXY(i, HEIGH_CONSOLE / 2 - 2);
			setOutputColor(LRed, White);
			cout << (char)219;
			if (i == WIDTH_CONSOLE / 2 - 4) i = WIDTH_CONSOLE / 2 + 3;
		}
		for (int i = 4; i < WIDTH_CONSOLE - 3; i++)
		{
			GoToXY(i, HEIGH_CONSOLE / 2 + 2);
			setOutputColor(LAqua, White);
			cout << (char)219;
			if (i == WIDTH_CONSOLE / 2 - 4) i = WIDTH_CONSOLE / 2 + 3;
		}
		break;
	}
	case 3:
	{
		int i = 0;
		int color = 1;
		for (; i <= HEIGH_CONSOLE - 5; i++)
		{
			//Thanh doc thu 1 ben trai
			GoToXY(4, 1 + i);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = 4; i < WIDTH_CONSOLE - 4; i++)
		{
			//Thanh ngang duoi thu 1
			GoToXY(i, HEIGH_CONSOLE - 3);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = HEIGH_CONSOLE - 3; i > 3; i--)
		{
			//Thanh doc thu 1 ben phai
			GoToXY(WIDTH_CONSOLE - 4, i);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = WIDTH_CONSOLE - 4; i > 8; i--)
		{
			//Thanh ngang tren thu 1
			GoToXY(i, 3);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = 3; i < HEIGH_CONSOLE - 6; i++)
		{
			//Thanh doc ben trai thu 2
			GoToXY(8, i);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = 8; i < WIDTH_CONSOLE - 8; i++)
		{
			//Thanh ngang duoi thu 2
			GoToXY(i, HEIGH_CONSOLE - 6);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color += 2;
		for (i = HEIGH_CONSOLE - 6; i > 6; i--)
		{
			//Thanh doc ben phai lan 2
			GoToXY(WIDTH_CONSOLE - 8, i);
			setOutputColor(color, White);
			cout << (char)219;
		}
		color++;
		for (i = WIDTH_CONSOLE - 8; i > 10; i--)
		{
			//Thanh ngang tren lan 2
			GoToXY(i, 6);
			setOutputColor(color, White);
			cout << (char)219;
		}
		break;
	}
	}
}

void ScrUpdate() {
	snakeOut(STATE);
	foodOut(!(LEVEL_UP || !STATE));
	statusBoard();
	Sleep(1000 / SPEED);
}

void BoardRfsh() {
	snakeOut(0);
	foodOut(0);
}

void deadAnimate() {
	int i = 0;
	while ((i++) < 4) {
		for (int i = SIZE_SNAKE - 1; i >= 0; i--) {
			GoToXY(snake[i].x, snake[i].y);
			std::cout << ' ';
		}
		Sleep(300);
		setOutputColor(Green, White);
		for (int i = SIZE_SNAKE - 1; i >= 0; i--) {
			GoToXY(snake[i].x, snake[i].y);
			std::cout << '\xB2';
		}
		setOutputColor(White, White);
		Sleep(300);
	}
}

void printRound(int LEVEL_STATE)
{
	if (LEVEL_STATE == 1) {
		system("cls");
		setOutputColor(Red, White);
		/*GoToXY(1, 9);
		cout << " ______________________________________________________________________________________________________________________\n";
		cout << "|                                                                                                                      |\n";
		cout << "|== __________            _______       ___        ___   _______         ___   ___________             _______         |\n";
		cout << "|==|    ______|___    ___|_______|___  |   |      |   | |       \\       |   | |    _______|___        /       |        |\n";
		cout << "|==|   |      |   |  |   |       |   | |   |      |   | |   |\\   \\      |   | |   |       |   |      /   /|   |        |\n";
		cout << "|==|   |      |   |  |   |       |   | |   |      |   | |   | \\   \\     |   | |   |       |   |     /   / |   |        |\n";
		cout << "|==|   |______|___|  |   |       |   | |   |      |   | |   |  \\   \\    |   | |   |       |   |    /   /  |   |        |\n";
		cout << "|==|    ____   \\     |   |       |   | |   |      |   | |   |   \\   \\   |   | |   |       |   |   /___/  |   |         |\n";
		cout << "|==|   |    \\   \\    |   |       |   | |   |      |   | |   |    \\   \\  |   | |   |       |   |         |   |          |\n";
		cout << "|==|   |     \\   \\   |   |       |   | |   |      |   | |   |     \\   \\ |   | |   |       |   |         |   |          |\n";
		cout << "|==|   |      \\   \\  |___|_______|___| |___|______|___| |   |      \\   \\|   | |   |_______|___| ________|   |________  |\n";
		cout << "|==|___|       \\___\\     |_______|         |______|     |___|       \\_______| |___________|     |____________________|=|\n";
		cout << "|                                                                                                                      |\n";
		cout << "|______________________________________________________________________________________________________________________|\n";*/
		GoToXY(WIDTH_CONSOLE - 20, HEIGH_CONSOLE - 7);
		cout << ">>ROUND 1<<";
		Sleep(3000);
	}
	if (LEVEL_STATE == 2)
	{
		system("CLS");
		setOutputColor(Red, White);
		/*GoToXY(1, 9);
		cout << " ______________________________________________________________________________________________________________________\n";
		cout << "|                                                                                                                      |\n";
		cout << "|== __________            _______       ___        ___   _______         ___   ___________           ___________       |\n";
		cout << "|==|    ______|___    ___|_______|___  |   |      |   | |       \\       |   | |    _______|___   ___|___________|___   |\n";
		cout << "|==|   |      |   |  |   |       |   | |   |      |   | |   |\\   \\      |   | |   |       |   | |   |           |   |==|\n";
		cout << "|==|   |      |   |  |   |       |   | |   |      |   | |   | \\   \\     |   | |   |       |   | |___|           |   |==|\n";
		cout << "|==|   |______|___|  |   |       |   | |   |      |   | |   |  \\   \\    |   | |   |       |   |              ___|___|==|\n";
		cout << "|==|    ____   \\     |   |       |   | |   |      |   | |   |   \\   \\   |   | |   |       |   |          ___|___|    ==|\n";
		cout << "|==|   |    \\   \\    |   |       |   | |   |      |   | |   |    \\   \\  |   | |   |       |   |      ___|___|        ==|\n";
		cout << "|==|   |     \\   \\   |   |       |   | |   |      |   | |   |     \\   \\ |   | |   |       |   |  ___|___|            ==|\n";
		cout << "|==|   |      \\   \\  |___|_______|___| |___|______|___| |   |      \\   \\|   | |   |_______|___| |   |_______________ ==|\n";
		cout << "|==|___|       \\___\\     |_______|         |______|     |___|       \\_______| |___________|     |___________________|==|\n";
		cout << "|                                                                                                                      |\n";
		cout << "|______________________________________________________________________________________________________________________|\n";
		setOutputColor(White, White);*/
		GoToXY(WIDTH_CONSOLE - 20, HEIGH_CONSOLE - 7);
		cout << ">>ROUND 2<<";
		Sleep(3000);
	}
	if (LEVEL_STATE == 3) {
		setOutputColor(Red, White);
		GoToXY(WIDTH_CONSOLE - 20, HEIGH_CONSOLE - 7);
		cout << ">>ROUND 3<<";
		Sleep(3000);
	}
	if (LEVEL_STATE == 4) {
		setOutputColor(Red, White);
		GoToXY(WIDTH_CONSOLE / 2 - 18, HEIGH_CONSOLE - 7);
		cout << "YOU WIN!!!";
		Sleep(10000);
	}
}

void deadMessage() {
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE, Red); int i = 0;
	setOutputColor(LRed, White);
	GoToXY(WIDTH_CONSOLE / 2 - 1, HEIGH_CONSOLE / 2 - 1);
	std::cout << "DEAD.";
	GoToXY(WIDTH_CONSOLE / 2 - 3, HEIGH_CONSOLE / 2);
	std::cout << "SCORE: " << SCORE;
	GoToXY(WIDTH_CONSOLE / 2 - 8, HEIGH_CONSOLE / 2 + 1);
	std::cout << "Press any key to exit";
	FORCE_LOCK[0] = NULL; FORCE_LOCK[1] = NULL;
	setOutputColor(Black, White);
}