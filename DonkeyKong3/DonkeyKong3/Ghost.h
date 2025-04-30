#pragma once
#include "Entity.h"
#include <random>

class Game;

class Ghost : public Entity {
private:
    std::mt19937 rng;  // Unique RNG for each ghost
    std::uniform_real_distribution<double> dist;  // Uniform distribution
    void friendlyGhostCollision();
    void handleCollision(Ghost& otherGhost);

public:
    Ghost(Game* g = nullptr, int x = 0, int y = 0, char ch = 'x', HorizontalDirection dir = HorizontalDirection::Right) : Entity(x, y, ch, dir), dist(0.0, 1.0)     // Start moving right by default
    { game = g;
      rng.seed(x * 100 + y);}  // Unique seed per ghost  

    void move() override;

protected:
    Board* getBoard() { return board; }  // Allow access to protected board in Entity
    Game* getGame() { return game; }  // Allow access to protected game in Entity
};



