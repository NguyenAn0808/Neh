#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <thread>
#include <string>

#include "snake.h"

using namespace std;

//Main menu's global varible
bool selection[3];
bool animated;


//main menu's control
void WChosing(int& Index, bool select[3]) {
	select[Index] = 0;
	if (Index == 0) {
		Index = 2; select[Index] = 1;
	}
	else {
		Index--; select[Index] = 1;
	}
}

void SChosing(int& Index, bool select[3]) {
	select[Index] = 0;
	if (Index == 2) {
		Index = 0; select[Index] = 1;
	}
	else {
		Index++; select[Index] = 1;
	}
}

//load save's control
void AChosing(int& Index, bool select[2]) {
	select[Index] = 0;
	if (Index == 0) {
		Index = 1; select[Index] = 1;
	}
	else {
		Index--; select[Index] = 1;
	}
}

void DChosing(int& Index, bool select[2]) {
	select[Index] = 0;
	if (Index == 1) {
		Index = 0; select[Index] = 1;
	}
	else {
		Index++; select[Index] = 1;
	}
}


//main menu's title and display
void snakeBoard() {
	bool tw = true;
	bool colorIndex = 0;
	int i = 0;
	while (animated) {
		Sleep(100);
		setOutputColor(Green, White);
		GoToXY(70 - 45, 0); cout << "~( :)>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
		colorIndex = (!colorIndex ? 1 : 0);
		setOutputColor(colorIndex ? Red : Yellow, White);
		GoToXY(70 - 45, 1); cout << "  SSSSSSSS   NNN      NN        A       KK    KKK  EEEEEEEEE\n";
		GoToXY(70 - 45, 2); cout << " SS      SS  NN N     NN       A A      KK   KK    EE\n";
		GoToXY(70 - 45, 3); cout << " SS          NN  N    NN      A   A     KK  KK     EE\n";
		GoToXY(70 - 45, 4); cout << "  SSSSSSSS   NN   N   NN     AAAAAAA    KKKKK      EEEEEEEEE\n";
		GoToXY(70 - 45, 5); cout << "         SS  NN    N  NN    AA     AA   KK  KK     EE\n";
		GoToXY(70 - 45, 6); cout << " SS      SS  NN     N NN   A         A  KK   KK    EE\n";
		GoToXY(70 - 45, 7); cout << "  SSSSSSSS   NN      NNN  A           A KK    KKK  EEEEEEEEE\n";
		setOutputColor(Green, White);
		GoToXY(70 - 45, 8); cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<(: )~";
		setOutputColor(White, White);
		PrintMainMenu();

	}
}

void PrintMainMenu() {
	GoToXY(WIDTH_CONSOLE / 2 + 15, HEIGH_CONSOLE / 2 + 1); setOutputColor(selection[0] ? LRed : Blue, White);
	cout << "START";
	GoToXY(WIDTH_CONSOLE / 2 + 13, HEIGH_CONSOLE / 2 + 3); setOutputColor(selection[1] ? LRed : Blue, White);
	cout << "CONTINUE";
	GoToXY(WIDTH_CONSOLE / 2 + 15, HEIGH_CONSOLE / 2 + 5); setOutputColor(selection[2] ? LRed : Blue, White);
	cout << "QUIT";
	GoToXY(WIDTH_CONSOLE / 2, HEIGH_CONSOLE / 2 + 10);
	setOutputColor(Red, White);
	cout << "S to up, W to down, Enter to select.";
}

//Main menu's mechanic
void MainMenu() {
	setup();
	int Index = 0; animated = 1;
	selection[0] = 1; selection[1] = 0; selection[2] = 0;
	PrintMainMenu();
	thread t1(snakeBoard); HANDLE handle_t1 = t1.native_handle();
	while (1) {
		int key = _getch();
		switch (key) {
		case 119:
			WChosing(Index, selection);
			break;
		case 115:
			SChosing(Index, selection);
			break;
		case 13: // enter
			if (selection[0]) {
				SuspendThread(handle_t1);
				Sleep(100);
				system("cls"); LOAD = 0;
				thread t2(game); t2.join();
				system("CLS");
			}
			if (selection[1]) {
				SuspendThread(handle_t1);
				Sleep(100);
				system("cls"); LOAD = 0;
				thread t2(Continue); t2.join();
				system("CLS");
			}
			if (selection[2]) {
				t1.detach(); animated = 0; system("cls");
				return;
			}
			break;
		default:
			break;
		}
		ResumeThread(handle_t1);
	}
}

//save/load game's global varibles
bool selection2[2];
bool FOUND;


//Save/Load game's display
void YesNoScreen() {
	GoToXY(WIDTH_CONSOLE - 26, HEIGH_CONSOLE - HEIGH_CONSOLE + 12);
	setOutputColor(selection2[0] ? Red : Blue, White);
	cout << "PROCEED";
	GoToXY(WIDTH_CONSOLE - 14, HEIGH_CONSOLE - HEIGH_CONSOLE + 12);
	setOutputColor(selection2[1] ? Red : Blue, White);
	cout << "RETURN";
	setOutputColor(White, White);
}

void YesNoDisable() {
	GoToXY(WIDTH_CONSOLE - 26, HEIGH_CONSOLE - HEIGH_CONSOLE + 12);
	setOutputColor(White, White);
	cout << "       ";
	GoToXY(WIDTH_CONSOLE - 14, HEIGH_CONSOLE - HEIGH_CONSOLE + 12);
	cout << "      ";
}

void datList() {
	int max;
	string* b = loadRawDATA(max);
	if (b != NULL) {
		GoToXY(WIDTH_CONSOLE / 2 + 2, HEIGH_CONSOLE - HEIGH_CONSOLE + 1);
		setOutputColor(Blue, White);
		cout << "saved file's name: ";
		setOutputColor(Green, White); int y = 0, x = 0;
		for (int i = 0; i < max; i++) {
			GoToXY(WIDTH_CONSOLE / 2 + 5 + x, HEIGH_CONSOLE - HEIGH_CONSOLE + 2 + y++);
			cout << *(b + i);
			if (y == 4) {
				x += 15; y = 0;
			}
		}
		delete[] b;
	}
}


//save/load game's control
void Control2(string a) {
	int Index = 0; selection2[0] = 1; selection2[1] = 0;
	YesNoScreen();
	while (1) {
		int temp = toupper(_getch());
		switch (temp) {
		case 'A':
			AChosing(Index, selection2);
			break;
		case 'D':
			DChosing(Index, selection2);
			break;
		case 13:
			if (selection2[0]) {
				if (FileExist(a)) {
					FOUND = 1; LOAD = 1; file = a;
					Sleep(100);
					system("cls");
					thread t(game); t.join();
					system("cls");
				}
				else {
					GoToXY(WIDTH_CONSOLE / 2 + 3, HEIGH_CONSOLE / 2 - 2);
					setOutputColor(Red, White);
					cout << "find not found";
				}
				return;
			}
			else {
				FOUND = 1; return;
			}
			break;
		}
		YesNoScreen();
	}
}

void Control3(string a) {
	int Index = 0; 	selection2[0] = 1; selection2[1] = 0;
	YesNoScreen();
	while (1) {
		int temp = toupper(_getch());
		switch (temp) {
		case 'A':
			AChosing(Index, selection2);
			break;
		case 'D':
			DChosing(Index, selection2);
			break;
		case 13:
			if (selection2[0]) {
				SaveGame(a);
				EXIT = 1;
				if (LEVEL_STATE > 1)
				{
					delete[] wall; wall = NULL;
				}
				return;
			}
			else {
				return;
			}
			break;
		}
		YesNoScreen();
	}
}

//Save/Load menu's mechanic
void Continue() {
	FOUND = 0;
	DrawBoard(WIDTH_CONSOLE / 2 - 8, HEIGH_CONSOLE - HEIGH_CONSOLE, 50, 15, Red);
	while (!FOUND) {
		datList();
		setOutputColor(Red, White);
		string a;
		GoToXY(WIDTH_CONSOLE / 2 - 3, HEIGH_CONSOLE / 2 - 1);
		cout << "enter for next step, then A,D to navigate";
		setOutputColor(Blue, White);
		GoToXY(WIDTH_CONSOLE / 2 + 25, HEIGH_CONSOLE / 2 - 3);
		cout << ".txt";
		GoToXY(WIDTH_CONSOLE / 2 + 6, HEIGH_CONSOLE / 2 - 3);
		cout << "                    ";
		GoToXY(WIDTH_CONSOLE / 2 - 5, HEIGH_CONSOLE / 2 - 3);
		cout << "Enter name: "; getline(cin >> ws, a);
		a += ".txt";
		Control2(a);
		YesNoDisable();
	}
}

void SaveMenu() {
	DrawBoard(WIDTH_CONSOLE / 2 - 8, HEIGH_CONSOLE - HEIGH_CONSOLE, 50, 15, Red);
	datList();
	setOutputColor(Red, White);
	string a;
	GoToXY(WIDTH_CONSOLE / 2 - 5, HEIGH_CONSOLE / 2 - 1);
	cout << "enter for next step, then A,D to navigate";
	setOutputColor(Blue, White);
	GoToXY(WIDTH_CONSOLE / 2 + 25, HEIGH_CONSOLE / 2 - 3);
	cout << ".txt";
	GoToXY(WIDTH_CONSOLE / 2 - 5, HEIGH_CONSOLE / 2 - 3);
	cout << "Enter name: "; getline(cin >> ws, a); a += ".txt";
	Control3(a);
}

void firstWord() {
	setOutputColor(Green, White);
	GoToXY(50, 10);
	cout << "<= THE_SNAKE = >";
	GoToXY(45, 11);
	cout << "team's member: ";
	GoToXY(45, 12);
	cout << "Le Dang Minh KHoi     20127213";
	GoToXY(45, 13);
	cout << "Nguyen Trung Nguyen   20127404";
	GoToXY(45, 14);
	cout << "Doan Anh Duong        20127474";
	GoToXY(45, 15);
	cout << "Nguyen Phuong Khanh   20127204";
	GoToXY(45, 16);
	cout << "Pham Huy Cuong Thinh  20127335";
	GoToXY(47, 17);
	system("Pause");
	system("cls");
	setOutputColor(White, White);
}