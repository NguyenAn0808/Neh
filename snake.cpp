#include <iostream>
#include "snake.h"

using namespace std;

//global value(varible)
int GATE_SIZE;
int WALL_SIZE;
POINT snake[MAX_SIZE_SNAKE]; //snake and it's body cordinate
POINT food[MAX_SIZE_FOOD]; //food cordinate
int MAX_SIZE_ROUND[] = { 16, 32, 41 };
POINT* wall;
int CHAR_LOCK, FORCE_LOCK[2];//unmoveable side
int MOVING;//define current side when moving
int SPEED;//speed, level
int HEIGH_CONSOLE, WIDTH_CONSOLE; //console size
int FOOD_INDEX;//currently showned up food
int SIZE_SNAKE; //snake size
int STATE; //snake status(die/alive)
int LEVEL_STATE;// variable hold the state of round
long int SCORE;
bool LEVEL_UP; //Manage through-round process
POINT* GATE; //gate way
bool PLAY; //game status
bool LOAD;
std::string file;
bool EXIT; //exit game
//--------------------------------------------------------

int main() {
	system("color 70");
	firstWord();
	setOutputColor(Black, White);
	fixConsoleWindow(); //fix console to a set size
	setcursor(0, 0); //removed cursor
	MainMenu();
	system("CLS");
	return 0;
}