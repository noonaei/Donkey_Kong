#include "Menus.h"
#include <algorithm> // std::min


void Menu::presentOpenScreen()
{
	//just in case, though not a must with how the print fuction works
	system("CLS");

	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << openScreen[i] << '\n';
	}
	std::cout << openScreen[MAX_Y - 1];
};

//menu printing functions
void Menu::presentMainMenu()
{
	//just in case, though not a must with how the print fuction works
	system("CLS");

	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << mainMenu[i] << '\n';
	}
	std::cout << mainMenu[MAX_Y - 1];
};

void Menu::presentInstructMenu()
{
	// Just in case, though not a must with how the print function works
	system("CLS");

	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y; i++) {
		std::cout << instructionMenu[i] << '\n';
	}
};

void Menu::presentGameOverScreen() {

	//just in case, though not a must with how the print fuction works
	system("CLS");
	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << gameOverScreen[i] << '\n';
	}
	std::cout << gameOverScreen[MAX_Y - 1];
}

void Menu::presentExitScreen() {

	//just in case, though not a must with how the print fuction works
	system("CLS");
	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << exitScreen[i] << '\n';
	}
	std::cout << exitScreen[MAX_Y - 1];
}

void Menu::presentWinScreen() {

	//just in case, though not a must with how the print fuction works
	system("CLS");
	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << winningScreen[i] << '\n';
	}
	std::cout << winningScreen[MAX_Y - 1];
}


void Menu::printScreensMenu(const vector <string>& fileNames)
{
	system("CLS");

	string title = "BOARD SELECTION MENU:";
	string prompt = "Please select the board you'd like to start with";
	string note = "Or press enter to return to the main menu.";

	//top border (W-line)
	cout << string(MAX_X, 'W') << endl;

	printEmptyLine();

	printCentered(title);

	cout << "W" << string(MAX_X - 2, '=') << "W" << endl;

	printEmptyLine();

	printCentered(prompt);

	printEmptyLine();

	printCentered(note);

	printEmptyLine();

	// Print the file variations, numbered and centered
	for (int i = 0; i < fileNames.size(); i++) {
		string fileName = to_string(i + 1) + ". " + fileNames[i]; // Number the files 
		printCentered(fileName);
		printEmptyLine();
	}

	// Calculate remaining lines to fill (25 total height, minus lines already printed)
	int remainingLines = MAX_Y - (fileNames.size() + 13); // Subtracting the lines we've used already

	// Fill the remaining lines with empty lines
	for (int i = 0; i < remainingLines; i++) {
		printEmptyLine();
	}

	// Print the bottom border (W-line)
	cout << string(MAX_X, 'W');
}


// Function to print a centered string with W framing
void Menu::printCentered(const string& str)
{
	int padding = (MAX_X - str.length() - 2) / 2;
	cout << "W" << string(padding, ' ') << str << string(MAX_X - 2 - padding - str.length(), ' ') << "W" << endl;
}

// Function to print an empty line with W framing
void Menu::printEmptyLine() {
	cout << "W" << string(MAX_X - 2, ' ') << "W" << endl;
}







int Menu::handleMainMenuChoice(const vector <string>& fileNames, int numOfBoards, int* currBoard)
{
	char key = _getch();
	int board = 0;

	switch (key){

		case NEW_GAME:
			return 1; // Return 1 to start the game

		case CHOOSE_BOARD:
		
			printScreensMenu(fileNames);

			// Waiting for the user to press a valid board number
			do {
				key = _getch();
				// Return to the main menu if enter is pressed
				if (key == '\r')
					return -1;

				board = key - '0'; // Convert character to integer  
			} while (board < 1 || board > numOfBoards); // Ensure the board is within valid range  

			*currBoard = board - 1; // Set the current board (on the screen they are numbered from 1 not 0, hence the -1) 
			return 1;
		
		case SHOW_INSTRUCTIONS: 
			presentInstructMenu(); 

			// Waiting for the user to press Enter to return to the main menu 
			do {
				key = _getch(); 
			} while (key != '\r'); // Wait until the user presses Enter 

			return -1;
		
			
		case EXIT:
			return 0; // Return 0 to terminate the program

		default:
			return 3; // Default to invalid input
		}

}


// main menu printing and input handling
int Menu::mainMenuFunction(const vector <string>& fileNames, int numOfBoards, int* currBoard)
{
	int choice;
	do {

		presentMainMenu();
		choice = handleMainMenuChoice(fileNames, numOfBoards, currBoard);

	} while (choice == -1);

	return choice;
}

void Menu::winning()
{
	presentWinScreen();
	char key = _getch();


	//waiting for the user to press enter to return to the main menu
	//prevent the menu from receiving other key inputs
	do {
		key = _getch();
	} while (key != '\r');
}

void Menu::nextLvl()
{
	//just in case, though not a must with how the print fuction works
	system("CLS");
	utils::gotoxy(0, 0);

	for (int i = 0; i < MAX_Y - 1; i++) {
		std::cout << nextLevel[i] << '\n';
	}
	std::cout << nextLevel[MAX_Y - 1];


}