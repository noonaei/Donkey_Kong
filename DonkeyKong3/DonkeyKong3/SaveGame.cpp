#include "SaveGame.h"
#include <iostream>



void SaveGame::handleInput(int* exitFlag) {
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
            stepsFile << "t=" << gameTime << ",Mario,dir=" << key << "\n";  // Log key press
        }
    }
}

void SaveGame ::clearRecods(int numOfBoards) {  

	std::ofstream resultFile;
	std::ofstream stepsFile; 

	for (int i = 0; i < numOfBoards; i++) {
		std::string resultFileName = "dkong_0" + std::to_string(i + 1) + ".result";
		resultFile.open(resultFileName, std::ios::out);
		resultFile.close();
		std::string stepsFileName = "dkong_0" + std::to_string(i + 1) + ".steps";
		stepsFile.open(stepsFileName, std::ios::out);
		stepsFile.close();
	}
}



void SaveGame::handleDeath() {
    resultFile << "t=" << gameTime << ",Mario,death\n";  // Log Mario's death
    resultFile.flush();  // Ensure data is written immediately
    Game::handleDeath();  // Reset game state AFTER logging
}


void SaveGame::updateEntities() {
    gameTime++;
    Game::updateEntities(); // Keep normal updates
}

void SaveGame::playGame() {
    int exit = 0;
    int initialScore = newScore;  // Store score at the beginning of the level

    initialize();  // Calls Game's initialization logic
    board.print();            // Print the board at the start

    while (isRunning) {
        handleInput(&exit);

        if (isPaused) {
            continue;  // Skip updates when paused
        }

        Sleep(GAME_SPEED);
        printLegend();
        updateEntities();

        // Check for victory
        if (mario.checkForVictory()) {
            gamesWon++;
            updateNewScore(PointsPerGame);
            prevScore = newScore;

            int screenScore = newScore - initialScore;  // Calculate score gained on this screen
            resultFile << "t=" << gameTime << ",Mario,victory\n";  // Log victory event
            resultFile << "score=" << screenScore << "\n";  // Log the score for this screen

            if (finishedGame(gamesWon)) {
                gameTime = 0;
                initializeScores();
                gamesWon = 0;
                menu.winning();
            }
            else {
                menu.nextLvl();
                Sleep(SCREEN_DURATION);
            }
            break;
        }
    }

    // Game over or exit handling
    if (lives == 0) {
        initializeScores();
        resultFile << "t=" << gameTime << ",Mario,death\n";  // Log Mario's death
        resultFile << "score=" << newScore << "\n";  // Log the score for this screen, 0
        resultFile.flush();  // Ensure data is written immediately
        gameTime = 0;
        menu.presentGameOverScreen();
        Sleep(SCREEN_DURATION);
    }
    else if (exit == 1) {
        menu.presentExitScreen();
        Sleep(SCREEN_DURATION);
    }

    newScore = prevScore;
}



void SaveGame::loadAndPlayBoard(int boardIndex) {
    
    // Set up the result and steps files
    setupFilesForBoard(boardIndex);

    // Call the base Game function to load and play the board
    Game::loadAndPlayBoard(boardIndex);
  
    // Close the files after finishing this board
    resultFile.close();
    stepsFile.close();
}
 
void SaveGame::setupFilesForBoard(int boardIndex) {
    // Close any previously open result or steps files
    if (resultFile.is_open()) {
        resultFile.close();
    }
    if (stepsFile.is_open()) {
        stepsFile.close();
    }
    

    // Generate the result file name
    std::string resultFileName = "dkong_0" + std::to_string(boardIndex + 1) + ".result";
    resultFile.open(resultFileName, std::ios::out);

    if (!resultFile) {
        std::cerr << "Error: Could not create result file " << resultFileName << "\n";
        return;
    }

    // Generate the steps file name
    std::string stepsFileName = "dkong_0" + std::to_string(boardIndex + 1) + ".steps";
    stepsFile.open(stepsFileName, std::ios::out);

    if (!stepsFile) {
        std::cerr << "Error: Could not create steps file " << stepsFileName << "\n";
        return;
    }
}
