#pragma once

class Board {
public:
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
	static constexpr  char wall = 'Q';
	static constexpr  char Floor = '=';
	static constexpr  char Pauline = '$';
	static constexpr  char Player = '@';
	static constexpr  char Hammer = 'p';
	static constexpr  char Donkeykong = '&';
	static constexpr  char legend = 'L';

private:

	char originalBoard[MAX_Y][MAX_X + 1];
	char currentBoard[MAX_Y][MAX_X + 1];

public:

	void load(const std::string& filename);

	void reset();
	void print() const;
	char getChar(int x, int y) const { return currentBoard[y][x]; }
	void setChar(int x, int y, char newCh) { currentBoard[y][x] = newCh; }
	bool isWall(int x, int y) const { return getChar(x, y) == wall; }
	bool isFloor(int x, int y) const { char ch = getChar(x, y); return ch == '=' || ch == '>' || ch == '<'; }

	friend class Game;
};

