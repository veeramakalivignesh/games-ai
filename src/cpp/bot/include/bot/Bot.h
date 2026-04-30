#ifndef BOTINTERFACE_H
#define BOTINTERFACE_H

#include <cstring>
#include "../../src/AbstractBot.h"
#include "../../src/cannon/CannonBot.h"
#include "../../src/abalone/AbaloneBot.h"

enum Game { Cannon, Abalone };

class Bot {
  public:
    AbstractBot *botImplementation;

    Bot(Game game);
    string findBestMove(bool isBlackTurn);
};

#endif

// C interface used by the Python ctypes layer
extern "C" {
    void *new_bot_cannon() { return new Bot(Cannon); }

    void find_best_move_cannon(Bot *bot, int **gameStateArr, int numRows, int numColumns,
                               int ***forbiddenStatesArr, int numForbiddenStates,
                               bool isBlackTurn, char *responseMoveBuffer) {
        vector<vector<int>> gameState;
        for (int i = 0; i < numRows; i++) {
            vector<int> row;
            for (int j = 0; j < numColumns; j++) row.push_back(gameStateArr[i][j]);
            gameState.push_back(row);
        }

        vector<vector<vector<int>>> forbiddenStates;
        for (int n = 0; n < numForbiddenStates; n++) {
            vector<vector<int>> fs;
            for (int i = 0; i < numRows; i++) {
                vector<int> row;
                for (int j = 0; j < numColumns; j++) row.push_back(forbiddenStatesArr[n][i][j]);
                fs.push_back(row);
            }
            forbiddenStates.push_back(fs);
        }

        CannonBot *cannonBot = dynamic_cast<CannonBot *>(bot->botImplementation);
        cannonBot->setGameState(gameState);
        cannonBot->setForbiddenStates(forbiddenStates);

        string responseMove = bot->findBestMove(isBlackTurn);
        strcpy(responseMoveBuffer, responseMove.c_str());
    }

    void *new_bot_abalone() { return new Bot(Abalone); }

    void find_best_move_abalone(Bot *bot, int **gameStateArr, int numRows, int numColumns,
                                int ***forbiddenStatesArr, int numForbiddenStates,
                                bool isBlackTurn, char *responseMoveBuffer) {
        vector<vector<int>> gameState;
        for (int i = 0; i < numRows; i++) {
            vector<int> row;
            for (int j = 0; j < numColumns; j++) row.push_back(gameStateArr[i][j]);
            gameState.push_back(row);
        }

        vector<vector<vector<int>>> forbiddenStates;
        for (int n = 0; n < numForbiddenStates; n++) {
            vector<vector<int>> fs;
            for (int i = 0; i < numRows; i++) {
                vector<int> row;
                for (int j = 0; j < numColumns; j++) row.push_back(forbiddenStatesArr[n][i][j]);
                fs.push_back(row);
            }
            forbiddenStates.push_back(fs);
        }

        AbaloneBot *abaloneBot = dynamic_cast<AbaloneBot *>(bot->botImplementation);
        abaloneBot->setGameState(gameState, numRows);
        abaloneBot->setForbiddenStates(forbiddenStates);

        string responseMove = bot->findBestMove(isBlackTurn);
        strcpy(responseMoveBuffer, responseMove.c_str());
    }
}
