#ifndef ABALONEBOT_H
#define ABALONEBOT_H

#include "../AbstractBot.h"

/**
 * Bot implementation for Abalone.
 * Board values: 0=empty, 1=white, -1=black, 2=forbidden
 */
class AbaloneBot : public AbstractBot {
  private:
    vector<vector<int>> gameState;
    vector<vector<vector<int>>> forbiddenStates;
    int gridSize;
    int edgeSize;

  public:
    AbaloneBot();
    void setGameState(vector<vector<int>> gameState, int gridSize);
    void setForbiddenStates(vector<vector<vector<int>>> forbiddenStates);

    AbstractBot* cloneGameState();
    vector<string> getValidMoves(bool isBlackTurn);
    void executeMove(string move);
    bool isGameOver();
    float getUtility();
    bool isBotInForbiddenState(AbstractBot* abstractBot);
};

#endif
