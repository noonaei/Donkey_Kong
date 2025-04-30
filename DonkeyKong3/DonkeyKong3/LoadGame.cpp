#include "LoadGame.h"
#include <iostream>
#include <fstream>
#include <iomanip>  // for std::setw, std::left, std::right
#include <sstream>




void LoadGame::setupFilesForBoard(int boardIndex) {
    // Close any previously open files
    if (stepsFile.is_open()) stepsFile.close();
    if (resultFile.is_open()) resultFile.close();
   
    // Open the steps file
    std::string stepsFileName = "dkong_0" + std::to_string(boardIndex + 1) + ".steps";
    stepsFile.open(stepsFileName);

    if (!stepsFile) {
        std::cerr << "Error: Could not open steps file " << stepsFileName << "\n";
        return;
    }

    // Open the result file if in silent mode
    if (isSilent) {
        std::string resultFileName = "dkong_0" + std::to_string(boardIndex + 1) + ".result";
        resultFile.open(resultFileName);
        if (!resultFile) {
            std::cerr << "Error: Could not open result file " << resultFileName << "\n";
            return;
        }
    }
}


void LoadGame::playGame() {
    initialize();

    //isSilent = true;  // *for now Run silent mode

    if (isSilent) {
        isRunning = true;

        playSilent();  // Run silent mode
       
        gamesWon++; 
        return;
    }

    int exit = 0;
    board.print();  // Print the board at the start

    while (isRunning) {
        handleInput(&exit);  // Handle input from step file


        if (isPaused) {
            continue;  // Skip updates when paused
        }

        Sleep(LOAD_GAME_SPEED);  // Control game speed
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
        menu.presentGameOverScreen();
        Sleep(SCREEN_DURATION);
    }
    else if (exit == 1) {
        menu.presentExitScreen();
        Sleep(SCREEN_DURATION);
    }
    newScore = prevScore;
}


void LoadGame::handleInput(int* exitFlag) {
    if (!stepsFile.is_open()) return;

    std::string line;
    std::streampos lastPosition = stepsFile.tellg();  // Store the initial position

    if (std::getline(stepsFile, line)) {
        int stepTime = -1;
        char key = '\0';


        // Manually extract `t=<time>`
        size_t tPos = line.find("t=");
        if (tPos != std::string::npos) {
            size_t commaPos = line.find(",", tPos);
            if (commaPos != std::string::npos) {
                stepTime = std::stoi(line.substr(tPos + 2, commaPos - (tPos + 2)));  // Extract number
            }
        }

        // Manually extract `dir=<key>`
        size_t dirPos = line.find("dir=");
        if (dirPos != std::string::npos && dirPos + 4 < line.length()) {
            key = line[dirPos + 4];  // Extract single character
        }

       

        // Only apply the input when the recorded game time matches
        if (stepTime == gameTime && key != '\0') {
            
            mario.handleKeyPress(key);
        }
        else {
            
            stepsFile.seekg(lastPosition);  // Reset the file position
        }

        lastPosition = stepsFile.tellg();  // Update the lastPosition after successful read
        
    }
}


void LoadGame::playSilent() {
    int exit = 0;
    std::ifstream resultFile("dkong_0" + std::to_string(gamesWon + 1) + ".result");  // Open expected result file

    // Redirect std::cout but keep std::cerr visible for debugging
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ofstream nullStream;
    nullStream.open("/dev/null");  // Linux & Mac 
    if (!nullStream) {
        nullStream.open("NUL");  // Windows alternative
    }
    std::cout.rdbuf(nullStream.rdbuf());  // Suppress UI printing

    int prevLives = lives;  // Store initial number of lives

    // Silent mode loop
    while (isRunning) {
        handleInput(&exit);  // Read input from steps file
        updateEntities();  // Update entities (without printing)

        // **Check if Mario lost a life**
        if (lives < prevLives) {
            

            if (resultFile.is_open()) {
                std::string expectedLine;
                std::getline(resultFile, expectedLine);
                
            }

            // **If all lives are lost, validate score is 0 and fail test**
            if (lives == 0) {
                if (newScore != 0) {
                    debugMessages.push_back("Silent mode test failed: Expected score 0 but got " + std::to_string(newScore));

                }
                isRunning = false;
                break;
            }

            prevLives = lives;  // Update previous lives count
        }

        // **Check for victory**
        if (mario.checkForVictory()) {

            if (resultFile.is_open()) {
                std::string expectedLine;
                std::getline(resultFile, expectedLine);
                if (expectedLine.find("Mario,victory") == std::string::npos) {
                    debugMessages.push_back("Silent mode test failed: Mario was expected to win but did not.");
                }

                // **Check score validation**
                if (expectedLine.find("score=") != std::string::npos) {
                    int expectedScore = std::stoi(expectedLine.substr(6));
                    if (expectedScore != newScore) {
                        debugMessages.push_back("Silent mode test failed: Expected score " + std::to_string(expectedScore)
                            + " but got " + std::to_string(newScore));
                    }
                }
            }

            isRunning = false;  // Ensure game exits
            break;
        }
    }

    // Restore original output streams
    std::cout.rdbuf(oldCout);
    nullStream.close();

    debugMessages.push_back("Silent mode test for level completed.\n");
}



void LoadGame::updateEntities() {
    gameTime++;  // Increase time counter
    

    Game::updateEntities();

    // If silent mode is on, verify game results
    if (isSilent && resultFile.is_open()) {
        std::string expectedLine;
        if (std::getline(resultFile, expectedLine)) {
            if (expectedLine.find("Mario,death") != std::string::npos && !isRunning) {
                std:: cerr << "Test failed: Unexpected death detected.";
            }
            if (expectedLine.find("Mario,victory") != std::string::npos && mario.checkForVictory()) {
                std::cerr << "Test failed: Mario was expected to win but did not.";
            }
        }
    }

}


void LoadGame::loadAndPlayBoard(int boardIndex) {
    if (lives > 0)
    {
        setupFilesForBoard(boardIndex);

        if (!stepsFile.is_open()) return;  // Exit if file couldn't be opened

        Game::loadAndPlayBoard(boardIndex);
    }
    
   
    // Close files after playing this board
    stepsFile.close();
    if (resultFile.is_open()) resultFile.close();
}


void LoadGame::run() {
    int numOfBoards = fileNames.size();
    if (numOfBoards == 0) {
        std::cerr << "No game files found. Exiting.\n";
        return;
    }

    for (int i = 0; i < numOfBoards; i++) {
        loadAndPlayBoard(i);
    }

    if (isSilent) {

       system("cls");  // Clear the console
        
        // Print all collected debug messages at once
        for (const auto& message : debugMessages) {
            std::cerr << message << std::endl;
        }
       std::cout << "Silent mode test completed for all levels.\n";
    }
}
