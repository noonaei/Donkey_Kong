#pragma once

#include "Point.h"
#include "Board.h"
#include "Entity.h"
#include "Mario.h"


class Game;

class Barrel : public Entity {
private:
    int fallCount = 0;
    static constexpr int MAX_FALL_BEFORE_EXPLOSION = 8;
    static constexpr int EXPLOSION_RANGE = 2;

    void handleExplosion();
    bool isMarioNearby(const Mario* mario) const;

public:
    Barrel(Game* g = nullptr, int x = 0, int y = 0, char ch = 'O', HorizontalDirection dir = HorizontalDirection::Right) : Entity(x, y, ch, dir) {game = g;}
    
    void move() override;
};





