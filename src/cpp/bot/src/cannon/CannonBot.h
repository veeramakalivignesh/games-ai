#ifndef CANNONBOT_H
#define CANNONBOT_H

#include "AbstractBot.h"

class CannonBot : public AbstractBot {
  private:
    vector<vector<int>> gameState;

  public:
    void setGameState(vector<vector<int>> gameState);

    vector<string> getValidMoves(bool isBlackTurn);
    void executeMove(string move);
    bool isGameOver();
    float getUtility();
};

#endif