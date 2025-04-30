#include "SmartGhost.h"
#include "Game.h"

void SmartGhost::move() {
    
    Board* board = getBoard();
    int marioY = getGame()->getMario()->getPosition().getY();
    int ghostX = position.getX();
    int ghostY = position.getY();

    char below = board->getChar(ghostX, ghostY + 1);
    char above = board->getChar(ghostX, ghostY - 1);
    char current = board->getChar(ghostX, ghostY);

    //  If smartghost is on a ladder
    if (isLadder(current)) {
        if ((ghostY == marioY) && isFloor(below)) {
            Ghost::move(); 
            return; 
        }  

        else if (marioY < ghostY) {
            position.setY(ghostY - (isFloor(above) ? 2 : 1));  // Step onto floor if reached
            return;
        }
        else if (marioY > ghostY && isLadder(below)) {
            position.setY(ghostY + 1);  //  Move down 
            return;
        }
    }

    Ghost::move();
}
