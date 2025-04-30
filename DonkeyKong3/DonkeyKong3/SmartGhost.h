#pragma once
#include "Ghost.h"  

class SmartGhost : public Ghost {
public:
    SmartGhost(Game* g, int x, int y) : Ghost(g, x, y, 'X') {}

    void move() override;
};

