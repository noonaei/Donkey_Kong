#include "Barrel.h"
#include "Game.h"
#include "utils.h"
#include <windows.h>



void Barrel::move() {
    int x = position.getX();
    int y = position.getY();

    char below = board->getChar(x, y + static_cast<int>(VerticalDirection::Down));
    char infront = board->getChar(x + static_cast<int>(dirHoriz), y);

    // Falling logic
    if (below == ' ') {  // Unsupported, keep falling
        fallCount++;
        position.setY(y + static_cast<int>(VerticalDirection::Down));
        return;
    }
    else {
        // Barrel has landed
        if (fallCount >= MAX_FALL_BEFORE_EXPLOSION) {
            handleExplosion();
            return;
        }
        fallCount = 0;  // Reset fall count if barrel lands safely
    }

    // Rolling logic
    if (below == '<') dirHoriz = HorizontalDirection::Left;
    else if (below == '>') dirHoriz = HorizontalDirection::Right;

    // Check if stuck (no movement possible)
    if ((dirHoriz != HorizontalDirection::None && infront == Board::wall) || (dirHoriz == HorizontalDirection::None && below != ' ')) {
        game->removeBarrel(this);  // Barrel is stuck, notify Game to remove it
        return;
    }

    // Horizontal movement
    if (dirHoriz != HorizontalDirection::None && infront != Board::wall) {
        position.setX(x + static_cast<int>(dirHoriz));
    }

}


void Barrel::handleExplosion() {
    // Visualize explosion
    position.erase();
    utils::gotoxy(position.getX(), position.getY());
    std::cout << '*';  // Explosion symbol

    Sleep(20);  // Briefly display explosion
    position.erase();

    // Check if Mario is nearby
    if (isMarioNearby(game->getMario())) {  // Use game pointer
        game->getMario()->erase();
        Sleep(1000);
        game->notifyDeath();  // Mario dies
    }

    position.setCh(' ');  // Make barrel invisible

    // Notify Game to remove this barrel
    if (game) {
        game->removeBarrel(this);
    }
}

bool Barrel::isMarioNearby(const Mario* mario) const {
    int marioX = mario->getPosition().getX();
    int marioY = mario->getPosition().getY();

    int barrelX = position.getX();
    int barrelY = position.getY();

    // Check if Mario is within explosion range
    return (abs(marioX - barrelX) <= EXPLOSION_RANGE && abs(marioY - barrelY) <= EXPLOSION_RANGE);
}

