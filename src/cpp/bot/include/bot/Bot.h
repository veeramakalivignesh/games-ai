#ifndef BOTINTERFACE_H
#define BOTINTERFACE_H

#include "../../src/AbstractBot.h"
#include "../../src/cannon/CannonBot.h"

enum Game { Cannon };

class Bot {
  public:
    AbstractBot *botImplementation;

    Bot(Game game);
    string findBestMove(bool isBlackTurn);
};

#endif

// interface with c to be used as a python library
extern "C" {
    void *new_bot_cannon() { return new Bot(Cannon); }

    void find_best_move_cannon(Bot * bot, int **gameStateArr, int numRows, int numColumns, int ***forbiddenStatesArr, int numForbiddenStates,
                               bool isBlackTurn, char *responseMoveBuffer) {
        // create c++ vector from array
        vector<vector<int>> gameState;
        for (int i = 0; i < numRows; i++) {
            vector<int> column;
            for (int j = 0; j < numColumns; j++) {
                column.push_back(gameStateArr[i][j]);
            }
            gameState.push_back(column);
        }

        vector<vector<vector<int>>> forbiddenStates;
        for (int n = 0; n < numForbiddenStates; n++) {
            vector<vector<int>> forbiddenState;
            for (int i = 0; i < numRows; i++) {
                vector<int> column;
                for (int j = 0; j < numColumns; j++) {
                    column.push_back(forbiddenStatesArr[n][i][j]);
                }
                forbiddenState.push_back(column);
            }
            forbiddenStates.push_back(forbiddenState);
        }

        CannonBot *cannonBot = dynamic_cast<CannonBot *>(bot->botImplementation);
        cannonBot->setGameState(gameState);
        cannonBot->setForbiddenStates(forbiddenStates);

        cannonBot->printForbiddenStates();

        string responseMove = bot->findBestMove(isBlackTurn);
        strcpy(responseMoveBuffer, responseMove.c_str());
    }
}