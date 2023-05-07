#ifndef CANNONBOT_H
#define CANNONBOT_H

#include "../AbstractBot.h"

/**
 * This class contains the bot implementation for the Game-Of-Cannons
 *
 * @author veeramakali vignesh
 */
class CannonBot : public AbstractBot {
  private:
    vector<vector<int>> gameState;
    vector<vector<vector<int>>> forbiddenStates;

  public:
    CannonBot();
    void setGameState(vector<vector<int>> gameState);
    void setForbiddenStates(vector<vector<vector<int>>> forbiddenStates);
    void printGame();
    void printForbiddenStates();

    // implementations of pure virual functions defined in AbstractBot
    AbstractBot* cloneGameState();
    vector<string> getValidMoves(bool isBlackTurn);
    void executeMove(string move);
    bool isGameOver();
    float getUtility();
    bool isBotInForbiddenState(AbstractBot* abstractBot);
};

#endif