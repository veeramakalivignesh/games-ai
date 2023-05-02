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

  public:
    void setGameState(vector<vector<int>> gameState);
    void printGame();

    // implementations of pure virual functions defined in AbstractBot
    AbstractBot* clone();
    vector<string> getValidMoves(bool isBlackTurn);
    void executeMove(string move);
    bool isGameOver();
    float getUtility();
};

#endif