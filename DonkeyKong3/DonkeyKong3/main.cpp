#include <string>
#include "Game.h"
#include "SaveGame.h"
#include "LoadGame.h"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string mode = argv[1];

        if (mode == "-save") {
            SaveGame saveGame;
            saveGame.run();  // Record and save game data
        }
        else if (mode == "-load") {
            bool silent = (argc > 2 && std::string(argv[2]) == "-silent");
            LoadGame loadGame(silent);
            loadGame.run();  // Load and replay game
        }
        else {
            std::cerr << "Invalid argument. Use -save or -load [-silent]\n";
            return 1;
        }
    }
    else {
        Game game;
        game.run();  // Normal mode (no saving or loading)
    }

    return 0;
}
