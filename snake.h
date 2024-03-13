#include <windows.h>
#include <thread>
#include <string>

#ifndef _globalValue
#define _globalValue
#define MAX_SIZE_SNAKE 41
#define MAX_SPEED 30
#define MAX_SIZE_FOOD 6
//Color
#define Black 0
#define Blue 1
#define Green 2
#define Aqua 3
#define Red 4
#define Purple 5
#define Yellow 6
#define White 7
#define Gray 8
#define LBlue 9
#define LGreen 10
#define LAqua 11
#define LRed 12
#define LPurple 13
#define LYellow 14
#define BWhite 15


//global value(varible)
extern int GATE_SIZE;
extern int WALL_SIZE;
extern POINT snake[MAX_SIZE_SNAKE]; //snake and it's body cordinate
extern POINT food[MAX_SIZE_FOOD]; //food cordinate
extern POINT* wall;
extern int MAX_SIZE_ROUND[];
extern int CHAR_LOCK, FORCE_LOCK[2];//unmoveable side
extern int MOVING;//define current side when moving
extern int SPEED;//speed, level
extern int HEIGH_CONSOLE, WIDTH_CONSOLE; //console size
extern int FOOD_INDEX;//currently showned up food
extern int SIZE_SNAKE; //snake size
extern int STATE; //snake status(die/alive)
extern int LEVEL_STATE;
extern long int SCORE; //score
extern bool LEVEL_UP;
extern POINT* GATE;
extern bool PLAY;
extern bool LOAD;
extern std::string file;
extern bool EXIT;
#endif

#ifndef _console
#define _console
void fixConsoleWindow(); //fixed console size
void GoToXY(int x, int y); //cordinate jump
void setcursor(bool visible, DWORD size);//remove cursor visible = 0 -> cursor removed
void setOutputColor(int text, int BG);// 12 for red, 14 for yellow, 15 for white, 10 for green
void DrawBoard(int x, int y, int width, int height, int color);
void statusBoard();
void foodOut(bool type);
void snakeOut(bool type);
void gateOut();
void wallOut();
void ScrUpdate();
void BoardRfsh();
void deadAnimate();
void printRound(int LEVEL_STATE);
void deadMessage();
#endif

#ifndef _mainMenu
#define _mainMenu
extern bool selection[3];
void WChosing(int& Index, bool select[3]);
void SChosing(int& Index, bool select[3]);
void snakeBoard();

void PrintMainMenu();
void MainMenu();
//save menu
void AChosing(int& Index, bool select[2]);
void DChosing(int& Index, bool select[2]);
void YesNoScreen();
void YesNoDisable();
void datList();
void Control2(std::string a);
void Control3(std::string a);
void Continue();
void SaveMenu();
void firstWord();
#endif


#ifndef _main
#define _main
bool samePoint(int x1, int y1, int x2, int y2);
bool samePoint(POINT a, POINT b);
bool pointStatus(int x, int y);
bool pointValid(POINT target[], int nTarget);

void gateGeneration();
void foodGeneration();
void wallGeneration();
void deleteGate();

void ScoreCount();

void lvlUp();
void setup();
void StartGame();
void Pause(HANDLE handle_t);
void ExitGame(std::thread& t);
void eat();
void ProgressDead();
//directional
void MoveRight(); bool rightDead();
void MoveLeft(); bool leftDead();
void MoveUp(); bool upDead();
void MoveDown(); bool downDead();
//game
void Update();
void game();
//save game
std::string* loadRawDATA(int& n);
bool FileExist(std::string a);
void LoadGame(std::string a);
void SaveGame(std::string a);
#endif