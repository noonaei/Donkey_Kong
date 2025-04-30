#pragma once
#include "Point.h"
#include "Board.h"

#include "Entity.h"

class Game;

class Mario : public Entity {
private:
    static constexpr char keys[] = { 'w', 'a', 's', 'd', 'x', 'p' };
    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);
    static constexpr int JUMP_HEIGHT = 2;
    static constexpr int FALL_DEATH_HEIGHT = 5;

    static constexpr int HAMMER_RANGE = 2; // range of 1 is very hard gameplay ( "just before Mario reaches their position" == 1 ?)

    int fallCount = 0;  // Tracks how far Mario has fallen
    bool isJumping = false;  // Prevents multiple jumps
    bool isClimbing = false;  // Prevents multiple jumps
    int jumpStepCount = 0; // Tracks the number of steps during a jump

    bool hasHammer = false;
    HorizontalDirection hammerDirection = HorizontalDirection::None;


    void handleFalling(char below);  // Handles falling logic
    void handleJump();  // Handles jumping logic
    void handleClimbing(char above);  // Handles climbing up or down
    void VerticalMovement(char above, char below, int currY);
    void HorizontalMovement(char infront, int currX);
    bool checkBarrelCollision() const;
    bool checkGhostCollision() const;
    void stopMovement();  // Stops all movement (horizontal and vertical)
    void useHammer();

public:
    Mario(int x, int y, char ch = Board::Player) : Entity(x, y, ch) {}
    Mario(int x, int y) : Entity(x, y, Board::Player) {}
    //Mario() : Entity(StartX, StartY, '@') {}

    void handleKeyPress(char key);  // Handles user input for Mario
    void move() override;  // Updates Mario's position and state

    void checkFallDamage(char below);
    //bool checkForVictory(Point position);
    bool checkForVictory();
    void respawn();  // Respawns Mario at a specific position

    bool isUsingHammer() const { return hasHammer; }
};
