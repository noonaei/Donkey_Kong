#pragma once
#include <iostream>
#include "utils.h"

class Board;

class Point {
private:
    int _x, _y;     // Coordinates
    char _ch;       // Character representing the entity
   

public:
    Point(int x = 0, int y = 0, char ch = '*') : _x(x), _y(y), _ch(ch) {}
    
    bool operator==(const Point& other) const {return _x == other._x && _y == other._y;}

    
    void set(int x, int y) { _x = x; _y = y; }
    void draw() const { utils::gotoxy(_x, _y); std::cout << _ch; }
    void erase() const { utils::gotoxy(_x, _y); std::cout << ' '; }
    int getX() const { return _x; }
    int getY() const { return _y; }
    void setX(int i) { _x = i; }
    void setY(int i) { _y = i; }
    void setCh(char ch) { _ch = ch; }
};