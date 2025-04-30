#pragma once
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>  // For system()

#include <vector>
#include <string>



#include "utils.h"

using namespace std;
static constexpr int SCREEN_DURATION = 2000;



class Menu
{

	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
	

	static constexpr char NEW_GAME = '1';
	static constexpr char CHOOSE_BOARD = '2';
	static constexpr char SHOW_INSTRUCTIONS = '8';
	static constexpr char EXIT = '9';





	const char* openScreen[MAX_Y] = {
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
	"W                                                                              W", // 1
	"W    ___    ___  ____  __  _   ___ __ __                                       W", // 2
	"W   |   \\  /   \\|    \\|  |/ ] /  _]  |  |                                      W", // 3
	"W   |    \\|     |  _  |  ' / /  [_|  |  |                    __                W", // 4
	"W   |  D  |  O  |  |  |    \\|    _]  ~  |                   / _,\\              W", // 5
	"W   |     |     |  |  |     \\   [_|___, |                   \\_\\                W", // 6
	"W   |     |     |  |  |  .  |     |     |        ,,,,    _,_)  #      /)       W", // 7
	"W   |____\\|\\___/|__|__|__|\\_|_____|____/         [ ]  __/    __/     //        W", // 8
	"W                                              C[____]     _(    ___//         W", // 9
	"W          __  _  ___  ____   ____               \\_,/  -.   '-._/,--'          W", // 10
	"W         |  |/ ]/   \\|    \\ /    |        _\\_,  /           -//.              W", // 11
	"W         |  ' /|     |  _  |   __|         \\_ \\_/  -,._ _     ) )             W", // 12
	"W         |    \\|  O  |  |  |  |  |           \\/    /    )    / /              W", // 13
	"W         |     \\     |  |  |  |_ |           \\-__,/    (    ( (               W", // 14
	"W         |  .  |     |  |  |     |                      \\.__,-)\\_             W", // 15
	"W         |__|\\_|\\___/|__|__|___,_|                       )\\_ / -(             W", // 16
	"W                                                        / -(////              W", // 17
	"W                                                       ////                   W", // 18
	"W                                                                              W", // 19
	"W                                                                              W", // 20
	"W                                                                              W", // 21
	"W                                                                              W", // 22
	"W                                                                              W", // 23
	"================================================================================"  // 24
	};


	const char* mainMenu[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		  "W                                                                              W", // 1
		  "W                                                                              W", // 2
		  "W                                                                              W", // 3
		  "W                                                                              W", // 4
		  "W             ====================  MENU  =====================                W", // 5
		  "W                                                                              W", // 6
		  "W                                                                              W", // 7
		  "W                                                                              W", // 8
		  "W                (1) Start a new game                                          W", // 9
		  "W                                                                              W", // 10
		  "W                (2) Pick Your Level (The game starts immediately)             W", // 11 
		  "W                                                                              W", // 12 
		  "W                (8) Present instructions and keys                             W", //13
		  "W                                                                              W", // 14
		  "W                (9) EXIT (Press at any point to exit the game)                W", // 15
		  "W                                                                              W", // 16
		  "W                                                                              W", // 17
		  "W                                                                              W", // 18
		  "W                                                                              W", // 19
		  "W                                                                              W", // 20
		  "W                                                                              W", // 21
		  "W                                                                              W", // 22
		  "W                                                                              W", // 23
		  "================================================================================"  // 24
	};


	const char* instructionMenu[MAX_Y] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1
		"W      ====================  INSTRUCTIONS  =====================               W", // 2
		"W                                                                              W", // 3
		"W    + Mission: Uh-oh! Donkey Kong has taken Pauline!                          W", // 4
		"W      It's up to you to save her and stop that pesky ape!                     W", // 5
		"W                                                                              W", // 6
		"W    + Controls:                                                               W", // 7
		"W      - Move: Use the 'A'/'D' keys to guide Mario across the screen.          W", // 8
		"W      - Jump : Press 'W' to leap over those dangerous barrels!                W", // 9
		"W      - Ladders: Use the 'W'/'X' keys to guide Mario up / down the ladder.    W", // 10
		"W      - P is for you hammer, use it to defend afaingt the ghosts/ barrels     W",	// 11
		"W          to earn more points!                                                W",	// 12, 
		"W     + Beware!:                                                               W",	// 13, 
		"W       - Donkey Kong will throw barrels your way and ghosts roam the floors   W",	// 14, 
		"W         dodge them to stay in the game!                                      W",	// 15, 
		"W                                                                              W",	// 16, 
		"W       - Don't fall too far! Mario might be a tough guy,                      W",	// 17, 
		"W         but even he can't survive a long drop!                               W",	// 18, 
		"W                                                                              W", // 19
		"W               Press Enter to go back the the main menu.                      W", // 20
		"W                                                                              W", // 21
		"W                          GOODLUCK!                                           W", // 22
		"W                                                                              W", // 23
		"================================================================================"  // 24
	};

	const char* gameOverScreen[MAX_Y] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1
		"W       ===============================================================        W", // 2
		"W                             GAME OVER                                        W", // 3
		"W       ===============================================================        W", // 4
		"W                                                                              W", // 5
		"W                                                                              W", // 6
		"W                                                                              W", // 7
		"W                    You lost! Better luck next time.                          W", // 8
		"W                                                                              W", // 9
		"W                                                                              W", // 10
		"W                                                                              W", // 11
		"W               you are returning to the main menu, goodbye!                   W", // 12
		"W                                                                              W", // 13
		"W                                                                              W", // 14
		"W                                                                              W", // 15
		"W                                                                              W", // 16
		"W                                                                              W", // 17
		"W                                                                              W", // 18
		"W                                                                              W", // 19
		"W                                                                              W", // 20
		"W                                                                              W", // 21
		"W                                                                              W", // 22
		"W                                                                              W", // 23
		"================================================================================"  // 24
	};

	const char* exitScreen[MAX_Y] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1
		"W       ===============================================================        W", // 2
		"W                              EXITING GAME                                    W", // 3
		"W       ===============================================================        W", // 4
		"W                                                                              W", // 5
		"W                                                                              W", // 6
		"W                                                                              W", // 7
		"W                      you are exiting the game, goodbye!                      W", // 8
		"W                                                                              W", // 9
		"W                                                                              W", // 10
		"W                                                                              W", // 11
		"W                                                                              W", // 12
		"W                                                                              W", // 13
		"W                                                                              W", // 14
		"W                                                                              W", // 15
		"W                                                                              W", // 16
		"W                                                                              W", // 17
		"W                                                                              W", // 18
		"W                                                                              W", // 19
		"W                                                                              W", // 20
		"W                                                                              W", // 21
		"W                                                                              W", // 22
		"W                                                                              W", // 23
		"================================================================================"  // 24
	};

	const char* winningScreen[MAX_Y] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1
		"W       ===============================================================        W", // 2
		"W                                VICTORY!                                      W", // 3
		"W       ===============================================================        W", // 4
		"W                                                                              W", // 5
		"W                                                                              W", // 6
		"W                                                                              W", // 7
		"W                           It's-a me, Mario!                                  W", // 8
		"W                                                                              W", // 9
		"W                                                                              W", // 10
		"W                                                                              W", // 11
		"W             Pauline's in safe hands!  Donkey Kong's defeated!                W", // 12
		"W                                                                              W", // 13
		"W                                                                              W", // 14
		"W                     Press ENTER to return to main menu                       W", // 15
		"W                                                                              W", // 16
		"W                                                                              W", // 17
		"W                                                                              W", // 18
		"W                                                                              W", // 19
		"W                                                                              W", // 20
		"W                                                                              W", // 21
		"W                                                                              W", // 22
		"W                                                                              W", // 23
		"================================================================================"  // 24
	};

	const char* nextLevel[MAX_Y] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                                                                              W", // 1
		"W       ===============================================================        W", // 2
		"W                                GREAT JOB!                                    W", // 3
		"W       ===============================================================        W", // 4
		"W                                                                              W", // 5
		"W                                                                              W", // 6
		"W                                                                              W", // 7
		"W                             You're on fire!                                  W", // 8
		"W                                                                              W", // 9
		"W                                                                              W", // 10
		"W                                                                              W", // 11
		"W                  Let's see if you can beat the next one!                     W", // 12
		"W                                                                              W", // 13
		"W                                                                              W", // 14
		"W                                                                              W", // 15
		"W                                                                              W", // 16
		"W                                                                              W", // 17
		"W                                                                              W", // 18
		"W                                                                              W", // 19
		"W                                                                              W", // 20
		"W                                                                              W", // 21
		"W                                                                              W", // 22
		"W                                                                              W", // 23
		"================================================================================"  // 24
	};




public:

	//menu printing functions
	void presentOpenScreen();

	void presentMainMenu();

	void presentInstructMenu();

	void presentGameOverScreen();

	void presentExitScreen();

	void presentWinScreen();

	void printScreensMenu(const vector <string>& fileNames);

	void printCentered(const string& str);

	void printEmptyLine();

	int handleMainMenuChoice(const vector <string>& fileNames, int numOfBoards, int* currBoard);


	int mainMenuFunction(const vector<string>& fileNames, int numOfBoards, int* currBoard);

	void winning();

	void nextLvl();

};

