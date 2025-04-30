#include <cstring>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "Board.h"

void Board::load(const std::string& filename)
{
	std::ifstream screen_file(filename);

	// Check if the file is successfully opened
	if (!screen_file.is_open()) {
		system("CLS");

		std::cerr << "\n\n\t\tError: Unable to open board file: " << filename << std::endl;
		return;
	}

	int curr_row = 0;
	int curr_col = 0;
	char c;
	while (!screen_file.get(c).eof() && curr_row < MAX_Y) {
		if (c == '\n') {
			if (curr_col < MAX_X) {
				// add spaces for missing cols
#pragma warning(suppress : 4996)  // to allow strcpy
				strcpy(originalBoard[curr_row] + curr_col, std::string(MAX_X - curr_col - 1, ' ').c_str());
			}
			++curr_row;
			curr_col = 0;
			continue;
		}
		if (curr_col < MAX_X) {
			originalBoard[curr_row][curr_col++] = c;
		}
	}
	int last_row = (curr_row < MAX_Y ? curr_row : MAX_Y - 1);
	// add a closing frame
	// first line
#pragma warning(suppress : 4996) // to allow strcpy
	strcpy(originalBoard[0], std::string(MAX_X, wall).c_str());
	originalBoard[0][MAX_X] = '\n';
	// last line
#pragma warning(suppress : 4996) // to allow strcpy
	strcpy(originalBoard[last_row], std::string(MAX_X, Floor).c_str());
	originalBoard[last_row][MAX_X] = '\0';
	// first col + last col
	for (int row = 1; row < last_row; ++row) {
		originalBoard[row][0] = wall;
		originalBoard[row][MAX_X - 1] = wall;
		originalBoard[row][MAX_X] = '\n';
	}

	// Close the file after reading
	screen_file.close();
}




void Board::reset()
{
	// copy the original board to the current board, as an entire 2d array
	memcpy(currentBoard, originalBoard, MAX_Y * (MAX_X + 1));
	
}


void Board::print() const {
	system("cls");

	std::cout << currentBoard[0];

}

