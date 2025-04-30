#include "Mario.h"
#include "Game.h"
#include <iostream>


void Mario::handleKeyPress(char key) {
    // Validate the input key
    bool isValidKey = false;
    for (size_t i = 0; i < numKeys; ++i) {
        if (key == keys[i]) {
            isValidKey = true;
            break;
        }
    }

    if (!isValidKey) {
        return;  // Invalid key, do nothing
    }

    // Fetch Mario's position and board state
    int x = position.getX();
    int y = position.getY();
    char pos = board->getChar(x, y);
    char below = board->getChar(x, y + 1);
    char belowbelow = board->getChar(x, y + 2);
    char above = board->getChar(x, y - 1);

    // Handle the key input
    switch (key) {

    case 'd':  // Move right
        dirHoriz = HorizontalDirection::Right;
        break;

    case 'a':  // Move left
        dirHoriz = HorizontalDirection::Left;
        break;

    case 'p':
        if (hasHammer && dirHoriz != HorizontalDirection::None) {
            hammerDirection = dirHoriz; // Use hammer in current direction
            useHammer();
        }
        break;

    case 'w':  // Jump or climb up
        if (isLadder(pos)) {  // If on a ladder, climb
            handleClimbing(above);
        }
        else if (isFloor(below)) {  // Jumping logic
            handleJump();
        }
        break;


    case 'x':  // Climb down
        if (isLadder(pos)) {  // If on a ladder, climb down
            dirVert = VerticalDirection::Down;
        }
        else if (isFloor(below) && isLadder(belowbelow)) {
            dirVert = VerticalDirection::ClimbToLadder;
        }
        break;

    case 's':  // Stop all movement
        stopMovement();
        break;

    default:
        // Do nothing for unsupported keys 
        break;
    }
}

void Mario::handleFalling(char below) {
    if (below == ' ') {  // Unsupported
        dirVert = VerticalDirection::Down;  // Fall downward
        isJumping = false;  // No longer jumping
    }
    else {  // Grounded
        dirVert = VerticalDirection::None;  // Stop falling
        isJumping = false;  // Reset jumping state
        jumpStepCount = 0;  // Reset jump counter
    }
}

void Mario::handleJump() {
    if (!isJumping) {  // Start the jump
        dirVert = VerticalDirection::Up;  // Move upward
        isJumping = true;  // Set jumping state
        jumpStepCount = 0;  // Reset jump counter
    }
}

void Mario::handleClimbing(char above) {
    if (isLadder(above)) {
        dirVert = VerticalDirection::Up;  // Climb up
    }
    else if (isFloor(above)) {
        dirVert = VerticalDirection::ClimbToFloor;  // Climb onto the floor
    }
    else {
        dirVert = VerticalDirection::None;  // Stop climbing if no ladder or floor above
    }
}

// Stops all movement
void Mario::stopMovement() {
    dirHoriz = HorizontalDirection::None;
    dirVert = VerticalDirection::None;
}

void Mario::VerticalMovement(char above, char below, int currY) {
    if (dirVert == VerticalDirection::Up && !isJumping) {  // Climbing up
        if (isLadder(above)) {  // Continue climbing the ladder
            position.setY(currY - 1);
        }
        else if (above == ' ') {  // Stop climbing if no ladder or floor above
            dirVert = VerticalDirection::None;
        }
        else if (isFloor(above)) {  // Stop climbing if no ladder or floor above
            dirVert = VerticalDirection::ClimbToFloor;
        }
    }

    if (dirVert == VerticalDirection::ClimbToFloor) { //Climb up the ladder to the floor
        position.setY(currY - 2);
        dirVert = VerticalDirection::None;
    }

    // Handle climbing down
    if (dirVert == VerticalDirection::Down) {  // Climbing down
        if (isLadder(below)) {  // Continue climbing the ladder
            position.setY(currY + 1);
        }
        else if (isFloor(below)) {  // Reached the floor
            dirVert = VerticalDirection::None;  // Stop vertical movement
        }
        else {  // Unsupported below, transition to falling
            handleFalling(below);
        }
    }

    if (dirVert == VerticalDirection::ClimbToLadder) {  // Climb down from the floor to the ladder
        position.setY(currY + 2);
        dirVert = VerticalDirection::Down;
    }

    // Handle falling
    if (!isJumping && below == ' ') {  // Not climbing or jumping
        handleFalling(below);
        fallCount++;
    }

    // Handle jumping
    else if (isJumping) {
        if (jumpStepCount < JUMP_HEIGHT) {  // Allow the jump to continue for 2 steps
            if (isFloor(above) || above == Board::wall) {  // Collision with a floor
                dirVert = VerticalDirection::Down;  // Transition to falling
                isJumping = false;  // End the jump
            }
            else {  // No collision, continue jumping
                position.setY(currY - 1);  // Move upward
                jumpStepCount++;  // Increment jump counter
            }
        }
        else {  // After 2 steps, transition to falling
            dirVert = VerticalDirection::Down;  // Fall downward
            isJumping = false;  // End the jump
        }
    }
    // falling
    if (dirVert == VerticalDirection::Down && below == ' ') {  // Falling
        position.setY(currY + 1);  // Move downward
    }
}

void Mario::HorizontalMovement(char infront, int currX) {
    if (dirHoriz != HorizontalDirection::None && infront != Board::wall) {  // If moving horizontally
        position.setX(currX + static_cast<int>(dirHoriz));  // Update X position
    }
    else if (infront == Board::wall) {  // Collision with wall
        dirHoriz = HorizontalDirection::None;  // Stop horizontal movement
    }
}


void Mario::move() {
    // Fetch Mario's current position
    int currX = position.getX();
    int currY = position.getY();

    // Fetch board state
    char pos = board->getChar(currX, currY);
    char below = board->getChar(currX, currY + 1);
    char above = board->getChar(currX, currY - 1);
    char infront = board->getChar(currX + static_cast<int>(dirHoriz), currY);

    // Check for collision BEFORE moving
    if (checkBarrelCollision()) { Sleep(1000); game->notifyDeath(); return; } // Stop further movement 
    if (checkGhostCollision()) { Sleep(1000); game->notifyDeath(); return; }



    // Handle vertical movement
    VerticalMovement(above, below, currY);

    // Handle horizontal movement
    HorizontalMovement(infront, currX);

    checkFallDamage(below);


    // Check for collision AFTER moving
    if (checkBarrelCollision()) { Sleep(1000); game->notifyDeath(); return; }
    if (checkGhostCollision()) { Sleep(1000); game->notifyDeath(); return; }

    if (pos == Board::Hammer) {
        hasHammer = true;  // Update Mario's state
        board->setChar(currX, currY, ' ');  // Remove hammer from the board
        utils::gotoxy(currX, currY);
        std::cout << ' ';
    }
}


void Mario::checkFallDamage(char below) {
    if ((isFloor(below)) && fallCount >= FALL_DEATH_HEIGHT) {
        Sleep(1000);
        if (game) game->notifyDeath();  // Notify Game of death
        return;  // Stop further processing
    }

    else if (isFloor(below)) {
        fallCount = 0;
    }
}

bool Mario::checkBarrelCollision() const {
    for (int i = 0; i < game->getBarrelCount(); ++i) {
        if (position == game->getBarrel(i)->getPosition()) {
            //game->notifyDeath();
            game->getBarrel(i)->draw();
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}


bool Mario::checkGhostCollision() const {
    for (const auto& ghost : game->getGhosts()) {  // Iterate through vector
        if (position == ghost->getPosition()) {   //  Dereference unique_ptr
            ghost->draw();
            return true;
        }
    }
    return false;
}



bool Mario::checkForVictory()
{
    if (board->getChar(position.getX(), position.getY()) == Board::Pauline) { return true; } // Check if Mario reaches Pauline ($)
    else return false;
}


// Respawns Mario at a specific position
void Mario::respawn() {
    position.erase();
    dirHoriz = HorizontalDirection::None;
    dirVert = VerticalDirection::None;
    fallCount = 0;
    isJumping = false;
    hasHammer = false;
}

void Mario::useHammer() {
    if (!hasHammer || hammerDirection == HorizontalDirection::None) return;

    for (int offset = 1; offset <= HAMMER_RANGE; ++offset) {  // Check up to 2 tiles away
        int targetX = position.getX() + offset * static_cast<int>(hammerDirection);
        int targetY = position.getY();

        // Check for barrels
        for (int i = 0; i < game->getBarrelCount(); ++i) {
            if (game->getBarrel(i)->getPosition() == Point(targetX, targetY)) {
                game->getBarrel(i)->erase();
                game->removeBarrel(game->getBarrel(i));
                game->updateNewScore(game->getPointsPerEnemy()); 
                return;
            }
        }

        // Check for ghosts
        for (const auto& ghost : game->getGhosts()) {  //  Iterate over vector
            if (ghost->getPosition() == Point(targetX, targetY)) {
                ghost->erase();  //  Remove ghost from screen
                game->removeGhost(ghost.get());  //  Pass raw pointer to removeGhost
                game->updateNewScore(game->getPointsPerEnemy());
                return;
            }
        }
    }
}





