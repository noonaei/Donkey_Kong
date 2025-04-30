#pragma once
#include "Game.h"
#include <fstream>

class SaveGame : public Game {
private:
    std::ofstream stepsFile;
    std::ofstream resultFile;
    bool isRecording = false;
    int gameTime = 0;

    
    void clearRecods(int numOfBoards);

public:
    //the same as the game class but clear previous records
    SaveGame() : Game() { clearRecods(fileNames.size()); }
    ~SaveGame() { if (stepsFile.is_open()) stepsFile.close(); if (resultFile.is_open()) resultFile.close(); }

    void loadAndPlayBoard(int boardIndex) override;
    void setupFilesForBoard(int boardIndex);
    void playGame() override;
    void handleInput(int* exitFlag) override;
    void handleDeath() override;
    void updateEntities() override;
    
    
};


