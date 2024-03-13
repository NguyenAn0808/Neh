#include "snake.h"

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <fstream>
#include <string>

using namespace std;

//mechanic
bool samePoint(int x1, int y1, int x2, int y2)
{
	return (x1 == x2 && y1 == y2) ? 1 : 0;
}

bool samePoint(POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}

bool pointStatus(int x, int y) {
	//check if the point already contain a value
	for (int i = 0; i < SIZE_SNAKE; i++)
		if (samePoint(snake[i].x, snake[i].y, x, y))
			return 0;
	for (int i = 0; i < FOOD_INDEX; i++)
		if (samePoint(food[i].x, food[i].y, x, y))
			return 0;
	if (LEVEL_STATE > 1)
	{
		for (int i = 0; i < WALL_SIZE; i++)
		{
			if (samePoint(wall[i].x, wall[i].y, x, y))
				return 0;
		}
	}
	return 1;
}

bool pointValid(POINT target[], int nTarget)
{
	for (int i = 0; i < nTarget; i++)
	{
		for (int j = 0; j < SIZE_SNAKE; j++)
			if (samePoint(target[i], snake[j]))
				return false;
		for (int j = 0; j < FOOD_INDEX; j++)
		{
			if (samePoint(target[i], food[i]))
				return false;
		}
		if (LEVEL_STATE > 1)
		{
			for (int j = 0; j < WALL_SIZE; j++)
			{
				if (samePoint(target[i], wall[j]))
					return false;
				if (samePoint(target[i].x + 3, target[i].y, wall[j].x, wall[j].y))
					return false;
			}
		}
	}
	return true;
}


//Generation & delete
void foodGeneration() { //generate food
	int x, y;
	srand((int)time(0));
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		do
		{
			x = rand() % (WIDTH_CONSOLE - 3) + 2; y = rand() % (HEIGH_CONSOLE - 3) + 2;
		} while (!pointStatus(x, y));
		food[i] = { x , y };
	}
}

void gateGeneration()
{
	//gate
	//
	//036
	//14
	//257
	GATE_SIZE = 8;
	GATE = new POINT[GATE_SIZE];
	srand((int)time(0));
	do
	{
		GATE[4].x = rand() % (WIDTH_CONSOLE - 6) + 2;
		GATE[4].y = rand() % (HEIGH_CONSOLE - 5) + 3;
		GATE[0].x = GATE[4].x - 1; GATE[0].y = GATE[4].y - 1;
		GATE[1].x = GATE[4].x - 1; GATE[1].y = GATE[4].y;
		GATE[2].x = GATE[4].x - 1; GATE[2].y = GATE[4].y + 1;
		GATE[3].x = GATE[4].x; GATE[3].y = GATE[4].y - 1;
		GATE[5].x = GATE[4].x; GATE[5].y = GATE[4].y + 1;
		GATE[6].x = GATE[4].x + 1; GATE[6].y = GATE[4].y - 1;
		GATE[7].x = GATE[4].x + 1; GATE[7].y = GATE[4].y + 1;
	} while (!pointValid(GATE, GATE_SIZE));
}

void deleteGate()
{
	for (int i = 0; i < GATE_SIZE; i++)
	{
		GoToXY(GATE[i].x, GATE[i].y);
		std::cout << ' ';

	}
	delete[] GATE; GATE = NULL;
}

void wallGeneration()
{
	int wIndex = 0;
	switch (LEVEL_STATE)
	{
	case 1: return;
	case 2:
	{
		WALL_SIZE = 146;
		wall = new POINT[WALL_SIZE];
		for (int i = 3; i < HEIGH_CONSOLE - 2; i++)
		{
			wall[wIndex].x = WIDTH_CONSOLE / 2 - 3;
			wall[wIndex].y = i;
			if (i == HEIGH_CONSOLE / 2 - 2) i = HEIGH_CONSOLE / 2 + 1;
			wIndex++;
		}
		for (int i = 3; i < HEIGH_CONSOLE - 2; i++)
		{
			wall[wIndex].x = WIDTH_CONSOLE / 2 + 3;
			wall[wIndex].y = i;
			if (i == HEIGH_CONSOLE / 2 - 2) i = HEIGH_CONSOLE / 2 + 1;
			wIndex++;
		}
		for (int i = 4; i < WIDTH_CONSOLE - 3; i++)
		{
			wall[wIndex].x = i;
			wall[wIndex].y = HEIGH_CONSOLE / 2 - 2;
			if (i == WIDTH_CONSOLE / 2 - 4) i = WIDTH_CONSOLE / 2 + 3;
			wIndex++;
		}
		for (int i = 4; i < WIDTH_CONSOLE - 3; i++)
		{
			wall[wIndex].x = i;
			wall[wIndex].y = HEIGH_CONSOLE / 2 + 2;
			if (i == WIDTH_CONSOLE / 2 - 4) i = WIDTH_CONSOLE / 2 + 3;
			wIndex++;
		}
		break;
	}
	case 3:
	{
		WALL_SIZE = 275;
		wall = new POINT[WALL_SIZE];
		int i = 0;
		for (; i <= HEIGH_CONSOLE - 5; i++)
		{
			//Thanh doc thu 1 ben trai
			wall[wIndex].x = 4;
			wall[wIndex].y = 1 + i;// 1 + 20-5 = 16
			wIndex++;
		}
		for (i = 4; i < WIDTH_CONSOLE - 4; i++)
		{
			//Thanh ngang duoi thu 1
			wall[wIndex].x = i;
			wall[wIndex].y = HEIGH_CONSOLE - 3; // 17
			wIndex++;
		}
		for (i = HEIGH_CONSOLE - 3; i > 3; i--)
		{
			//Thanh doc thu 1 ben phai
			wall[wIndex].x = WIDTH_CONSOLE - 4;
			wall[wIndex].y = i;
			wIndex++;
		}
		for (i = WIDTH_CONSOLE - 4; i > 8; i--)
		{
			//Thanh ngang tren thu 1
			wall[wIndex].x = i;
			wall[wIndex].y = 3;
			wIndex++;
		}
		for (i = 3; i < HEIGH_CONSOLE - 6; i++)
		{
			//Thanh doc ben trai thu 2
			wall[wIndex].x = 8;
			wall[wIndex].y = i;
			wIndex++;
		}
		for (i = 8; i < WIDTH_CONSOLE - 8; i++)
		{
			//Thanh ngang duoi thu 2
			wall[wIndex].x = i;
			wall[wIndex].y = HEIGH_CONSOLE - 6;
			wIndex++;
		}
		for (i = HEIGH_CONSOLE - 6; i > 6; i--)
		{
			//Thanh doc ben phai lan 2
			wall[wIndex].x = WIDTH_CONSOLE - 8;
			wall[wIndex].y = i;
			wIndex++;
		}
		for (i = WIDTH_CONSOLE - 8; i > 10; i--)
		{
			//Thanh ngang tren lan 2
			wall[wIndex].x = i;
			wall[wIndex].y = 6;
			wIndex++;
		}
		break;
	}
	}
}

void ScoreCount() {
	SCORE += SPEED * 10;
}

void ExitGame(thread& t) {
	t.detach(); PLAY = 0;
}

void Pause(HANDLE handle_t) {
	SuspendThread(handle_t);
	BoardRfsh();
	GoToXY(WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2);
	setOutputColor(Red, White);  cout << "PAUSE";
	setOutputColor(White, White);
}


//directional moving
bool rightDead() {
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE && LEVEL_STATE > 1)
		return true;
	for (int i = 0; i < SIZE_SNAKE; i++)
		if ((snake[SIZE_SNAKE - 1].x + 1 == snake[i].x) && (snake[SIZE_SNAKE - 1].y == snake[i].y))
			return true;
	if (LEVEL_UP)
	{
		for (int i = 0; i <= 3; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x + 1 == GATE[i].x) && (snake[SIZE_SNAKE - 1].y == GATE[i].y))
				return true;
		}
		for (int i = 5; i < 8; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x + 1 == GATE[i].x) && (snake[SIZE_SNAKE - 1].y == GATE[i].y))
				return true;
		}
	}
	if (LEVEL_STATE > 1)
		for (int i = 0; i < WALL_SIZE; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x + 1 == wall[i].x) && (snake[SIZE_SNAKE - 1].y == wall[i].y))
				return true;
		}
	return false;
}

void MoveRight() {
	if (rightDead()) {
		ProgressDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y && !LEVEL_UP)
			eat();
		if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE && LEVEL_STATE == 1)
			snake[SIZE_SNAKE - 1].x = 1;
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
			snake[i] = { snake[i + 1].x, snake[i + 1].y };
		snake[SIZE_SNAKE - 1].x++;
	}
}

bool leftDead() {
	if (snake[SIZE_SNAKE - 1].x - 1 == 0 && LEVEL_STATE > 1)
		return true;
	for (int i = 0; i < SIZE_SNAKE; i++)
		if ((snake[SIZE_SNAKE - 1].x - 1 == snake[i].x) && (snake[SIZE_SNAKE - 1].y == snake[i].y))
			return 1;
	if (LEVEL_UP)
	{
		if ((snake[SIZE_SNAKE - 1].x - 1 == GATE[6].x) && (snake[SIZE_SNAKE - 1].y == GATE[6].y))
			return 1;
		if ((snake[SIZE_SNAKE - 1].x - 1 == GATE[7].x) && (snake[SIZE_SNAKE - 1].y == GATE[7].y))
			return 1;
	}
	if (LEVEL_STATE > 1)
		for (int i = 0; i < WALL_SIZE; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x - 1 == wall[i].x) && (snake[SIZE_SNAKE - 1].y == wall[i].y))
				return 1;
		}
	return 0;
}

void MoveLeft() {
	if (leftDead()) {
		ProgressDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y && !LEVEL_UP)
			eat();
		if (snake[SIZE_SNAKE - 1].x - 1 == 0 && LEVEL_STATE == 1)
		{
			snake[SIZE_SNAKE - 1].x = WIDTH_CONSOLE - 1;
		}
		if (LEVEL_UP)
		{
			if (snake[SIZE_SNAKE - 1].x == GATE[4].x && snake[SIZE_SNAKE - 1].y == GATE[4].y)
				for (int i = 0; i < SIZE_SNAKE - 1; i++)
					snake[i] = { snake[i + 1].x, snake[i + 1].y };
			else
			{
				for (int i = 0; i < SIZE_SNAKE - 1; i++)
					snake[i] = { snake[i + 1].x, snake[i + 1].y };
				snake[SIZE_SNAKE - 1].x--;
			}
		}
		else
		{
			for (int i = 0; i < SIZE_SNAKE - 1; i++)
				snake[i] = { snake[i + 1].x, snake[i + 1].y };
			snake[SIZE_SNAKE - 1].x--;
		}
	}
}

bool upDead() {
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 && LEVEL_STATE > 1)
		return 1;
	for (int i = 0; i < SIZE_SNAKE - 1; i++) {
		if ((snake[SIZE_SNAKE - 1].y - 1 == snake[i].y) && (snake[SIZE_SNAKE - 1].x == snake[i].x))
			return 1;
	}
	if (LEVEL_UP)
	{
		for (int i = 0; i < 8; i++)
		{
			if ((snake[SIZE_SNAKE - 1].y - 1 == GATE[i].y) && (snake[SIZE_SNAKE - 1].x == GATE[i].x))
				return 1;
		}
	}
	if (LEVEL_STATE > 1)
		for (int i = 0; i < WALL_SIZE; i++)
		{
			if ((snake[SIZE_SNAKE - 1].y - 1 == wall[i].y) && (snake[SIZE_SNAKE - 1].x == wall[i].x))
				return 1;
		}
	return 0;
}

void MoveUp() {
	if (upDead()) {
		ProgressDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y && snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && !LEVEL_UP)
			eat();
		if (snake[SIZE_SNAKE - 1].y - 1 == 0 && LEVEL_STATE == 1)
			snake[SIZE_SNAKE - 1].y = HEIGH_CONSOLE - 1;
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
			snake[i] = { snake[i + 1].x, snake[i + 1].y };
		snake[SIZE_SNAKE - 1].y--;
	}
}

bool downDead() {
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE && LEVEL_STATE > 1)
		return true;
	for (int i = 0; i < SIZE_SNAKE - 1; i++) {
		if ((snake[SIZE_SNAKE - 1].y + 1 == snake[i].y) && (snake[SIZE_SNAKE - 1].x == snake[i].x))
			return 1;
	}
	if (LEVEL_UP)
	{
		for (int i = 0; i <= 7; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x == GATE[i].x) && (snake[SIZE_SNAKE - 1].y + 1 == GATE[i].y))
				return 1;
		}

	}
	if (LEVEL_STATE > 1)
		for (int i = 0; i < WALL_SIZE; i++)
		{
			if ((snake[SIZE_SNAKE - 1].x == wall[i].x) && (snake[SIZE_SNAKE - 1].y + 1 == wall[i].y))
				return 1;
		}
	return 0;

}

void MoveDown() {
	if (downDead()) {
		ProgressDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y && snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && !LEVEL_UP)
			eat();
		if (snake[SIZE_SNAKE - 1].y + 1 == HEIGH_CONSOLE && LEVEL_STATE == 1)
			snake[SIZE_SNAKE - 1].y = 1;
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
			snake[i] = { snake[i + 1].x, snake[i + 1].y };
		snake[SIZE_SNAKE - 1].y++;
	}
}


//game setup
void eat() {
	//PlaySound(TEXT("hit.wav"), NULL, SND_FILENAME| SND_ASYNC);
	snake[SIZE_SNAKE] = food[FOOD_INDEX]; ScoreCount();
	if (SIZE_SNAKE == MAX_SIZE_ROUND[LEVEL_STATE - 1] - 1) {
		LEVEL_UP = true; SIZE_SNAKE++; FOOD_INDEX = NULL;
		SPEED = 4; FORCE_LOCK[0] = 27; FORCE_LOCK[1] = 'T';
		gateGeneration(); gateOut();
		return;
	}
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1) {
		FOOD_INDEX = 0; SIZE_SNAKE++;
		foodGeneration(); SPEED++;
	}
	else
	{
		SPEED++;
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}

void ProgressDead() { //dead initiate
	FORCE_LOCK[0] = 27; FORCE_LOCK[1] = 'T';
	deadAnimate();
	STATE = 0; LEVEL_UP = 0;
	system("cls");
	if (LEVEL_STATE > 1) {
		delete[] wall; //wall = NULL;
	}
	//reset snake and food data to NULL
	for (int i = 0; i < SIZE_SNAKE; i++) {
		snake[i] = { NULL, NULL };
	}
	for (int i = 0; i < MAX_SIZE_FOOD; i++) {
		food[i] = { NULL, NULL };
	}
	deadMessage();
}

void setup() {
	//preset board and snake/food
	//snake location lock
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 4; FOOD_INDEX = 0;
	WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20, SIZE_SNAKE = 4; LEVEL_UP = false;
	LEVEL_STATE = 1;
	//snake location
	snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
	snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
	//snake[4] = { 14, 5 }; snake[5] = { 15, 5 };
	foodGeneration();
	SCORE = 0;
}

void StartGame() {
	system("CLS");
	GoToXY(0, 0);
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE, Blue);
	if (LEVEL_STATE > 1) {
		wallOut();
	}
	STATE = 1; //snake alive
	PLAY = 1;
	LEVEL_UP = false; EXIT = 0;
}

void Update() {
	setcursor(0, 0);
	while (PLAY) {
		if (STATE == 1) {
			BoardRfsh();
			switch (MOVING) {
			case 'A': MoveLeft(); break;
			case 'D': MoveRight(); break;
			case 'W': MoveUp(); break;
			case 'S': MoveDown(); break;
			}
			ScrUpdate();
		}
	}
}


//save & load game
bool FileExist(string a) {
	ifstream input(a.c_str());
	if (!input) {
		input.close();
		return 0;
	}
	input.close();
	return 1;
}

string* loadRawDATA(int& n) {
	ifstream input("rawData.txt");
	if (!input)
		return NULL;
	input >> n;
	string* a = new string[n + 1];
	for (int i = 0; i < n; i++) {
		getline(input >> ws, a[i]);
	}
	input.close();
	return a;
}

void LoadGame(string a) {
	WIDTH_CONSOLE = 70, HEIGH_CONSOLE = 20;
	ifstream input(a.c_str());
	input >> MOVING >> SPEED >> SCORE >> LEVEL_STATE;
	//snake location
	input >> SIZE_SNAKE;
	for (int i = 0; i < SIZE_SNAKE; i++) {
		input >> snake[i].x >> snake[i].y;
	}
	if (MOVING == 'D') CHAR_LOCK = 'A';
	else if (MOVING == 'W') CHAR_LOCK = 'S';
	else if (MOVING == 'S') CHAR_LOCK = 'W';
	else CHAR_LOCK = 'D';
	//
	if (LEVEL_STATE > 1) {
		wallGeneration();
	}
	FOOD_INDEX = 0;
	foodGeneration();
	input.close();
	return;
}

void SaveGame(string a) {
	//rawData section
	if (!FileExist(a)) {
		int max;
		string* b = loadRawDATA(max);
		ofstream output2("rawData.txt");
		output2 << max + 1 << endl;
		for (int i = 0; i < max; i++) {
			output2 << b[i] << endl;
		}
		output2 << a;
		output2.close();
		delete[] b;
	}
	//Save section
	ofstream output(a.c_str());
	output << MOVING << " " << SPEED << " " << SCORE << " " << LEVEL_STATE << endl;
	output << SIZE_SNAKE << "  ";
	for (int i = 0; i < SIZE_SNAKE; i++) {
		output << snake[i].x << " " << snake[i].y << "  ";
	}
	output.close();
}

//process up Round and main

/// <summary>
/// first update speed and level state, then print out new round.
/// after that, delete recently created gate. 
/// New food, new challenges with wall, and new gate for snake.
/// update snake size.
/// </summary>
void lvlUp()
{
	SPEED += 2;
	if (LEVEL_STATE > 1)
	{
		delete[] wall; wall = NULL;
	}
	LEVEL_STATE++;
	system("cls");
	printRound(LEVEL_STATE);
	deleteGate();
	if (LEVEL_STATE > 3)
	{
		EXIT = 1; return;
	}
	StartGame();
	wallGeneration();
	foodGeneration();
	gateGeneration();
	gateOut();
	SIZE_SNAKE += 4;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		snake[i] = GATE[4];
	}
	wallOut();
	FORCE_LOCK[0] = NULL; FORCE_LOCK[1] = NULL;
}

void game() {
	system("cls");
	if (LOAD) //indicate wheater load file or first play
	{
		LoadGame(file);
	}
	else
		setup();
	//first of game need to print the round
	printRound(LEVEL_STATE);
	StartGame();
	std::thread t1(Update); //assign control to thread t1
	HANDLE handle_t1 = t1.native_handle(); //handle thread t1
	bool pauseflag = 0; PLAY = 1;
	while (1) {
		if (EXIT) { //indicate file saved and exit
			ExitGame(t1); return;
		}
		if (STATE == 0) {
			deadMessage();
		}
		//Sleep(150);
		int temp = toupper(_getch());
		if (STATE == 1) {
			if (temp == CHAR_LOCK || temp == FORCE_LOCK[0] || temp == FORCE_LOCK[1])
				continue;
			if (temp == 'O') {
				SuspendThread(handle_t1);
				Sleep(500);
				system("cls");
				SaveMenu();
				system("cls");
				ResumeThread(handle_t1);
				continue;
			}
			else
				switch (temp) {
				case 'P':
					pauseflag = (pauseflag ? 0 : 1);
					if (pauseflag)
						Pause(handle_t1);
					else {
						GoToXY(WIDTH_CONSOLE / 2 - 2, HEIGH_CONSOLE / 2);
						setOutputColor(Black, White);  cout << "     ";
						ResumeThread(handle_t1);
					}
					break;
				case 27:
					ExitGame(t1); return;
					break;
				default:
					if (!pauseflag) {
						ResumeThread(handle_t1);
						if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S') {
							if (temp == 'D') CHAR_LOCK = 'A';
							else if (temp == 'W') CHAR_LOCK = 'S';
							else if (temp == 'S') CHAR_LOCK = 'W';
							else CHAR_LOCK = 'D';
							MOVING = temp;
						}
						// Through round process
						if (snake[0].x == snake[SIZE_SNAKE - 1].x && snake[0].y == snake[SIZE_SNAKE - 1].y && LEVEL_UP)
						{
							SuspendThread(handle_t1);
							lvlUp();
							ResumeThread(handle_t1);
							MOVING = 'D';
							if (EXIT)
								break;
						}
						// delete gate in case new round
						if (GATE != NULL && !LEVEL_UP)
							if (snake[0].x != GATE[4].x)
							{
								deleteGate();
							}
					}
					break;
				}
		}
		else {
			ExitGame(t1); return;
		}
	}
}