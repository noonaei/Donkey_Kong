#pragma once
#include "Point.h"
#include "Board.h"


class Game;

class Entity {
public:
    enum class HorizontalDirection {
        None = 0,
        Left = -1,
        Right = 1
    };

    enum class VerticalDirection {
        None = 0,
        Up = -1,
        Down = 1,
        ClimbToFloor = -2,
        ClimbToLadder = 2
    };

protected:

    Point position;
    Board* board = nullptr;
    Game* game = nullptr;

    HorizontalDirection dirHoriz = HorizontalDirection::None;
    VerticalDirection dirVert = VerticalDirection::None;

public:
    Entity(int x, int y, char ch) : position(x, y, ch) {}
    Entity(int x, int y, char ch, HorizontalDirection dir) : position(x, y, ch), dirHoriz(dir) {}

    virtual ~Entity() = default;

    virtual void move() = 0;  // Pure virtual function for move
    void draw() const { position.draw(); }
    void erase() const 
    {
        char originalTile = board->getChar(position.getX(), position.getY());  // Fetch the original tile
        utils::gotoxy(position.getX(), position.getY());
        std::cout << originalTile; // Restore the original tile
    } 

    bool isFloor(char ch) { return ch == '=' || ch == '<' || ch == '>'; }  // Determines if the char is a floor
    bool isLadder(char ch) { return ch == 'H'; }  // Determines if the char is a ladder
    void setBoard(Board* b) { board = b; } //position.setBoard(b); }
    Board* getBoard() const { return board; }
    void setGame(Game* g) { game = g; }
    HorizontalDirection getDirHoriz() const { return dirHoriz; }
    VerticalDirection getDirVert() const  { return dirVert; }
    void reverseDirection() 
    {
        if (dirHoriz == HorizontalDirection::Right) { dirHoriz = HorizontalDirection::Left;}
        else if (dirHoriz == HorizontalDirection::Left) { dirHoriz = HorizontalDirection::Right;}
    }
    void setDirHoriz(HorizontalDirection dir) {dirHoriz = dir;}
    void setDirVert(VerticalDirection dir) { dirVert = dir;}
    const Point& getPosition() const { return position; }
    void setPosition(int newX, int newY) { position.set(newX, newY); }
};



    




