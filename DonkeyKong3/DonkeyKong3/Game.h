#pragma once


#include <iostream>
#include <windows.h>
#include <conio.h>

#include <vector>
#include <memory>  // For std::unique_ptr
#include <string>
#include <filesystem>

#include "Board.h"
#include "Menus.h"
#include "utils.h"
#include "Point.h"
#include "entity.h"
#include "Mario.h"
#include "Barrel.h"
#include "Ghost.h"
#include "SmartGhost.h"



class Game {
private:
   
    int DonkeyKongX = 0;
    int DonkeyKongY = 0;
    int LegendX = 0;
    int LegendY = 0;

    static constexpr int MAX_BARRELS = 10;  // Max number of barrels
    static constexpr int BARREL_SPAWN_INTERVAL = 25;  // Frames between spawns
    std::mt19937 rng;  // Random number generator
    std::uniform_int_distribution<int> directionDist;  // Random direction (0 or 1)

    void spawnBarrel();  // Spawn a new barrel
    void findDonkeyKong(Board& board);
    void findLegend(Board& board);
    void initializeMario(Board& board);
    void initializeGhosts(Board& board);
    void playBoards(int currBoard, int numOfBoards);
    void getAllBoardFileNames(std::vector<std::string>& vec_to_fill);
    

protected:
    static constexpr int ESC = 27;
    static constexpr char EXIT = '9';
    static constexpr int NUM_OF_LIVES = 3;
    static constexpr int PointsPerGame = 10;
    static constexpr int PointsPerEnemy = 2;
    static constexpr int GAME_SPEED = 150; // lower is faster
    bool isRunning;
    bool isPaused; // flag for pause state
    int lives; // Number of lives Mario has 
    int gamesWon = 0;
    int prevScore = 0;
    int newScore = 0;

    std::vector<std::string> fileNames;

    Menu menu;
    Board board;
    Mario mario;
    Barrel barrels[MAX_BARRELS];  // Fixed-size array for barrels
    int barrelCount = 0;  // Current number of barrels
    int spawnCounter = 0; // Counter for spawning barrels
    std::vector<std::unique_ptr<Ghost>> ghosts;  // Stores both Ghost & SmartGhost

    void initialize();
    void printLegend() const;
    virtual void handleInput(int* exitFlag);
    virtual void playGame(); // actual game loop
    virtual void handleDeath(); // Function to handle death logic
    virtual void updateEntity(Entity& entity) { entity.erase(); entity.move(); entity.draw(); }
    virtual void updateEntities();
    bool finishedGame(int gamesWon);
    virtual void loadAndPlayBoard(int boardIndex);


public:
    Game();
    virtual ~Game() = default;

    
    virtual void run(); // method to run general game, including the menu printing and handling user input

    const Mario* getMario() const { return &mario; }

    int getBarrelCount() const { return barrelCount; }
    Barrel* getBarrel(int index) { return &barrels[index]; }
    
    const std::vector<std::unique_ptr<Ghost>>& getGhosts() const { return ghosts; }

    void notifyDeath(); // Method Mario calls to notify death
    void removeBarrel(Barrel* barrelToRemove);  // Removes the given barrel
    void removeGhost(Ghost* ghostToRemove);   // Removes the given ghost
    void initializeScores();
    void updateNewScore(int points); 
    int getPointsPerEnemy() { return PointsPerEnemy; }  
 
};






