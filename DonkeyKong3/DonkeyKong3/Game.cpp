#include "Game.h"
#include "SaveGame.h"


// Constructor
Game::Game()
    : mario(0, 0, Board::Player), isRunning(true), isPaused(false), lives(NUM_OF_LIVES), rng(1), directionDist(0, 1) {
    utils::ShowConsoleCursor(false);

    getAllBoardFileNames(fileNames);

    board.reset();            // Initialize the board
    mario.setBoard(&board);   // Set the board for Mario
    mario.setGame(this);     // Allow Mario to interact with the Game class
    
}


// Handle user input
void Game::handleInput(int* exitFlag) {
    if (_kbhit()) {
        char key = std::tolower(_getch());

        if (key == EXIT) {  // Exit the game
            isRunning = false;
            *exitFlag = 1;
        }
        else if (key == ESC) {
            isPaused = !isPaused;  // Toggle pause state
        }
        else if (!isPaused) {
            mario.handleKeyPress(key);  // Forward key to Mario
        }
    }
}

// Reset game state when Mario dies or a new game starts
void Game::handleDeath() {

    board.reset();            // Reset the game board 
    initializeMario(board);
    mario.setBoard(&board);
    barrelCount = 0;          // Clear existing barrels
    spawnCounter = 0;         // Reset barrel spawn timer
    //ghostCount = 0;
    //ghosts.clear();


    if (!gamesWon)
        initializeScores();
    else
        newScore = prevScore;
   
    initializeGhosts(board);
    board.print();            // Redraw the board
}

// Notify when Mario dies
void Game::notifyDeath() {
    lives--;
    if (lives > 0) {
        handleDeath();
        printLegend();
    }
    else {
        isRunning = false;  // End the game
    }
}



void Game::printLegend() const {

    utils::gotoxy(LegendX, LegendY);  // Example position: top-right corner 
    std::cout << "LIVES: " << lives << "  ";  // Add spaces to clear old values

    utils::gotoxy(LegendX, LegendY + 1);
    std::cout << "SCORE: " << newScore << "  ";

    utils::gotoxy(LegendX, LegendY + 2);
    std::cout << "HAMMER: " << (mario.isUsingHammer() ? "Y" : "N");
}


//Spawn a barrel at a fixed location
void Game::spawnBarrel() {
    if (barrelCount < MAX_BARRELS) {
        barrels[barrelCount] = Barrel(this, DonkeyKongX, DonkeyKongY);   // Where donkey kong is, rolling right
        barrels[barrelCount].setBoard(&board);

        if (directionDist(rng) == 0) { barrels[barrelCount].reverseDirection(); }

        //if (barrelCount % 2 == 0) { barrels[barrelCount].reverseDirection(); }
        //Entity::HorizontalDirection direction = (barrelCount % 2 == 0) ? Entity::HorizontalDirection::Right : Entity::HorizontalDirection::Left;
        //barrels[barrelCount].setDirHoriz(direction);
        barrelCount++;
    }
}


void Game::removeBarrel(Barrel* barrelToRemove) {
    for (int i = 0; i < barrelCount; ++i) {
        if (&barrels[i] == barrelToRemove) {  // Compare memory addresses
            barrels[i] = barrels[barrelCount - 1];  // Replace with the last barrel
            barrelCount--;  // Decrease barrel count
            return;  // Exit after removal
        }
    }
}

void Game::removeGhost(Ghost* ghostToRemove) {
    auto it = std::remove_if(ghosts.begin(), ghosts.end(),
        [ghostToRemove](const std::unique_ptr<Ghost>& ghost) {
            return ghost.get() == ghostToRemove;
        });

    if (!ghosts.empty() && it != ghosts.end()) {
        ghosts.erase(it, ghosts.end());
    }
}


void Game::updateEntities() {
    // Update Ghosts
    for (const auto& ghost : ghosts) {  //  Loop through the vector
        updateEntity(*ghost);           //  Dereference unique_ptr before passing
    }

    // Update Barrels
    for (int i = 0; i < barrelCount; ++i) {
        updateEntity(barrels[i]);
    }

    // Update Mario
    updateEntity(mario);

    // Barrel-spawn logic
    spawnCounter++;
    if (spawnCounter >= BARREL_SPAWN_INTERVAL) {
        spawnBarrel();
        spawnCounter = 0;
    }
}


void Game::initialize() {
    rng.seed(1);
    barrelCount = 0;          // Clear existing barrels
    spawnCounter = 0;         // Reset barrel spawn timer
    findLegend(board);
    findDonkeyKong(board);
    initializeMario(board);
    initializeGhosts(board);
}

void Game::playGame() {
    int exit = 0;
   
    initialize();
    board.print();            // Print the board at the start

    while (isRunning) {
        handleInput(&exit);  // Handle user input

        if (isPaused) {
            continue;  // Skip updates when paused
        }

        Sleep(GAME_SPEED);  // Control game speed
        printLegend();

        // update all the ghosts, barrels, and mario
        updateEntities();

        // Check for victory
        if (mario.checkForVictory())
        {
            gamesWon++;
            updateNewScore(PointsPerGame);
            prevScore = newScore;

            if (finishedGame(gamesWon)) {
                initializeScores();
                gamesWon = 0;
                menu.winning();
            }
            else
            {
                menu.nextLvl();
                Sleep(SCREEN_DURATION); 
            }

            break;

        }
    }

    // Game over or exit handling
    if (lives == 0) {
        initializeScores();
        menu.presentGameOverScreen();
        Sleep(SCREEN_DURATION);
    }
    else if (exit == 1) {
        menu.presentExitScreen();
        Sleep(SCREEN_DURATION);
    }
    newScore = prevScore;

}




// Run the game, including menu interaction
void Game::run() {

    // Display the opening screen and pause for a moment
    menu.presentOpenScreen();
    Sleep(SCREEN_DURATION); 

    while (true) {

        int numOfBoards = fileNames.size();


        int currBoard = 0; // Start from the beginning if no selection was made by the user

        // Get user's choice from the main menu
        int choice = menu.mainMenuFunction(fileNames, numOfBoards, &currBoard);

        if (choice == 0) {  // Exit
            menu.presentExitScreen();
            return;
        }

        if (choice == 1) {  // Start the game

            isRunning = true;
            lives = 3;  // Reset lives

            // Check if there are no boards
            if (numOfBoards == 0) {
                system("CLS");
                std::cout << "\n\n\t\tNo boards were found for the game, can't start the game." << std::endl;
                Sleep(SCREEN_DURATION*2);
                return;
            }

            // Load and play the boards, starting from currBoard
            playBoards(currBoard, numOfBoards);
        }
    }
}

void Game::playBoards(int currBoard, int numOfBoards) {
    // Play boards starting from currBoard and then loop back to the beginning if needed
    for (int i = currBoard; i < numOfBoards; i++) {
        loadAndPlayBoard(i);
        if (!isRunning) return;
    }

    // Loop back to boards before currBoard if we didn't start at the first board
    if (currBoard != 0) {
        for (int i = 0; i < currBoard; i++) {
            loadAndPlayBoard(i);
            if (!isRunning) return;
        }
    }
}

void Game::loadAndPlayBoard(int boardIndex) {

    // Load the board, handle death, and play the game
    board.load(fileNames[boardIndex]);

    board.reset();            // Reset the game board 
    mario.setBoard(&board);

    playGame();
}

void Game::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        auto filename = entry.path().filename();
        auto filenameStr = filename.string();
        if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") {
            vec_to_fill.push_back(filenameStr);
        }
    }
}

//function to check if won all levels
bool Game::finishedGame(int gamesWon) { return fileNames.size() == gamesWon; }


void Game::findDonkeyKong(Board& board) {

    for (int y = 0; y < Board::MAX_Y; ++y) {
        for (int x = 0; x < Board::MAX_X; ++x) {
            if (board.getChar(x, y) == '&') {
                DonkeyKongX = x;
                DonkeyKongY = y;
                return;
            }
        }
    }
}

void Game::findLegend(Board& board) {

    for (int y = 0; y < Board::MAX_Y; ++y) {
        for (int x = 0; x < Board::MAX_X; ++x) {
            if (board.getChar(x, y) == Board::legend) {
                LegendX = x;
                LegendY = y;
                return;
            }
        }
    }
}


void Game::initializeMario(Board& board) {
    mario.respawn();

    for (int y = 0; y < Board::MAX_Y; ++y) {
        for (int x = 0; x < Board::MAX_X; ++x) {
            if (board.getChar(x, y) == '@') {
                mario.setPosition(x, y);
                board.setChar(x, y, ' ');
                return;
            }
        }
    }
}


void Game::initializeGhosts(Board& board) {
    ghosts.clear();
    for (int y = 0; y < Board::MAX_Y; ++y) {
        for (int x = 0; x < Board::MAX_X; ++x) {
            bool ghostAdded = false;

            if (board.getChar(x, y) == 'x') {
                ghosts.push_back(std::make_unique<Ghost>(this, x, y));
                ghostAdded = true;
            }
            else if (board.getChar(x, y) == 'X') {
                ghosts.push_back(std::make_unique<SmartGhost>(this, x, y));
                ghostAdded = true;
            }

            //  Now only clears 'x' or 'X' if a ghost was actually added
            if (ghostAdded) {
                ghosts.back()->setBoard(&board);
                board.setChar(x, y, ' ');  // Clear the ghost marker properly
            }
        }
    }
}


void Game::initializeScores()
{
    prevScore = 0;
    newScore = 0;

}

void Game:: updateNewScore( int points)  
{ 
   // Add points to the new score
        newScore += points;
  
}


