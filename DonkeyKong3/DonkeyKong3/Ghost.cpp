#include "Ghost.h"
#include "Board.h"
#include "Game.h"

void Ghost::move() {
    // check ghost collision with other ghost
    friendlyGhostCollision();

    // Check the character below the ghost to ensure it’s on a floor
    char current = board->getChar(position.getX(), position.getY());
    char below = board->getChar(position.getX(), position.getY() + 1);
    if (!isFloor(below)) {
        return; // Do not move if not on a floor
    }

    // 95% chance to continue in the same direction
    if (dist(rng) <= 0.95) {
        // Attempt to move in the current horizontal direction
        int nextX = position.getX() + static_cast<int>(dirHoriz);
        char nextChar = board->getChar(nextX, position.getY());

        // Check if the next position is valid (not a wall or empty space below)
        if (nextChar != Board::wall && isFloor(board->getChar(nextX, position.getY() + 1))) {
            position.setX(nextX);
        }
        else {
            // Reverse direction if movement is blocked
            reverseDirection();
        }
    }
    else { reverseDirection(); } // 5% chance to change direction
        
}


void Ghost::friendlyGhostCollision() {
    for (const auto& otherGhost : game->getGhosts()) {  //  Iterate over vector
        if ((otherGhost.get() != this) && (position == otherGhost->getPosition())) {
            reverseDirection();
            otherGhost->reverseDirection();
        }
    }
}


void Ghost::handleCollision(Ghost& otherGhost) { // Reverse direction when colliding with another ghost
    reverseDirection(); 
    otherGhost.reverseDirection();
}

