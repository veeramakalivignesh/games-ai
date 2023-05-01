#ifndef ABSTRACTBOT_H
#define ABSTRACTBOT_H

#include <iostream>
#include <vector>
using namespace std;

struct Result {
    vector<string> strategy;
    float payoff;
};

class AbstractBot {
  public:
    virtual ~AbstractBot() = default;
    virtual AbstractBot* clone() = 0;
    virtual vector<string> getValidMoves(bool isBlackTurn) = 0;
    virtual void executeMove(string move) = 0;
    virtual bool isGameOver() = 0;
    virtual float getUtility() = 0;

    Result miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta, vector<string> prevDepthStrategy,
                         float timeLimit, bool applyTimeLimit);
    Result iterativeDeepeningSearch(bool isBlackTurn, float timeLimit);
};

#endif