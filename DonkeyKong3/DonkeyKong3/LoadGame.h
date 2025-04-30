#pragma once
#include "Game.h"
#include <fstream>

class LoadGame : public Game {
private:
    std::ifstream stepsFile;
    std::ifstream resultFile;

    
    bool isSilent = false;
    int gameTime = 0;
    static constexpr int LOAD_GAME_SPEED = GAME_SPEED / 5;
 

    // vector to collect silent mode debug messages
    std::vector<std::string> debugMessages; 

    

public:
    LoadGame() : isSilent(false) {}
    LoadGame(bool silentMode) : isSilent(silentMode) {}

    ~LoadGame() {if (stepsFile.is_open()) stepsFile.close(); if (resultFile.is_open()) resultFile.close(); }

    void updateEntities();

    void loadAndPlayBoard(int boardIndex) override;
    void setupFilesForBoard(int boardIndex);
    void playGame();
    void playSilent();
    void handleInput(int* exitFlag) override;
    void run() override;
};
